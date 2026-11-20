"""
Policy Template tools — Maf_Provisioning_PolicyTemplates (TS 26.510 V19.2.0 clause 5.2.7)
Endpoints (base path /3gpp-maf-provisioning/v1/provisioning-sessions/{id}/policy-templates):
  POST (201) on the collection; GET (200), PUT (200, 204), PATCH (200, 204), DELETE (204) on /{policyTemplateId}.
  The AF assigns policyTemplateId and state (PENDING, INVALID, READY, SUSPENDED) and stateReason: they are read-only.
  Every update moves the template back to PENDING for revalidation.
"""

from mcp_instance import mcp
import m1_http
import models
from . import _common

PT = "policy-templates"


@mcp.tool()
async def create_policy_template(template: models.PolicyTemplate, provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Create a Policy Template (POST, 201). The AF answers with the template as it holds it: policyTemplateId and state PENDING.

    externalReference (unique in the session) is required, and at least one of qoSSpecifications (PolicyConstraints with a componentReference
    and a downlinkQoSSpecification or uplinkQoSSpecification, or a qosReference), chargingSpecification, bdtPolicyId and bdtSpecification.
    applicationSessionContexts restrict the template to network slices and Data Networks. 400 when a property is not acceptable.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return (await _common.create(m1_http.session_path(provisioning_session_id, PT), models.body(template), "Policy Template",
                                 "policyTemplateId", id_label="Template ID"))[1]


@mcp.tool()
async def get_policy_template(policy_template_id: str, provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Retrieve a Policy Template (200) with its state and stateReason. Only a template in the READY state can be instantiated as a
    Dynamic Policy.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.retrieve(m1_http.session_path(provisioning_session_id, PT, policy_template_id), f"Policy Template '{policy_template_id}'")


@mcp.tool()
async def replace_policy_template(policy_template_id: str, template: models.PolicyTemplate, if_match: str = None,
                                  provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Replace a Policy Template (PUT, 200 or 204 when nothing changes). state, stateReason and policyTemplateId cannot be sent (403).
    The template returns to PENDING. if_match is a strong entity tag obtained earlier (clause 7.1.4.4); 412 when the resource has changed.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.replace(m1_http.session_path(provisioning_session_id, PT, policy_template_id), models.body(template),
                                 "Policy Template", if_match)


@mcp.tool()
async def patch_policy_template(policy_template_id: str, patch: str, patch_type: _common.PatchType = "merge-patch", if_match: str = None,
                                provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Patch a Policy Template (PATCH, 200 or 204): a JSON merge patch (e.g. {"externalReference": "HD-2"}) or a JSON patch, per patch_type.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.patch(m1_http.session_path(provisioning_session_id, PT, policy_template_id), patch, patch_type, "Policy Template", if_match)


@mcp.tool()
async def delete_policy_template(policy_template_id: str, if_match: str = None, provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Destroy a Policy Template (DELETE, 204), in any state. Media delivery sessions that use it return to the default network QoS.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.destroy(m1_http.session_path(provisioning_session_id, PT, policy_template_id),
                                 f"Policy Template '{policy_template_id}'", if_match)
