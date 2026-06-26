"""
Content Hosting Configuration tools — 3GPP M1 Interface
Endpoints:
  POST   /3gpp-m1/v2/provisioning-sessions/{id}/content-hosting-configuration
  GET    /3gpp-m1/v2/provisioning-sessions/{id}/content-hosting-configuration
  DELETE /3gpp-m1/v2/provisioning-sessions/{id}/content-hosting-configuration
"""

import json
from pathlib import Path
import httpx

from mcp_instance import mcp
import state

TEMPLATE_PATH = Path(__file__).parent.parent / "content_hosting_config_template.json"


# ---------------------------------------------------------------------------
# Helper
# ---------------------------------------------------------------------------
def _base_url_for_session(session_id: str) -> str:
    return (
        f"{state.get_m1_url()}/3gpp-m1/v2/provisioning-sessions/"
        f"{session_id}/content-hosting-configuration"
    )


def _connect_error(url: str) -> str:
    return f"ERROR: Could not connect to '{url}'. Is the 5GMS AF running?"


def _check_state() -> str | None:
    """Return an error string if m1_url or session_id are missing, else None."""
    if not state.get_m1_url():
        return (
            "ERROR: No M1 URL configured.\n"
            "Either run create_provisioning_session first, or pass m1_url explicitly."
        )
    if not state.get_session_id():
        return (
            "ERROR: No Provisioning Session ID found.\n"
            "Either run create_provisioning_session first, or pass provisioning_session_id explicitly."
        )
    return None


# ---------------------------------------------------------------------------
# Tool — Create Content Hosting Configuration
# ---------------------------------------------------------------------------
@mcp.tool()
async def create_content_hosting_configuration(
    name: str,
    ingest_base_url: str,
    entry_point_relative_path: str,
    entry_point_content_type: str = "application/dash+xml",
    domain_name_alias: str = None,
    dash_profiles: list[str] = None,
    ingest_pull: bool = True,
    ingest_protocol: str = "urn:3gpp:5gms:content-protocol:http-pull-ingest",
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    STEP 2 OF 3 — Create a Content Hosting Configuration.

    Defines WHERE the media content comes from (ingest origin server) and HOW it will
    be distributed to end-user devices. Must be called AFTER create_provisioning_session.

    PARAMETERS
    ----------
    name (REQUIRED)
        Human-readable label for this configuration.
        Example: "My Live Stream", "VOD Library", "Sports Channel HD"

    ingest_base_url (REQUIRED)
        Base URL of the origin server from which the 5GMS AF will pull media content.
        Example: "https://rdmedia.bbc.co.uk/"

    entry_point_relative_path (REQUIRED)
        Path to the media manifest relative to ingest_base_url.
        Example (DASH): "elephants_dream/1/client_manifest-all.mpd"
        Example (HLS):  "live/channel1/index.m3u8"

    entry_point_content_type (default: "application/dash+xml")
        MIME type: "application/dash+xml" for DASH, "application/vnd.apple.mpegurl" for HLS.

    domain_name_alias (default: none)
        Alternate hostname clients use to reach the distribution point.

    dash_profiles (default: ["urn:mpeg:dash:profile:isoff-live:2011"])
        List of DASH profile URNs.

    ingest_pull (default: true)
        true → AF pulls from origin. false → content is pushed to the AF.

    ingest_protocol (default: "urn:3gpp:5gms:content-protocol:http-pull-ingest")
        Protocol URN for the ingest method.

    provisioning_session_id (OPTIONAL)
        The ID of an existing provisioning session to attach this configuration to.
        If omitted, the session ID from the current state is used (set automatically
        by create_provisioning_session). Provide this explicitly when resuming work
        on a session that was created in a previous interaction.
        Example: "3a2f1b00-1234-5678-abcd-ef0123456789"

    m1_url (OPTIONAL)
        Base URL of the M1 interface. If omitted, the URL from the current state
        is used. Provide this explicitly when resuming a previous session.
        Example: "http://192.168.1.100:7778"
    """
    # Allow explicit parameters to override state
    if m1_url:
        state.set_m1_url(m1_url)
    if provisioning_session_id:
        state.set_session_id(provisioning_session_id)

    err = _check_state()
    if err:
        return err

    session_id = state.get_session_id()

    try:
        with open(TEMPLATE_PATH, "r", encoding="utf-8") as f:
            config = json.load(f)
    except FileNotFoundError:
        config = {
            "name": "",
            "ingestConfiguration": {
                "pull": True,
                "protocol": "urn:3gpp:5gms:content-protocol:http-pull-ingest",
                "baseURL": "",
            },
            "distributionConfigurations": [
                {
                    "entryPoint": {
                        "relativePath": "",
                        "contentType": "application/dash+xml",
                        "profiles": ["urn:mpeg:dash:profile:isoff-live:2011"],
                    }
                }
            ],
        }

    config["name"] = name
    config["ingestConfiguration"]["pull"] = ingest_pull
    config["ingestConfiguration"]["protocol"] = ingest_protocol
    config["ingestConfiguration"]["baseURL"] = (
        ingest_base_url if ingest_base_url.endswith("/") else ingest_base_url + "/"
    )

    profiles = dash_profiles or ["urn:mpeg:dash:profile:isoff-live:2011"]
    dist = {
        "entryPoint": {
            "relativePath": entry_point_relative_path.lstrip("/"),
            "contentType": entry_point_content_type,
            "profiles": profiles,
        }
    }
    if domain_name_alias:
        dist["domainNameAlias"] = domain_name_alias
    config["distributionConfigurations"] = [dist]

    url = _base_url_for_session(session_id)

    try:
        async with httpx.AsyncClient(timeout=30.0) as client:
            response = await client.post(url, json=config, headers={"Content-Type": "application/json"})

        if response.status_code in (200, 201, 204):
            return (
                f"SUCCESS: Content Hosting Configuration created.\n\n"
                f"  Session ID   : {session_id}\n"
                f"  Name         : {name}\n"
                f"  Ingest URL   : {config['ingestConfiguration']['baseURL']}\n"
                f"  Entry Point  : {entry_point_relative_path}\n"
                f"  Content Type : {entry_point_content_type}\n"
                + (f"  Domain Alias : {domain_name_alias}\n" if domain_name_alias else "")
                + f"\nNEXT STEP → Call create_consumption_reporting_configuration"
            )
        return (
            f"ERROR: HTTP {response.status_code} {response.reason_phrase}\n"
            f"  URL: {url}\n  Body:\n{json.dumps(config, indent=2)}\n"
            f"  Response: {response.text[:600]}"
        )

    except httpx.ConnectError:
        return _connect_error(state.get_m1_url())
    except httpx.TimeoutException:
        return f"ERROR: Request timed out to '{state.get_m1_url()}'."
    except Exception as exc:
        return f"ERROR: {type(exc).__name__}: {exc}"


# ---------------------------------------------------------------------------
# Tool — Get Content Hosting Configuration
# ---------------------------------------------------------------------------
@mcp.tool()
async def get_content_hosting_configuration(
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    Retrieve the Content Hosting Configuration for a Provisioning Session.

    PARAMETERS
    ----------
    provisioning_session_id (OPTIONAL)
        The provisioning session to query. Defaults to the current session in state.

    m1_url (OPTIONAL if already provided)
        Base URL of the 5GMS Application Function M1 interface.
        Example: "http://192.168.1.100:7778"

    RETURNS
    -------
    Full JSON representation of the Content Hosting Configuration, including
    ingest settings and distribution entry points.
    """
    if m1_url:
        state.set_m1_url(m1_url)
    if not state.get_m1_url():
        return "ERROR: No M1 URL configured. Provide m1_url parameter."

    session_id = provisioning_session_id or state.get_session_id()
    if not session_id:
        return "ERROR: No provisioning_session_id provided or stored in state."

    url = _base_url_for_session(session_id)

    try:
        async with httpx.AsyncClient(timeout=30.0) as client:
            response = await client.get(url)

        if response.status_code == 200:
            try:
                data = response.json()
                pretty = json.dumps(data, indent=2)
            except Exception:
                pretty = response.text
            return f"SUCCESS: Content Hosting Configuration for session '{session_id}'\n\n{pretty}"
        elif response.status_code == 404:
            return (
                f"NOT FOUND: No Content Hosting Configuration exists for session '{session_id}' (HTTP 404).\n"
                "Call create_content_hosting_configuration to create one."
            )
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
# Tool — Delete Content Hosting Configuration
# ---------------------------------------------------------------------------
@mcp.tool()
async def delete_content_hosting_configuration(
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    Delete the Content Hosting Configuration for a Provisioning Session.

    Removes the ingest and distribution settings from the session. The Provisioning
    Session itself is NOT deleted — only this sub-resource is removed.

    PARAMETERS
    ----------
    provisioning_session_id (OPTIONAL)
        The provisioning session whose Content Hosting Configuration to delete.
        Defaults to the current session in state.

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
        return "ERROR: No provisioning_session_id provided or stored in state."

    url = _base_url_for_session(session_id)

    try:
        async with httpx.AsyncClient(timeout=30.0) as client:
            response = await client.delete(url)

        if response.status_code in (200, 204):
            return (
                f"SUCCESS: Content Hosting Configuration for session '{session_id}' deleted.\n"
                "The Provisioning Session remains active. You can create a new configuration with "
                "create_content_hosting_configuration."
            )
        elif response.status_code == 404:
            return f"ERROR: No Content Hosting Configuration found for session '{session_id}' (HTTP 404)."
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
