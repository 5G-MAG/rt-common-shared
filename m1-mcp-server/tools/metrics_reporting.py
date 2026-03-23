"""
Metrics Reporting Configuration tools — 3GPP M1 Interface
Endpoints:
  POST   /3gpp-m1/v2/provisioning-sessions/{id}/metrics-reporting-configurations
  GET    /3gpp-m1/v2/provisioning-sessions/{id}/metrics-reporting-configurations/{metrics_id}
  DELETE /3gpp-m1/v2/provisioning-sessions/{id}/metrics-reporting-configurations/{metrics_id}
"""

import json
import httpx

from mcp_instance import mcp
import state


# ---------------------------------------------------------------------------
# Helper
# ---------------------------------------------------------------------------
def _build_url(session_id: str, metrics_id: str) -> str:
    return (
        f"{state.get_m1_url()}/3gpp-m1/v2/provisioning-sessions/"
        f"{session_id}/metrics-reporting-configurations/{metrics_id}"
    )


def _connect_error(url: str) -> str:
    return f"ERROR: Could not connect to '{url}'. Is the 5GMS AF running?"


# ---------------------------------------------------------------------------
# Tool — Create Metrics Reporting Configuration
# ---------------------------------------------------------------------------
@mcp.tool()
async def create_metrics_reporting_configuration(
    scheme: str = "urn:3gpp:5gms:metrics-reporting:qoe-metrics",
    reporting_interval: int = 10,
    sample_percentage: float = 100.0,
    sampling_period: int = None,
    metrics: list[str] = None,
    url_filters: list[str] = None,
    data_network_name: str = None,
    slice_scope: list[dict] = None,
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    Create a Metrics Reporting Configuration for a Provisioning Session.

    Configures which QoE/performance metrics the 5GMS client will collect and
    how often they are reported to the metrics server. Multiple configurations
    can exist per session, each identified by a unique ID returned on creation.

    PARAMETERS
    ----------
    scheme (default: "urn:3gpp:5gms:metrics-reporting:qoe-metrics")
        URN identifying the metrics reporting scheme.
        Defines the format and semantics of the collected metrics.
        Common values:
          "urn:3gpp:5gms:metrics-reporting:qoe-metrics" — standard 3GPP QoE metrics

    reporting_interval (default: 10, unit: seconds)
        How often clients send metrics reports to the reporting server.
        Recommended: 5 (testing), 10 (default), 30 (medium), 60 (large-scale).
        Must be a positive integer.

    sample_percentage (default: 100.0, range: 0.0–100.0)
        Percentage of connected clients that will send metrics reports.
        Reduce in large deployments to limit reporting traffic.
          100.0 → all clients report (recommended for testing)
           50.0 → half of clients report
           10.0 → 10% of clients (large-scale production)

    sampling_period (OPTIONAL, unit: seconds)
        How frequently the client samples metric values locally before aggregating
        them into a report. Must be a positive integer if provided.
        Example: 5 → sample every 5 seconds, report every reporting_interval seconds.

    metrics (OPTIONAL)
        List of metric URNs specifying which metrics to collect.
        If omitted, the AF applies its default set.
        Common URNs (3GPP DASH QoE):
          "urn:3GPP:ns:PSS:DASH:QM10#HTTPList"
          "urn:3GPP:ns:PSS:DASH:QM10#BufferLevel"
          "urn:3GPP:ns:PSS:DASH:QM10#RepSwitchList"
          "urn:3GPP:ns:PSS:DASH:QM10#MPDInformation"

    url_filters (OPTIONAL)
        List of URL pattern strings. When provided, reporting is restricted to
        requests matching at least one of these patterns.
        Example: ["https://cdn.example.com/live/*"]
        If omitted, all requests are covered.

    data_network_name (OPTIONAL)
        Data Network Name (DNN / APN) to scope this configuration to a specific
        network slice or access point. Leave empty for no restriction.
        Example: "internet"

    slice_scope (OPTIONAL)
        List of S-NSSAI objects identifying the network slices this configuration
        applies to. Each entry is a dict with at minimum an "sst" (Slice/Service Type)
        integer and optionally an "sd" (Slice Differentiator) string.
        Example: [{"sst": 1, "sd": "000001"}]
        If omitted or empty, the configuration applies to all slices.

    provisioning_session_id (OPTIONAL)
        The provisioning session to attach this configuration to.
        Defaults to the current session stored in state.
        Provide this explicitly when resuming a session from a previous interaction.
        Example: "3a2f1b00-1234-5678-abcd-ef0123456789"

    m1_url (OPTIONAL)
        Base URL of the M1 interface. If omitted, the URL from the current state
        is used. Provide this explicitly when resuming a previous session.
        Example: "http://192.168.1.100:7778"

    RETURNS
    -------
    The Metrics Reporting Configuration ID assigned by the AF, which is needed
    to retrieve or delete this configuration later.
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

    if reporting_interval <= 0:
        return f"ERROR: reporting_interval must be a positive integer (received {reporting_interval})."
    if sampling_period is not None and sampling_period <= 0:
        return f"ERROR: sampling_period must be a positive integer (received {sampling_period})."
    if not (0.0 <= sample_percentage <= 100.0):
        return f"ERROR: sample_percentage must be between 0.0 and 100.0 (received {sample_percentage})."

    payload: dict = {
        "scheme": scheme,
        "reportingInterval": reporting_interval,
        "samplePercentage": sample_percentage,
    }
    if data_network_name is not None:
        payload["dataNetworkName"] = data_network_name
    if sampling_period is not None:
        payload["samplingPeriod"] = sampling_period
    if metrics:
        payload["metrics"] = metrics
    if url_filters is not None:
        payload["urlFilters"] = url_filters
    if slice_scope is not None:
        payload["sliceScope"] = slice_scope

    url = (
        f"{state.get_m1_url()}/3gpp-m1/v2/provisioning-sessions/"
        f"{session_id}/metrics-reporting-configurations"
    )

    try:
        async with httpx.AsyncClient(timeout=30.0) as client:
            response = await client.post(
                url, json=payload, headers={"Content-Type": "application/json"}
            )

        if response.status_code in (200, 201):
            # Extract the assigned configuration ID from Location header or body
            config_id = None
            location = response.headers.get("Location", "")
            if location:
                config_id = location.rstrip("/").split("/")[-1]
            if not config_id:
                try:
                    data = response.json()
                    config_id = data.get("metricsReportingConfigurationId")
                except Exception:
                    pass

            id_line = f"  Configuration ID    : {config_id}\n" if config_id else ""
            return (
                f"SUCCESS: Metrics Reporting Configuration created.\n\n"
                f"  Session ID          : {session_id}\n"
                f"{id_line}"
                f"  Scheme              : {scheme}\n"
                f"  Reporting Interval  : every {reporting_interval} second(s)\n"
                + (f"  Sampling Period     : every {sampling_period} second(s)\n" if sampling_period else "")
                + f"  Sample Percentage   : {sample_percentage}%\n"
                + (f"  Data Network Name   : {data_network_name}\n" if data_network_name else "")
                + (f"  Metrics             : {', '.join(metrics)}\n" if metrics else "")
                + (f"  URL Filters         : {', '.join(url_filters)}\n" if url_filters else "")
                + (f"  Slice Scope         : {slice_scope}\n" if slice_scope else "")
                + (
                    f"\nNOTE: Save the Configuration ID above — you will need it to retrieve "
                    f"or delete this configuration later using get_metrics_reporting_configuration "
                    f"or delete_metrics_reporting_configuration."
                    if config_id else ""
                )
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
# Tool — Get Metrics Reporting Configuration
# ---------------------------------------------------------------------------
@mcp.tool()
async def get_metrics_reporting_configuration(
    metrics_reporting_configuration_id: str,
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    Retrieve a Metrics Reporting Configuration from a Provisioning Session.

    PARAMETERS
    ----------
    metrics_reporting_configuration_id (REQUIRED)
        The ID of the metrics reporting configuration to retrieve.
        This ID is returned in the Location header when creating a metrics
        reporting configuration (create_metrics_reporting_configuration).

    provisioning_session_id (OPTIONAL)
        The provisioning session that owns this configuration.
        Defaults to the current session stored in state.

    m1_url (OPTIONAL if already provided)
        Base URL of the 5GMS Application Function M1 interface.
        Example: "http://192.168.1.100:7778"

    RETURNS
    -------
    Full JSON representation of the Metrics Reporting Configuration, including
    scheme, reporting interval, sample percentage, metrics URNs, and URL filters.
    """
    if m1_url:
        state.set_m1_url(m1_url)
    if not state.get_m1_url():
        return "ERROR: No M1 URL configured. Provide m1_url parameter."

    session_id = provisioning_session_id or state.get_session_id()
    if not session_id:
        return "ERROR: No provisioning_session_id provided or stored in state."

    url = _build_url(session_id, metrics_reporting_configuration_id)

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
                f"SUCCESS: Metrics Reporting Configuration '{metrics_reporting_configuration_id}'\n"
                f"  Session: {session_id}\n\n{pretty}"
            )
        elif response.status_code == 404:
            return (
                f"NOT FOUND: Metrics Reporting Configuration '{metrics_reporting_configuration_id}' "
                f"does not exist for session '{session_id}' (HTTP 404)."
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
# Tool — Delete Metrics Reporting Configuration
# ---------------------------------------------------------------------------
@mcp.tool()
async def delete_metrics_reporting_configuration(
    metrics_reporting_configuration_id: str,
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    Delete a Metrics Reporting Configuration from a Provisioning Session.

    Removes the specified metrics reporting configuration. The Provisioning Session
    and other configurations are NOT affected.

    PARAMETERS
    ----------
    metrics_reporting_configuration_id (REQUIRED)
        The ID of the metrics reporting configuration to delete.

    provisioning_session_id (OPTIONAL)
        The provisioning session that owns this configuration.
        Defaults to the current session stored in state.

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

    url = _build_url(session_id, metrics_reporting_configuration_id)

    try:
        async with httpx.AsyncClient(timeout=30.0) as client:
            response = await client.delete(url)

        if response.status_code in (200, 204):
            return (
                f"SUCCESS: Metrics Reporting Configuration '{metrics_reporting_configuration_id}' "
                f"deleted from session '{session_id}'."
            )
        elif response.status_code == 404:
            return (
                f"ERROR: Metrics Reporting Configuration '{metrics_reporting_configuration_id}' "
                f"not found for session '{session_id}' (HTTP 404)."
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
