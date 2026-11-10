"""
Content Hosting Configuration tools — Maf_Provisioning_ContentHosting (TS 26.510 V19.2.0 clause 5.2.8)
Endpoints (base path /3gpp-maf-provisioning/v1/provisioning-sessions/{id}):
  POST   /content-hosting-configuration         create (201; at most one per session; 403 unless the session is MS_DOWNLINK)
  GET    /content-hosting-configuration         retrieve (200)
  PUT    /content-hosting-configuration         replace (200, 204 when unchanged)
  PATCH  /content-hosting-configuration         merge-patch+json or json-patch+json (200, 204)
  DELETE /content-hosting-configuration         destroy (204; the text of clause 5.2.8.5 says 200)
  POST   /content-hosting-configuration/purge   application/x-www-form-urlencoded pattern=<regex> (200 count, 204 nothing purged)
"""

from pathlib import Path
from typing import Optional
import json
import urllib.parse

from mcp_instance import mcp
import m1_http
import models
import state
from . import _common

TEMPLATE_PATH = Path(__file__).parent.parent / "content_hosting_config_template.json"
CHC = "content-hosting-configuration"


@mcp.tool()
async def create_content_hosting_configuration(
    name: str,
    ingest_base_url: str,
    entry_point_relative_path: str,
    entry_point_content_type: str = "application/dash+xml",
    domain_name_alias: str = None,
    certificate_id: str = None,
    dash_profiles: list[str] = None,
    ingest_pull: bool = True,
    ingest_protocol: str = "urn:3gpp:5gms:content-protocol:http-pull",
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    STEP 2 OF 3 — Create a Content Hosting Configuration with one distribution (distributionId "d1").

    For several distributions, caching, geo-fencing, URL signing or a PUSH distribution use
    create_content_hosting_configuration_from_schema. The Media AF assigns baseURL and canonicalDomainName of a PULL distribution.

    PARAMETERS
    ----------
    name (REQUIRED)  Label of the configuration.
    ingest_base_url (REQUIRED)  Origin base URL that the Media AS pulls from (ingestConfiguration.baseURL), e.g. "https://rdmedia.bbc.co.uk/".
    entry_point_relative_path (REQUIRED)  Path of the manifest relative to ingest_base_url, e.g. "elephants_dream/1/client_manifest-all.mpd".
    entry_point_content_type  "application/dash+xml" (DASH) or "application/vnd.apple.mpegurl" (HLS).
    domain_name_alias  Alternative FQDN advertised to the client; needs certificate_id whose certificate lists it.
    certificate_id  A Server Certificate of the session for HTTPS distribution.
    dash_profiles  Profile URIs of the entry point (default ["urn:mpeg:dash:profile:isoff-live:2011"]).
    ingest_pull  true: PULL ingest (default); false: PUSH ingest (then ingest_protocol has to be a DASH-IF term).
    ingest_protocol  Content protocol term of TS 26.512 table 8.1-1 (default http-pull).
    provisioning_session_id  The session; the one in the current state when omitted.
    m1_url  Base URL of the M1 interface; remembered.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    try:
        config = json.loads(TEMPLATE_PATH.read_text(encoding="utf-8"))
    except FileNotFoundError:
        config = {"name": "", "ingestConfiguration": {"mode": "PULL", "protocol": ingest_protocol}, "distributionConfigurations": []}
    config["name"] = name
    ingest = {"mode": "PULL" if ingest_pull else "PUSH", "protocol": ingest_protocol}
    if ingest_pull:  # the base URL of a PUSH ingest is assigned by the Media AF
        ingest["baseURL"] = ingest_base_url if ingest_base_url.endswith("/") else ingest_base_url + "/"
    config["ingestConfiguration"] = ingest
    dist = {"distributionId": "d1",
            "entryPoint": {"relativePath": entry_point_relative_path.lstrip("/"), "contentType": entry_point_content_type,
                           "profiles": dash_profiles or ["urn:mpeg:dash:profile:isoff-live:2011"]}}
    if certificate_id:
        dist["certificateId"] = certificate_id
    if domain_name_alias:
        dist["domainNameAlias"] = domain_name_alias
    config["distributionConfigurations"] = [dist]
    return (await _common.create(m1_http.session_path(provisioning_session_id, CHC), config, "Content Hosting Configuration",
                                 extra=f"  Name         : {name}\n  Ingest       : {ingest['mode']} {ingest.get('baseURL', '(assigned by the AF)')}\n"
                                       "\nNEXT STEP → Call create_consumption_reporting_configuration", id_label="Resource"))[1]


@mcp.tool()
async def create_content_hosting_configuration_from_schema(
    configuration: models.ContentHostingConfiguration,
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    Create a Content Hosting Configuration from the full Release 19 representation (TS 26.510 table 8.8.3.1-1).

    name, ingestConfiguration {mode, protocol, baseURL?} and distributionConfigurations (at least one, each with a distributionId) are
    required. For a PULL distribution the AF assigns baseURL and canonicalDomainName: omit them. 400 for an invalid reference
    (certificateId, contentPreparationTemplateId) or a domainNameAlias without a matching certificate.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return (await _common.create(m1_http.session_path(provisioning_session_id, CHC), models.body(configuration),
                                 "Content Hosting Configuration", id_label="Resource"))[1]


@mcp.tool()
async def get_content_hosting_configuration(provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Retrieve the Content Hosting Configuration of a Provisioning Session (200), including what the AF assigned
    (distribution baseURL, canonicalDomainName, push ingest baseURL). The ETag is shown for use as if_match.

    PARAMETERS
    ----------
    provisioning_session_id (OPTIONAL)  The session; the one in the current state when omitted.
    m1_url (OPTIONAL if already provided)  Base URL of the M1 interface.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.retrieve(m1_http.session_path(provisioning_session_id, CHC), "Content Hosting Configuration")


@mcp.tool()
async def replace_content_hosting_configuration(
    configuration: models.ContentHostingConfiguration,
    if_match: str = None,
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    Replace the Content Hosting Configuration (PUT, 200, or 204 when nothing changes).

    All writable properties except domainNameAlias can change; 403 for an attempt to change a read-only property such as
    canonicalDomainName. if_match is a strong entity tag obtained earlier (clause 7.1.4.4); 412 when the resource has changed since.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.replace(m1_http.session_path(provisioning_session_id, CHC), models.body(configuration),
                                 "Content Hosting Configuration", if_match)


@mcp.tool()
async def patch_content_hosting_configuration(
    patch: str,
    patch_type: _common.PatchType = "merge-patch",
    if_match: str = None,
    provisioning_session_id: str = None,
    m1_url: str = None,
) -> str:
    """
    Patch the Content Hosting Configuration (PATCH, 200 or 204).

    patch is a JSON text: a JSON merge patch (RFC 7396, application/merge-patch+json, e.g. {"name": "New name"}) or a JSON patch
    (RFC 6902, application/json-patch+json, e.g. [{"op": "replace", "path": "/name", "value": "New name"}]).
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.patch(m1_http.session_path(provisioning_session_id, CHC), patch, patch_type, "Content Hosting Configuration", if_match)


@mcp.tool()
async def delete_content_hosting_configuration(if_match: str = None, provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Destroy the Content Hosting Configuration (DELETE; 204, or 200 with an empty body as clause 5.2.8.5 says). The Media AF releases the
    network resources, purges the cache and ends the distribution.

    PARAMETERS
    ----------
    if_match (OPTIONAL)  A strong entity tag obtained earlier (clause 7.1.4.4).
    provisioning_session_id (OPTIONAL)  The session; the one in the current state when omitted.
    m1_url (OPTIONAL if already provided)  Base URL of the M1 interface.
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    return await _common.destroy(m1_http.session_path(provisioning_session_id, CHC), "Content Hosting Configuration", if_match, expect=(200, 204))


@mcp.tool()
async def purge_content_hosting_cache(pattern: str = ".*", provisioning_session_id: str = None, m1_url: str = None) -> str:
    """
    Purge media resources from the Media AS cache (POST .../content-hosting-configuration/purge).

    The body is application/x-www-form-urlencoded with the key "pattern" and a regular expression for the media resource URLs.
    200 with the number of purged entries, 204 when none matched, 400 for a malformed expression.

    PARAMETERS
    ----------
    pattern  Regular expression (default ".*": everything).
    """
    err = _common.prepare(m1_url, provisioning_session_id)
    if err:
        return err
    out = await m1_http.call("POST", m1_http.session_path(provisioning_session_id, CHC, "purge"),
                             body=urllib.parse.urlencode({"pattern": pattern}), content_type="application/x-www-form-urlencoded",
                             accept="text/plain, application/json, */*", expect=(200, 204))
    if not out.ok:
        return out.error
    if out.response.status_code == 204:
        return "SUCCESS: no cache entry matched the pattern (HTTP 204)."
    return f"SUCCESS: {out.response.text.strip()} cache entries purged."
