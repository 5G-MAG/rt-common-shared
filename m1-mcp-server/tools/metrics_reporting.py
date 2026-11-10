"""
Metrics Reporting Configuration tools — Maf_Provisioning_MetricsReporting (TS 26.510 V19.2.0 clause 5.2.11)
Endpoints (base path /3gpp-maf-provisioning/v1/provisioning-sessions/{id}/metrics-reporting-configurations):
  POST (201) on the collection; GET (200), PUT (200, 204), PATCH (200, 204), DELETE (204) on /{metricsReportingConfigurationId}.
  Several configurations per session are possible.
"""

from mcp_instance import mcp
import m1_http
import models
from . import _common

MRC = "metrics-reporting-configurations"


@mcp.tool()
async def create_metrics_reporting_configuration(
    sampling_period: int,
    scheme: str = "urn:3GPP:ns:PSS:DASH:QM10",
    reporting_interval: int = 10,
    sample_percentage: float = 100.0,
    metrics: list[str] = None,
    url_filters: list[str] = None,
    data_network_name: str = None,
    slice_scope: list[models.Snssai] = None,
    reporting_start_offset: int = None,
    reporting_duration: int = None,
    positive_crossing_thresholds: dict[str, list[float]] = None,
    negative_crossing_thresholds: dict[str, list[float]] = None,
    location_filter: list[dict] = None,
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    Create a Metrics Reporting Configuration (POST, 201); the AF assigns metricsReportingConfigurationId.

    PARAMETERS
    ----------
    sampling_period (REQUIRED, seconds, > 0)  Interval between samplings of the metrics at the client.
    scheme (default "urn:3GPP:ns:PSS:DASH:QM10")  QoE metrics scheme URI.
    reporting_interval (default 10, seconds, > 0)  Interval between reports; one final report when omitted in the resource.
    sample_percentage (default 100.0)  Proportion of media delivery sessions that report.
    metrics  Metric term URIs, e.g. "urn:3GPP:ns:PSS:DASH:QM10#BufferLevel"; the default set of the scheme when omitted.
    url_filters  Media Entry Point URL patterns to report for.
    data_network_name  Data Network (DNN) over which reports are sent.
    slice_scope  Slices the configuration applies to: [{"sst": 1, "sd": "000001"}].
    reporting_start_offset / reporting_duration  Window of each media delivery session in which reports are sent (seconds, not negative).
    positive_crossing_thresholds / negative_crossing_thresholds  {metric URI: [thresholds]}: report when a value rises above or falls below.
    location_filter  LocationArea5G objects: collect only inside these locations.
    provisioning_session_id (OPTIONAL)  The session; the one in the current state when omitted.
    m1_url (OPTIONAL)  Base URL of the M1 interface; remembered.

    RETURNS
    -------
    The Metrics Reporting Configuration ID, needed to retrieve, replace, patch or delete it.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    if reporting_interval is not None and reporting_interval <= 0:
        return f"ERROR: reporting_interval must be a positive integer (received {reporting_interval})."
    if sampling_period <= 0:
        return f"ERROR: sampling_period must be a positive integer (received {sampling_period})."
    if sample_percentage is not None and not (0.0 <= sample_percentage <= 100.0):
        return f"ERROR: sample_percentage must be between 0.0 and 100.0 (received {sample_percentage})."
    config = models.MetricsReportingConfiguration(
        samplingPeriod=sampling_period, scheme=scheme, reportingInterval=reporting_interval, samplePercentage=sample_percentage,
        metrics=metrics or None, urlFilters=url_filters or None, dataNetworkName=data_network_name, sliceScope=slice_scope or None,
        reportingStartOffset=reporting_start_offset, reportingDuration=reporting_duration,
        positiveCrossingThresholds=positive_crossing_thresholds, negativeCrossingThresholds=negative_crossing_thresholds,
        locationFilter=location_filter)
    return (await _common.create(m1_http.session_path(provisioning_session_id, MRC), models.body(config), "Metrics Reporting Configuration",
                                 "metricsReportingConfigurationId", id_label="Config ID"))[1]


@mcp.tool()
async def get_metrics_reporting_configuration(metrics_reporting_configuration_id: str, provisioning_session_id: str = None,
                                              m1_url: str = None) -> str:
    """
    Retrieve a Metrics Reporting Configuration (200).

    PARAMETERS
    ----------
    metrics_reporting_configuration_id (REQUIRED)  The ID returned on creation (also listed in metricsReportingConfigurationIds of the session).
    provisioning_session_id (OPTIONAL)  The session; the one in the current state when omitted.
    m1_url (OPTIONAL if already provided)  Base URL of the M1 interface.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.retrieve(m1_http.session_path(provisioning_session_id, MRC, metrics_reporting_configuration_id),
                                  f"Metrics Reporting Configuration '{metrics_reporting_configuration_id}'")


@mcp.tool()
async def replace_metrics_reporting_configuration(
    metrics_reporting_configuration_id: str,
    configuration: models.MetricsReportingConfiguration,
    if_match: str = None,
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    Replace a Metrics Reporting Configuration (PUT, 200 or 204 when nothing changes). 400 for a scheme or metric that the AF does not support;
    if_match is a strong entity tag obtained earlier (clause 7.1.4.4), 412 when the resource has changed since.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.replace(m1_http.session_path(provisioning_session_id, MRC, metrics_reporting_configuration_id),
                                 models.body(configuration), "Metrics Reporting Configuration", if_match)


@mcp.tool()
async def patch_metrics_reporting_configuration(
    metrics_reporting_configuration_id: str,
    patch: str,
    patch_type: _common.PatchType = "merge-patch",
    if_match: str = None,
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    Patch a Metrics Reporting Configuration (PATCH, 200 or 204): a JSON merge patch (e.g. {"reportingInterval": 20}) or a JSON patch, per patch_type.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.patch(m1_http.session_path(provisioning_session_id, MRC, metrics_reporting_configuration_id), patch, patch_type,
                               "Metrics Reporting Configuration", if_match)


@mcp.tool()
async def delete_metrics_reporting_configuration(metrics_reporting_configuration_id: str, if_match: str = None,
                                                 provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Destroy a Metrics Reporting Configuration (DELETE, 204): the AF stops metrics reporting for it and discards pending reports.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.destroy(m1_http.session_path(provisioning_session_id, MRC, metrics_reporting_configuration_id),
                                 f"Metrics Reporting Configuration '{metrics_reporting_configuration_id}'", if_match)
