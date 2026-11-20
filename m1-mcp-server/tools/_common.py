"""Helpers shared by the tool modules."""

import json
from typing import Literal, Optional

import httpx

import m1_http
import state

PatchType = Literal["merge-patch", "json-patch"]


def prepare(m1_url: Optional[str], provisioning_session_id: Optional[str] = None, need_session: bool = True) -> Optional[str]:
    """Remember m1_url and the session id when given; return an error text when something that is needed is missing."""
    err = m1_http.use_m1_url(m1_url)
    if err:
        return err
    if need_session:
        return m1_http.use_session(provisioning_session_id)
    return None


async def create(path: str, body, what: str, key: Optional[str] = None, *, content_type: str = m1_http.JSON,
                 accept: str = m1_http.JSON, extra: str = "", id_label: str = "ID") -> tuple[Optional[str], str]:
    """POST; returns (id of the created resource or None, text for the user)"""
    out = await m1_http.call("POST", path, body=body, content_type=content_type, accept=accept, expect=(201,))
    if not out.ok:
        return None, out.error
    resp = out.response
    new_id = m1_http.last_id(resp, key)
    return new_id, (f"SUCCESS: {what} created.\n\n  {id_label:<12}: {new_id}\n{m1_http.etag_line(resp)}{extra}")


async def retrieve(path: str, what: str, *, accept: str = m1_http.JSON, expect: tuple = (200,), if_none_match: Optional[str] = None) -> str:
    out = await m1_http.call("GET", path, accept=accept, expect=expect)
    if not out.ok:
        return out.error
    resp = out.response
    if resp.status_code == 204:
        return f"{what} exists but has no content yet (HTTP 204)."
    return f"SUCCESS: {what}\n\n{m1_http.etag_line(resp)}{m1_http.pretty(resp)}"


async def replace(path: str, body, what: str, if_match: Optional[str], *, content_type: str = m1_http.JSON, accept: str = m1_http.JSON) -> str:
    out = await m1_http.call("PUT", path, body=body, content_type=content_type, accept=accept, if_match=if_match, expect=(200, 204))
    if not out.ok:
        return out.error
    resp = out.response
    if resp.status_code == 204:
        return f"SUCCESS: {what} replaced; the representation did not change (HTTP 204)."
    return f"SUCCESS: {what} replaced.\n\n{m1_http.etag_line(resp)}{m1_http.pretty(resp)}"


async def patch(path: str, patch_text: str, patch_type: str, what: str, if_match: Optional[str]) -> str:
    try:
        json.loads(patch_text)
    except json.JSONDecodeError as exc:
        return f"ERROR: patch is not valid JSON: {exc}"
    out = await m1_http.call("PATCH", path, body=patch_text, content_type=m1_http.PATCH_MEDIA_TYPES[patch_type], if_match=if_match,
                             expect=(200, 204))
    if not out.ok:
        return out.error
    resp = out.response
    if resp.status_code == 204:
        return f"SUCCESS: {what} patched (HTTP 204)."
    return f"SUCCESS: {what} patched.\n\n{m1_http.etag_line(resp)}{m1_http.pretty(resp)}"


async def destroy(path: str, what: str, if_match: Optional[str], expect: tuple = (204,)) -> str:
    out = await m1_http.call("DELETE", path, if_match=if_match, expect=expect)
    if not out.ok:
        return out.error
    return f"SUCCESS: {what} deleted."


def parse_json_object(text: str, what: str):
    """The JSON of a tool argument that carries a document; (value, None) or (None, error text)"""
    try:
        return json.loads(text), None
    except json.JSONDecodeError as exc:
        return None, f"ERROR: {what} is not valid JSON: {exc}"
