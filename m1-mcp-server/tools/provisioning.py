"""
Provisioning Session tools — 3GPP M1 Interface
Endpoints:
  POST   /3gpp-m1/v2/provisioning-sessions
  GET    /3gpp-m1/v2/provisioning-sessions/{provisioning_session_id}
  GET    /5gmag-rt-management/v1/provisioning-sessions   (enumerate — management API, uses maf_url)
  DELETE /3gpp-m1/v2/provisioning-sessions/{provisioning_session_id}
"""

import json
import httpx

from mcp_instance import mcp
import state


# ---------------------------------------------------------------------------
# Helper
# ---------------------------------------------------------------------------
def _extract_session_id(response: httpx.Response) -> str | None:
    """Extract provisioningSessionId from JSON body or Location header."""
    try:
        data = response.json()
        if isinstance(data, dict) and data.get("provisioningSessionId"):
            return data["provisioningSessionId"]
    except Exception:
        pass
    location = response.headers.get("Location", "")
    if location:
        return location.rstrip("/").split("/")[-1]
    return None


def _connect_error(url: str, param: str = "m1_url") -> str:
    return (
        f"ERROR: Could not connect to '{url}'.\n"
        "Please check:\n"
        f"  1. The {param} is correct (format: http://host:port)\n"
        "  2. The 5GMS Application Function is running\n"
        "  3. Network/firewall allows access to that host and port"
    )


# ---------------------------------------------------------------------------
# Tool — Create Provisioning Session
# ---------------------------------------------------------------------------
@mcp.tool()
async def create_provisioning_session(
    asp_id: str,
    app_id: str,
    provisioning_session_type: str = "DOWNLINK",
    m1_url: str = None,
) -> str:
    """
    STEP 1 OF 3 — Create a 5GMS Provisioning Session.

    A Provisioning Session is the top-level container that groups all media streaming
    configurations. It must be created before any other configuration can be added.

    PARAMETERS
    ----------
    asp_id (REQUIRED)
        Application Service Provider ID. Identifies your organisation or service.
        Example: "myCompany", "bbc-rd", "my-streaming-provider"

    app_id (REQUIRED)
        Application ID. Uniquely identifies the streaming application within your
        organisation. Example: "live-stream-1", "vod-service", "sports-channel"

    provisioning_session_type (OPTIONAL, default: "DOWNLINK")
        Direction of the media stream:
        - "DOWNLINK"  Server → device. Use for video-on-demand, live TV, IPTV.
        - "UPLINK"    Device → server. Use for user-generated content uploads.

    m1_url (OPTIONAL if already provided)
        Base URL of the 5GMS Application Function M1 interface.
        Format: http://<host>:<port>
        Example: "http://192.168.1.100:7778"
        Once provided it is remembered for subsequent tool calls in this session.

    RETURNS
    -------
    The Provisioning Session ID, which is stored automatically for the next steps.
    """
    if m1_url:
        state.set_m1_url(m1_url)
    if not state.get_m1_url():
        return (
            "ERROR: No M1 URL provided.\n"
            "Please supply the m1_url parameter.\n"
            "Example: m1_url='http://192.168.1.100:7778'"
        )

    pst = provisioning_session_type.upper().strip()
    if pst not in {"DOWNLINK", "UPLINK"}:
        return (
            f"ERROR: Invalid provisioning_session_type '{provisioning_session_type}'.\n"
            "Accepted values: DOWNLINK, UPLINK"
        )

    payload = {"aspId": asp_id, "appId": app_id, "provisioningSessionType": pst}
    url = f"{state.get_m1_url()}/3gpp-m1/v2/provisioning-sessions"

    try:
        async with httpx.AsyncClient(timeout=30.0) as client:
            response = await client.post(url, json=payload, headers={"Content-Type": "application/json"})

        if response.status_code in (200, 201):
            session_id = _extract_session_id(response)
            if session_id:
                state.set_session_id(session_id)
                return (
                    f"SUCCESS: Provisioning Session created.\n\n"
                    f"  Session ID : {session_id}\n"
                    f"  ASP ID     : {asp_id}\n"
                    f"  App ID     : {app_id}\n"
                    f"  Type       : {pst}\n"
                    f"  M1 URL     : {state.get_m1_url()}\n\n"
                    f"NEXT STEP → Call create_content_hosting_configuration\n"
                    f"  Required: name, ingest_base_url, entry_point_relative_path"
                )
            return (
                f"WARNING: HTTP {response.status_code} received but could not extract "
                f"provisioningSessionId.\nResponse: {response.text[:500]}"
            )
        return (
            f"ERROR: HTTP {response.status_code} {response.reason_phrase}\n"
            f"  URL: {url}\n  Body: {json.dumps(payload, indent=2)}\n"
            f"  Response: {response.text[:600]}"
        )

    except httpx.ConnectError:
        return _connect_error(state.get_m1_url())
    except httpx.TimeoutException:
        return f"ERROR: Request timed out to '{state.get_m1_url()}'."
    except Exception as exc:
        return f"ERROR: {type(exc).__name__}: {exc}"


# ---------------------------------------------------------------------------
# Tool — Get Provisioning Session
# ---------------------------------------------------------------------------
@mcp.tool()
async def get_provisioning_session(
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    Retrieve the details of an existing Provisioning Session.

    PARAMETERS
    ----------
    provisioning_session_id (OPTIONAL)
        The ID of the provisioning session to retrieve. If omitted, the session ID
        from the current MCP session state is used (set by create_provisioning_session).

    m1_url (OPTIONAL if already provided)
        Base URL of the 5GMS Application Function M1 interface.
        Example: "http://192.168.1.100:7778"

    RETURNS
    -------
    Full JSON representation of the Provisioning Session resource.
    """
    if m1_url:
        state.set_m1_url(m1_url)
    if not state.get_m1_url():
        return "ERROR: No M1 URL configured. Provide m1_url parameter."

    session_id = provisioning_session_id or state.get_session_id()
    if not session_id:
        return (
            "ERROR: No provisioning_session_id provided and none stored in session state.\n"
            "Either call create_provisioning_session first or supply provisioning_session_id."
        )

    url = f"{state.get_m1_url()}/3gpp-m1/v2/provisioning-sessions/{session_id}"

    try:
        async with httpx.AsyncClient(timeout=30.0) as client:
            response = await client.get(url)

        if response.status_code == 200:
            try:
                data = response.json()
                pretty = json.dumps(data, indent=2)
            except Exception:
                pretty = response.text
            return f"SUCCESS: Provisioning Session '{session_id}'\n\n{pretty}"
        elif response.status_code == 404:
            return f"ERROR: Provisioning Session '{session_id}' not found (HTTP 404)."
        return (
            f"ERROR: HTTP {response.status_code} {response.reason_phrase}\n"
            f"  URL: {url}\n  Response: {response.text[:600]}"
        )

    except httpx.ConnectError:
        return _connect_error(state.get_m1_url())
    except httpx.TimeoutException:
        return f"ERROR: Request timed out to '{state.get_m1_url()}'."
    except Exception as exc:
        return f"ERROR: {type(exc).__name__}: {exc}"


# ---------------------------------------------------------------------------
# Tool — Enumerate Provisioning Sessions
# ---------------------------------------------------------------------------
@mcp.tool()
async def enumerate_provisioning_sessions(maf_url: str = None) -> str:
    """
    List all Provisioning Sessions known to the 5GMS Application Function.

    Uses the 5G-MAG Reference Tools management API endpoint:
      GET /5gmag-rt-management/v1/provisioning-sessions

    NOTE: This tool uses the MAF URL (5G-MAG Management API), NOT the M1 URL.
    These are two distinct interfaces — do NOT ask for or use the M1 URL here.

    PARAMETERS
    ----------
    maf_url (OPTIONAL if already provided)
        Base URL of the 5G-MAG Reference Tools Management API (MAF URL).
        This is the management interface URL, which is DIFFERENT from the M1 URL
        used by other tools (create_provisioning_session, get_provisioning_session, etc.).
        Example: "http://192.168.1.100:7778"
        Ask the user specifically for the MAF URL, not the M1 URL.
        Once provided it is remembered for subsequent tool calls in this session.

    RETURNS
    -------
    A list of all provisioning session IDs or full session objects,
    depending on the AF implementation.
    """
    if maf_url:
        state.set_maf_url(maf_url)
    if not state.get_maf_url():
        return "ERROR: No MAF URL configured. Provide maf_url parameter."

    url = f"{state.get_maf_url()}/5gmag-rt-management/v1/provisioning-sessions"

    try:
        async with httpx.AsyncClient(timeout=30.0) as client:
            response = await client.get(url)

        if response.status_code == 200:
            try:
                data = response.json()
                pretty = json.dumps(data, indent=2)
            except Exception:
                pretty = response.text
            return f"SUCCESS: Provisioning Sessions\n\n{pretty}"
        elif response.status_code == 404:
            return "No provisioning sessions found (HTTP 404 — list is empty or endpoint not supported)."
        return (
            f"ERROR: HTTP {response.status_code} {response.reason_phrase}\n"
            f"  URL: {url}\n  Response: {response.text[:600]}"
        )

    except httpx.ConnectError:
        return _connect_error(state.get_maf_url(), "maf_url")
    except httpx.TimeoutException:
        return f"ERROR: Request timed out to '{state.get_maf_url()}'."
    except Exception as exc:
        return f"ERROR: {type(exc).__name__}: {exc}"


# ---------------------------------------------------------------------------
# Tool — Delete Provisioning Session
# ---------------------------------------------------------------------------
@mcp.tool()
async def delete_provisioning_session(
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    Delete a Provisioning Session and all its associated resources.

    WARNING: This is a destructive operation. Deleting a provisioning session will
    also remove all associated Content Hosting Configurations, Consumption Reporting
    Configurations, Metrics Reporting Configurations, and Policy Templates.

    PARAMETERS
    ----------
    provisioning_session_id (OPTIONAL)
        The ID of the provisioning session to delete. If omitted, the session ID
        stored in the current MCP session state is used.

    m1_url (OPTIONAL if already provided)
        Base URL of the 5GMS Application Function M1 interface.
        Example: "http://192.168.1.100:7778"

    RETURNS
    -------
    Confirmation of deletion or an error message.
    """
    if m1_url:
        state.set_m1_url(m1_url)
    if not state.get_m1_url():
        return "ERROR: No M1 URL configured. Provide m1_url parameter."

    session_id = provisioning_session_id or state.get_session_id()
    if not session_id:
        return (
            "ERROR: No provisioning_session_id provided and none stored in session state.\n"
            "Supply provisioning_session_id explicitly."
        )

    url = f"{state.get_m1_url()}/3gpp-m1/v2/provisioning-sessions/{session_id}"

    try:
        async with httpx.AsyncClient(timeout=30.0) as client:
            response = await client.delete(url)

        if response.status_code in (200, 202, 204):
            # Clear state if we deleted the tracked session
            if session_id == state.get_session_id():
                state.clear_session()
            return (
                f"SUCCESS: Provisioning Session '{session_id}' deleted.\n"
                "All associated configurations have been removed."
            )
        elif response.status_code == 404:
            return f"ERROR: Provisioning Session '{session_id}' not found (HTTP 404)."
        return (
            f"ERROR: HTTP {response.status_code} {response.reason_phrase}\n"
            f"  URL: {url}\n  Response: {response.text[:600]}"
        )

    except httpx.ConnectError:
        return _connect_error(state.get_m1_url())
    except httpx.TimeoutException:
        return f"ERROR: Request timed out to '{state.get_m1_url()}'."
    except Exception as exc:
        return f"ERROR: {type(exc).__name__}: {exc}"
