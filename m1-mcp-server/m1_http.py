"""
HTTP access to the Maf_Provisioning API of TS 26.510 V19.2.0 at reference point M1 (base path /3gpp-maf-provisioning/v1).

Every tool goes through `call`, which applies the HTTP rules of clause 7.1:
  - only the methods that the OpenAPI files define are used (never OPTIONS or HEAD);
  - a request without a body carries no Content-Type header field (the AF answers 413 to a GET or DELETE with a body);
  - the strong entity tag that an earlier response carried can be sent back in If-Match (clause 7.1.4.4);
  - an error response is a ProblemDetails (clause 7.1.7): 405 names the Allow header field, 412, 413 and 415 are explained.
"""

import json
from dataclasses import dataclass
from typing import Optional

import httpx

import state

API_PATH = "/3gpp-maf-provisioning/v1"
JSON = "application/json"
PATCH_MEDIA_TYPES = {
    "merge-patch": "application/merge-patch+json",
    "json-patch": "application/json-patch+json",
}


@dataclass
class Outcome:
    """What `call` found: the response (None when there was none) and a ready error text when the call failed."""
    response: Optional[httpx.Response] = None
    error: Optional[str] = None

    @property
    def ok(self) -> bool:
        return self.error is None


def use_m1_url(m1_url: Optional[str]) -> Optional[str]:
    """Remember m1_url when given; return an error text when none is known."""
    if m1_url:
        state.set_m1_url(m1_url)
    if not state.get_m1_url():
        return ("ERROR: No M1 URL configured.\n"
                "Provide m1_url (format: http://host:port), for example m1_url='http://192.168.1.100:7777'.")
    return None


def use_session(provisioning_session_id: Optional[str]) -> Optional[str]:
    """Remember the provisioning session id when given; return an error text when none is known."""
    if provisioning_session_id:
        state.set_session_id(provisioning_session_id)
    if not state.get_session_id():
        return ("ERROR: No Provisioning Session ID found.\n"
                "Run create_provisioning_session first, or pass provisioning_session_id explicitly.")
    return None


def session_path(provisioning_session_id: Optional[str] = None, *suffix: str) -> str:
    """/provisioning-sessions/{id}/suffix... for the id given or the one in the state"""
    sid = provisioning_session_id or state.get_session_id()
    return "/".join(["/provisioning-sessions", sid, *suffix])


def problem_text(response: httpx.Response) -> str:
    """The ProblemDetails of an error response as text (title, detail, invalid parameters), or the start of the body"""
    ctype = response.headers.get("Content-Type", "").split(";")[0].strip().lower()
    if ctype in ("application/problem+json", JSON):
        try:
            pd = response.json()
            lines = [f"  {k}: {pd[k]}" for k in ("title", "status", "detail", "cause") if k in pd]
            for ip in pd.get("invalidParams", []) or []:
                lines.append(f"  invalid parameter {ip.get('param')}: {ip.get('reason', '')}")
            if lines:
                return "\n".join(lines)
        except Exception:
            pass
    return "  " + response.text[:600]


def failure_text(response: httpx.Response, method: str, url: str) -> str:
    """The error text of a response that is not a success"""
    code = response.status_code
    head = f"ERROR: HTTP {code} {response.reason_phrase} for {method} {url}"
    if code == 405:
        head += f"\n  The method is not defined for this resource (Allow: {response.headers.get('Allow', 'not stated')})."
    elif code == 412:
        head += "\n  The resource changed since the entity tag that was sent in If-Match (clause 7.1.4.4): get it again and retry."
    elif code == 413:
        head += "\n  The request body is too large or not allowed for this operation."
    elif code == 415:
        head += f"\n  Unsupported media type (accepted: {response.headers.get('Accept') or response.headers.get('Accept-Patch') or 'not stated'})."
    elif code in (404, 410):
        head += "\n  The resource does not exist (or no longer exists)."
    return head + "\n" + problem_text(response)


async def call(method: str, path: str, *, body=None, content_type: str = JSON, accept: str = JSON,
               if_match: Optional[str] = None, params: Optional[dict] = None, expect: tuple = (200,)) -> Outcome:
    """
    Send one request to the AF.

    body is a str or bytes sent as it is, or a dict or list sent as JSON; None sends no body and no Content-Type.
    The call succeeds when the status code is in expect; otherwise Outcome.error holds the text for the user.
    """
    url = f"{state.get_m1_url()}{API_PATH}{path}"
    headers = {"Accept": accept}
    content = None
    if body is not None:
        content = body if isinstance(body, (str, bytes)) else json.dumps(body)
        headers["Content-Type"] = content_type
    if if_match:
        headers["If-Match"] = if_match
    try:
        async with httpx.AsyncClient(timeout=30.0) as client:
            response = await client.request(method, url, content=content, headers=headers, params=params)
    except httpx.ConnectError:
        return Outcome(error=(f"ERROR: Could not connect to '{state.get_m1_url()}'.\n"
                              "Please check that the m1_url is correct (format: http://host:port) and that the 5GMS Application "
                              "Function is running."))
    except httpx.TimeoutException:
        return Outcome(error=f"ERROR: Request timed out to '{state.get_m1_url()}'.")
    except Exception as exc:  # pylint: disable=broad-except
        return Outcome(error=f"ERROR: {type(exc).__name__}: {exc}")
    if response.status_code in expect:
        return Outcome(response=response)
    return Outcome(response=response, error=failure_text(response, method, url))


def etag_line(response: httpx.Response) -> str:
    etag = response.headers.get("ETag")
    return f"  ETag         : {etag}  (send it as if_match to update or delete this resource)\n" if etag else ""


def pretty(response: httpx.Response) -> str:
    """The JSON body of a response, indented"""
    try:
        return json.dumps(response.json(), indent=2)
    except Exception:
        return response.text


def last_id(response: httpx.Response, key: Optional[str] = None) -> Optional[str]:
    """The identifier of a created resource: from the body property key, else from the Location header field"""
    if key:
        try:
            value = response.json().get(key)
            if value:
                return value
        except Exception:
            pass
    location = response.headers.get("Location", "")
    return location.rstrip("/").split("/")[-1] if location else None
