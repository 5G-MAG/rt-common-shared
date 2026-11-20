"""
Event Data Processing Configuration tools — Maf_Provisioning_EventDataProcessing (TS 26.510 V19.2.0 clause 5.2.13)
Endpoints (base path /3gpp-maf-provisioning/v1/provisioning-sessions/{id}/event-data-processing-configurations):
  POST (201; 400 for an event or Data Access Profile that is not acceptable; 404 without a Data Collection AF) on the collection;
  GET (200), PUT (200, 204), PATCH (200, 204), DELETE (204) on /{eventDataProcessingConfigurationId}.
"""

from mcp_instance import mcp
import m1_http
import models
from . import _common

EDP = "event-data-processing-configurations"


@mcp.tool()
async def create_event_data_processing_configuration(configuration: models.EventDataProcessingConfiguration,
                                                     provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Create an Event Data Processing Configuration (POST, 201): the data processing that the Data Collection AF applies to the UE data of an
    event before exposing it to an event consumer (NWDAF, Event Consumer AF, NEF).

    eventId is an AfEvent for media delivery (MS_QOE_METRICS, MS_CONSUMPTION, MS_NET_ASSIST_INVOCATION, MS_DYN_POLICY_INVOCATION,
    MS_ACCESS_ACTIVITY); dataAccessProfiles (at least one) each have a dataAccessProfileId (unique in the configuration),
    targetEventConsumerTypes and parameters, and optionally time, user and location access restrictions.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return (await _common.create(m1_http.session_path(provisioning_session_id, EDP), models.body(configuration),
                                 "Event Data Processing Configuration", "eventDataProcessingConfigurationId", id_label="Config ID"))[1]


@mcp.tool()
async def get_event_data_processing_configuration(event_data_processing_configuration_id: str, provisioning_session_id: str = None,
                                                  m1_url: str = None) -> str:
    """Retrieve an Event Data Processing Configuration (200)."""
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.retrieve(m1_http.session_path(provisioning_session_id, EDP, event_data_processing_configuration_id),
                                  f"Event Data Processing Configuration '{event_data_processing_configuration_id}'")


@mcp.tool()
async def replace_event_data_processing_configuration(event_data_processing_configuration_id: str,
                                                      configuration: models.EventDataProcessingConfiguration, if_match: str = None,
                                                      provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Replace an Event Data Processing Configuration (PUT, 200 or 204 when nothing changes). if_match is a strong entity tag obtained
    earlier (clause 7.1.4.4); 412 when the resource has changed since.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.replace(m1_http.session_path(provisioning_session_id, EDP, event_data_processing_configuration_id),
                                 models.body(configuration), "Event Data Processing Configuration", if_match)


@mcp.tool()
async def patch_event_data_processing_configuration(event_data_processing_configuration_id: str, patch: str,
                                                    patch_type: _common.PatchType = "merge-patch", if_match: str = None,
                                                    provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Patch an Event Data Processing Configuration (PATCH, 200 or 204): a JSON merge patch (e.g. {"authorizationUrl": "https://..."}) or a
    JSON patch, per patch_type.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.patch(m1_http.session_path(provisioning_session_id, EDP, event_data_processing_configuration_id), patch, patch_type,
                               "Event Data Processing Configuration", if_match)


@mcp.tool()
async def delete_event_data_processing_configuration(event_data_processing_configuration_id: str, if_match: str = None,
                                                     provisioning_session_id: str = None, m1_url: str = None) -> str:
    """Destroy an Event Data Processing Configuration (DELETE, 204): the data collection and the event subscriptions for it end."""
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.destroy(m1_http.session_path(provisioning_session_id, EDP, event_data_processing_configuration_id),
                                 f"Event Data Processing Configuration '{event_data_processing_configuration_id}'", if_match)
