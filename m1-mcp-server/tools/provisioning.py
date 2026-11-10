"""
Provisioning Session tools — Maf_Provisioning_ProvisioningSessions and Maf_Provisioning_ContentProtocols (TS 26.510 V19.2.0 clauses 5.2.2, 5.2.3)
Endpoints (base path /3gpp-maf-provisioning/v1):
  GET    /provisioning-sessions                                   enumerate
  POST   /provisioning-sessions                                   create (201)
  GET    /provisioning-sessions/{provisioningSessionId}           retrieve (200)
  DELETE /provisioning-sessions/{provisioningSessionId}           destroy (204)
  GET    /provisioning-sessions/{provisioningSessionId}/content-protocols
"""

from typing import Optional

from mcp_instance import mcp
import m1_http
import state
from . import _common


@mcp.tool()
async def create_provisioning_session(
    external_service_id: str,
    app_id: str,
    asp_id: str = None,
    location_reporting: bool = None,
    m1_url: str = None,
) -> str:
    """
    STEP 1 OF 3 — Create a 5GMS Provisioning Session (provisioningSessionType MS_DOWNLINK).

    A Provisioning Session is the umbrella for all media delivery provisioning (TS 26.510 clause 8.2).
    Request body: {provisioningSessionType: "MS_DOWNLINK", externalServiceId, appId, aspId?, locationReporting?}.

    PARAMETERS
    ----------
    external_service_id (REQUIRED)
        Identifier of the session for the Media Client, a reverse FQDN, different for every Provisioning Session.
        Example: "com.example.live-event-1"

    app_id (REQUIRED)
        Application Identifier, e.g. "live-stream-1".

    asp_id (OPTIONAL)
        Application Service Provider identifier, e.g. "myCompany".

    location_reporting (OPTIONAL)
        If true, the Media Session Handler has to populate UE location information in its reports and policy requests.
        Omitted from the request when not given.

    m1_url (OPTIONAL if already provided)
        Base URL of the Application Function at M1, http://<host>:<port>; remembered for the next tool calls.

    RETURNS
    -------
    The Provisioning Session identifier (stored for the next steps) and the entity tag of the resource.
    """
    err = _common.prepare(m1_url, need_session=False)
    if err:
        return err
    payload = {"provisioningSessionType": "MS_DOWNLINK", "externalServiceId": external_service_id, "appId": app_id}
    if asp_id:
        payload["aspId"] = asp_id
    if location_reporting is not None:
        payload["locationReporting"] = location_reporting
    new_id, text = await _common.create(
        "/provisioning-sessions", payload, "Provisioning Session", "provisioningSessionId", id_label="Session ID",
        extra=(f"  Service ID   : {external_service_id}\n  ASP ID       : {asp_id or '(none)'}\n  App ID       : {app_id}\n"
               f"  Type         : MS_DOWNLINK\n  M1 URL       : {state.get_m1_url()}\n\n"
               "NEXT STEP → Call create_content_hosting_configuration (Required: name, ingest_base_url, entry_point_relative_path)"))
    if new_id:
        state.set_session_id(new_id)
    return text


@mcp.tool()
async def get_provisioning_session(provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Retrieve an existing Provisioning Session (200): provisioningSessionId, provisioningSessionType, externalServiceId, appId, aspId,
    locationReporting and the read-only lists serverCertificateIds, contentPreparationTemplateIds, metricsReportingConfigurationIds,
    policyTemplateIds and eventDataProcessingConfigurationIds.

    PARAMETERS
    ----------
    provisioning_session_id (OPTIONAL)  The session to retrieve; the one in the current state when omitted.
    m1_url (OPTIONAL if already provided)  Base URL of the M1 interface.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.retrieve(m1_http.session_path(provisioning_session_id), f"Provisioning Session '{provisioning_session_id or state.get_session_id()}'")


@mcp.tool()
async def enumerate_provisioning_sessions(m1_url: str = None) -> str:
    """
    List the identifiers of the Provisioning Sessions that the Application Function lets this client manipulate
    (GET /3gpp-maf-provisioning/v1/provisioning-sessions, clause 5.2.2.2: a JSON array of identifiers, empty when there are none).

    PARAMETERS
    ----------
    m1_url (OPTIONAL if already provided)  Base URL of the M1 interface, http://<host>:<port>.
    """
    err = _common.prepare(m1_url, need_session=False)
    if err:
        return err
    out = await m1_http.call("GET", "/provisioning-sessions")
    if not out.ok:
        return out.error
    return f"SUCCESS: Provisioning Sessions\n\n{m1_http.etag_line(out.response)}{m1_http.pretty(out.response)}"


@mcp.tool()
async def delete_provisioning_session(provisioning_session_id: str = None, if_match: str = None, m1_url: str = None) -> str:
    """
    Destroy a Provisioning Session and every sub-resource it holds (DELETE, 204).

    WARNING: destructive. The Content Hosting Configuration, certificates, templates and reporting configurations go with it.

    PARAMETERS
    ----------
    provisioning_session_id (OPTIONAL)  The session to destroy; the one in the current state when omitted.
    if_match (OPTIONAL)  A strong entity tag obtained earlier (clause 7.1.4.4); 412 when the resource has changed since.
    m1_url (OPTIONAL if already provided)  Base URL of the M1 interface.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    sid = provisioning_session_id or state.get_session_id()
    text = await _common.destroy(m1_http.session_path(provisioning_session_id), f"Provisioning Session '{sid}'", if_match)
    if text.startswith("SUCCESS") and sid == state.get_session_id():
        state.clear_session()
    return text


@mcp.tool()
async def get_content_protocols(provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Retrieve the Content Protocols resource of a Provisioning Session (GET .../content-protocols, 200): downlinkIngestProtocols
    (termIdentifier, descriptionLocator?) and geoFencingLocatorTypes. The resource cannot be created, updated or destroyed (405).

    PARAMETERS
    ----------
    provisioning_session_id (OPTIONAL)  The session; the one in the current state when omitted.
    m1_url (OPTIONAL if already provided)  Base URL of the M1 interface.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.retrieve(m1_http.session_path(provisioning_session_id, "content-protocols"), "Content Protocols")
