"""
Consumption Reporting Configuration tools — Maf_Provisioning_ConsumptionReporting (TS 26.510 V19.2.0 clause 5.2.12)
Endpoints (base path /3gpp-maf-provisioning/v1/provisioning-sessions/{id}/consumption-reporting-configuration):
  POST (201), GET (200), PUT (200, 204), PATCH (200, 204), DELETE (204). At most one configuration per session.
"""

from mcp_instance import mcp
import m1_http
import models
from . import _common

CRC = "consumption-reporting-configuration"


@mcp.tool()
async def create_consumption_reporting_configuration(
    reporting_interval: int = 10,
    sample_percentage: float = 100.0,
    access_reporting: bool = True,
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    STEP 3 OF 3 — Activate Consumption Reporting (Consumption Reporting Configuration, 201).

    Body: {reportingInterval (> 0, seconds), samplePercentage (0.0 to 100.0), accessReporting}. All parameters have defaults.
    Whether the Media Session Handler reports the UE location is the locationReporting property of the Provisioning Session,
    not of this configuration.

    PARAMETERS
    ----------
    reporting_interval (default 10, seconds)  Interval between two consecutive consumption reports.
    sample_percentage (default 100.0)  Proportion of clients that report.
    access_reporting (default true)  Report access network changes during a session.
    provisioning_session_id (OPTIONAL)  The session; the one in the current state when omitted.
    m1_url (OPTIONAL)  Base URL of the M1 interface; remembered.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    if reporting_interval <= 0:
        return f"ERROR: reporting_interval must be a positive integer (received {reporting_interval})."
    if not (0.0 <= sample_percentage <= 100.0):
        return f"ERROR: sample_percentage must be between 0.0 and 100.0 (received {sample_percentage})."
    payload = {"reportingInterval": reporting_interval, "samplePercentage": sample_percentage, "accessReporting": access_reporting}
    return (await _common.create(m1_http.session_path(provisioning_session_id, CRC), payload, "Consumption Reporting Configuration",
                                 id_label="Resource"))[1]


@mcp.tool()
async def get_consumption_reporting_configuration(provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Retrieve the Consumption Reporting Configuration of a Provisioning Session (200); 404 when there is none.

    PARAMETERS
    ----------
    provisioning_session_id (OPTIONAL)  The session; the one in the current state when omitted.
    m1_url (OPTIONAL if already provided)  Base URL of the M1 interface.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.retrieve(m1_http.session_path(provisioning_session_id, CRC), "Consumption Reporting Configuration")


@mcp.tool()
async def replace_consumption_reporting_configuration(
    configuration: models.ConsumptionReportingConfiguration,
    if_match: str = None,
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    Replace the Consumption Reporting Configuration (PUT, 200 or 204 when nothing changes).

    if_match is a strong entity tag obtained earlier (clause 7.1.4.4); 412 when the resource has changed since.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.replace(m1_http.session_path(provisioning_session_id, CRC), models.body(configuration),
                                 "Consumption Reporting Configuration", if_match)


@mcp.tool()
async def patch_consumption_reporting_configuration(
    patch: str,
    patch_type: _common.PatchType = "merge-patch",
    if_match: str = None,
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    Patch the Consumption Reporting Configuration (PATCH, 200 or 204). patch is a JSON merge patch (e.g. {"reportingInterval": 20}) or a
    JSON patch (a list of operations), as selected by patch_type.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.patch(m1_http.session_path(provisioning_session_id, CRC), patch, patch_type,
                               "Consumption Reporting Configuration", if_match)


@mcp.tool()
async def delete_consumption_reporting_configuration(if_match: str = None, provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Destroy the Consumption Reporting Configuration (DELETE, 204): consumption reporting stops for the session.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.destroy(m1_http.session_path(provisioning_session_id, CRC), "Consumption Reporting Configuration", if_match)
