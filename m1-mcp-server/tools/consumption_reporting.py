"""
Consumption Reporting Configuration tools — 3GPP M1 Interface
Endpoints:
  POST /3gpp-m1/v2/provisioning-sessions/{id}/consumption-reporting-configuration
  GET  /3gpp-m1/v2/provisioning-sessions/{id}/consumption-reporting-configuration
"""

import json
import httpx

from mcp_instance import mcp
import state


# ---------------------------------------------------------------------------
# Tool — Create Consumption Reporting Configuration
# ---------------------------------------------------------------------------
@mcp.tool()
async def create_consumption_reporting_configuration(
    reporting_interval: int = 10,
    sample_percentage: float = 100.0,
    location_reporting: bool = True,
    access_reporting: bool = True,
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    STEP 3 OF 3 — Create a Consumption Reporting Configuration.

    Configures how the 5GMS system collects analytics about how viewers consume
    the media stream. Must be called AFTER create_provisioning_session.

    ALL PARAMETERS ARE OPTIONAL — sensible defaults are provided.

    PARAMETERS
    ----------
    reporting_interval (default: 10, unit: seconds)
        How often clients send consumption reports.
        Recommended: 5 (testing), 10 (default), 30 (medium), 60 (large-scale).

    sample_percentage (default: 100.0, range: 0.0–100.0)
        Percentage of connected clients that will send reports.
        Reduce in large deployments to limit reporting traffic.

    location_reporting (default: true)
        Whether to include the viewer's geographic location in the report.

    access_reporting (default: true)
        Whether to include access network information (WiFi/cellular, signal quality).

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

    RETURNS
    -------
    Confirmation summary. If all 3 steps are complete, a full workflow summary
    is displayed.
    """
    # Allow explicit parameters to override state
    if m1_url:
        state.set_m1_url(m1_url)
    if provisioning_session_id:
        state.set_session_id(provisioning_session_id)

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

    if reporting_interval <= 0:
        return f"ERROR: reporting_interval must be a positive integer (received {reporting_interval})."
    if not (0.0 <= sample_percentage <= 100.0):
        return f"ERROR: sample_percentage must be between 0.0 and 100.0 (received {sample_percentage})."

    session_id = state.get_session_id()
    payload = {
        "reportingInterval": reporting_interval,
        "samplePercentage": sample_percentage,
        "locationReporting": location_reporting,
        "accessReporting": access_reporting,
    }
    url = (
        f"{state.get_m1_url()}/3gpp-m1/v2/provisioning-sessions/"
        f"{session_id}/consumption-reporting-configuration"
    )

    try:
        async with httpx.AsyncClient(timeout=30.0) as client:
            response = await client.post(url, json=payload, headers={"Content-Type": "application/json"})

        if response.status_code in (200, 201, 204):
            return (
                f"SUCCESS: Consumption Reporting Configuration created.\n\n"
                f"  Session ID          : {session_id}\n"
                f"  Reporting Interval  : every {reporting_interval} second(s)\n"
                f"  Sample Percentage   : {sample_percentage}%\n"
                f"  Location Reporting  : {'enabled' if location_reporting else 'disabled'}\n"
                f"  Access Reporting    : {'enabled' if access_reporting else 'disabled'}\n\n"
                f"All 3 steps completed! Your 5GMS session is fully configured:\n"
                f"  [1] Provisioning Session          : {session_id}\n"
                f"  [2] Content Hosting Configuration : created\n"
                f"  [3] Consumption Reporting         : configured\n\n"
                f"The 5GMS Application Function is now ready to serve media to clients."
            )
        return (
            f"ERROR: HTTP {response.status_code} {response.reason_phrase}\n"
            f"  URL: {url}\n  Body: {json.dumps(payload, indent=2)}\n"
            f"  Response: {response.text[:600]}"
        )

    except httpx.ConnectError:
        return f"ERROR: Could not connect to '{state.get_m1_url()}'. Is the 5GMS AF running?"
    except httpx.TimeoutException:
        return f"ERROR: Request timed out to '{state.get_m1_url()}'."
    except Exception as exc:
        return f"ERROR: {type(exc).__name__}: {exc}"


# ---------------------------------------------------------------------------
# Tool — Get Consumption Reporting Configuration
# ---------------------------------------------------------------------------
@mcp.tool()
async def get_consumption_reporting_configuration(
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    Retrieve the Consumption Reporting Configuration for a Provisioning Session.

    PARAMETERS
    ----------
    provisioning_session_id (OPTIONAL)
        The provisioning session whose configuration to retrieve.
        Defaults to the current session stored in state.
        Provide this explicitly when querying a session from a previous interaction.
        Example: "3a2f1b00-1234-5678-abcd-ef0123456789"

    m1_url (OPTIONAL)
        Base URL of the M1 interface. If omitted, the URL from the current state
        is used. Provide this explicitly when resuming a previous session.
        Example: "http://192.168.1.100:7778"

    RETURNS
    -------
    Full JSON representation of the Consumption Reporting Configuration, including
    reporting interval, sample percentage, and location/access reporting flags.
    """
    if m1_url:
        state.set_m1_url(m1_url)
    if provisioning_session_id:
        state.set_session_id(provisioning_session_id)

    if not state.get_m1_url():
        return (
            "ERROR: No M1 URL configured.\n"
            "Either run create_provisioning_session first, or pass m1_url explicitly."
        )
    session_id = state.get_session_id()
    if not session_id:
        return (
            "ERROR: No Provisioning Session ID found.\n"
            "Either run create_provisioning_session first, or pass provisioning_session_id explicitly."
        )

    url = (
        f"{state.get_m1_url()}/3gpp-m1/v2/provisioning-sessions/"
        f"{session_id}/consumption-reporting-configuration"
    )

    try:
        async with httpx.AsyncClient(timeout=30.0) as client:
            response = await client.get(url)

        if response.status_code == 200:
            try:
                data = response.json()
                pretty = json.dumps(data, indent=2)
            except Exception:
                pretty = response.text
            return (
                f"SUCCESS: Consumption Reporting Configuration for session '{session_id}'\n\n"
                f"{pretty}"
            )
        elif response.status_code == 404:
            return (
                f"NOT FOUND: No Consumption Reporting Configuration exists for "
                f"session '{session_id}' (HTTP 404).\n"
                f"Use create_consumption_reporting_configuration to create one."
            )
        return (
            f"ERROR: HTTP {response.status_code} {response.reason_phrase}\n"
            f"  URL: {url}\n  Response: {response.text[:600]}"
        )

    except httpx.ConnectError:
        return f"ERROR: Could not connect to '{state.get_m1_url()}'. Is the 5GMS AF running?"
    except httpx.TimeoutException:
        return f"ERROR: Request timed out to '{state.get_m1_url()}'."
    except Exception as exc:
        return f"ERROR: {type(exc).__name__}: {exc}"
