"""
Content Preparation Template tools — Maf_Provisioning_ContentPreparationTemplates (TS 26.510 V19.2.0 clause 5.2.5)
Endpoints (base path /3gpp-maf-provisioning/v1/provisioning-sessions/{id}/content-preparation-templates):
  POST (201, Location) on the collection; GET (200, the Content-Type of the creation), PUT (200, 204), PATCH (204), DELETE (204; 409 while a
  distribution configuration refers to the template) on /{contentPreparationTemplateId}.
  The template has any media type that the Media AF understands (415 otherwise).
"""

from mcp_instance import mcp
import m1_http
from . import _common

CPT = "content-preparation-templates"
DEFAULT_TYPE = "application/vnd.cmmf-encoder-configuration+json"


@mcp.tool()
async def create_content_preparation_template(template: str, content_type: str = DEFAULT_TYPE, provisioning_session_id: str = None,
                                              m1_url: str = None) -> str:
    """
    Create a Content Preparation Template (POST, 201): processing that the Media AS applies to the content it ingests before distributing it.

    The template is sent as it is, with content_type as its Content-Type (415 when the AF does not understand the type). Reference the
    returned ID as contentPreparationTemplateId in a distribution configuration.

    PARAMETERS
    ----------
    template (REQUIRED)  The template text.
    content_type  Media type of the template, default "application/vnd.cmmf-encoder-configuration+json".
    provisioning_session_id (OPTIONAL)  The session; the one in the current state when omitted.
    m1_url (OPTIONAL if already provided)  Base URL of the M1 interface.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return (await _common.create(m1_http.session_path(provisioning_session_id, CPT), template, "Content Preparation Template",
                                 content_type=content_type, accept="*/*", id_label="Template ID"))[1]


@mcp.tool()
async def get_content_preparation_template(content_preparation_template_id: str, provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Retrieve a Content Preparation Template (200); the Content-Type of the answer is the one it was created with.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    out = await m1_http.call("GET", m1_http.session_path(provisioning_session_id, CPT, content_preparation_template_id), accept="*/*")
    if not out.ok:
        return out.error
    return (f"SUCCESS: Content Preparation Template '{content_preparation_template_id}'\n\n  Content-Type : "
            f"{out.response.headers.get('Content-Type')}\n{m1_http.etag_line(out.response)}\n{out.response.text}")


@mcp.tool()
async def replace_content_preparation_template(content_preparation_template_id: str, template: str, content_type: str = DEFAULT_TYPE,
                                               if_match: str = None, provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Replace a Content Preparation Template (PUT, 200 or 204 when nothing changes; 403 when the AF does not allow modification).
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.replace(m1_http.session_path(provisioning_session_id, CPT, content_preparation_template_id), template,
                                 "Content Preparation Template", if_match, content_type=content_type, accept="*/*")


@mcp.tool()
async def patch_content_preparation_template(content_preparation_template_id: str, patch: str, patch_type: _common.PatchType = "merge-patch",
                                             if_match: str = None, provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Patch a Content Preparation Template (PATCH, 204): a JSON merge patch or a JSON patch, per patch_type.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.patch(m1_http.session_path(provisioning_session_id, CPT, content_preparation_template_id), patch, patch_type,
                               "Content Preparation Template", if_match)


@mcp.tool()
async def delete_content_preparation_template(content_preparation_template_id: str, if_match: str = None, provisioning_session_id: str = None,
                                              m1_url: str = None) -> str:
    """
    Destroy a Content Preparation Template (DELETE, 204); 409 while a distribution configuration still refers to it.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.destroy(m1_http.session_path(provisioning_session_id, CPT, content_preparation_template_id),
                                 f"Content Preparation Template '{content_preparation_template_id}'", if_match)
