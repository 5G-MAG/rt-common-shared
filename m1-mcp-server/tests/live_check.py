#!/usr/bin/env python3
"""
Runs the tools of the server through the MCP tool-call path (argument validation against the tool schemas included) against a running AF
and checks the whole M1 life cycle: session, certificate, content preparation template, content hosting, policy template, consumption
and metrics reporting, event data processing, replace, patch with If-Match, and then deletes everything.

    python3 tests/live_check.py [http://host:port]        (default http://127.0.0.23:7777)

Exit status 0 when every step passed.
"""
import asyncio
import json
import logging
import re
import sys
import uuid
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

import server  # noqa: F401,E402  (registers the tools)
from mcp_instance import mcp  # noqa: E402

results = []
logging.getLogger("httpx").setLevel(logging.WARNING)


def check(name, ok, detail=""):
    results.append(bool(ok))
    print(("PASS  " if ok else "FAIL  ") + name + ("" if ok else "\n      " + detail.replace("\n", "\n      ")))
    return ok


async def tool(tool_name, /, **args):
    out = await mcp.call_tool(tool_name, args)
    content = out[0] if isinstance(out, tuple) else out
    return "".join(getattr(c, "text", "") for c in content)


def find(pattern, text):
    m = re.search(pattern, text)
    return m.group(1) if m else None


async def main(url):
    suffix = uuid.uuid4().hex[:8]
    sid = None
    try:
        t = await tool("create_provisioning_session", external_service_id=f"com.example.mcp{suffix}", app_id="mcp-app", asp_id="mcp-asp",
                       location_reporting=True, m1_url=url)
        sid = find(r"Session ID\s*: (\S+)", t)
        check("create_provisioning_session", t.startswith("SUCCESS") and sid, t)
        t = await tool("get_provisioning_session")
        check("get_provisioning_session has MS_DOWNLINK and locationReporting", '"MS_DOWNLINK"' in t and '"locationReporting": true' in t, t)
        t = await tool("enumerate_provisioning_sessions")
        check("enumerate_provisioning_sessions lists it (M1, not the management API)", sid in t, t)
        t = await tool("get_content_protocols")
        check("get_content_protocols", "downlinkIngestProtocols" in t and "uplinkEgestProtocols" not in t, t)

        t = await tool("create_server_certificate")
        cert = find(r"Certificate ID: (\S+)", t)
        check("create_server_certificate", t.startswith("SUCCESS") and cert, t)
        t = await tool("reserve_server_certificate", domain_name_aliases=["www.example.org"])
        reserved = find(r"Certificate ID: (\S+)", t)
        check("reserve_server_certificate returns a CSR", reserved and "BEGIN CERTIFICATE REQUEST" in t, t)
        t = await tool("delete_server_certificate", certificate_id=reserved)
        check("delete_server_certificate (reserved, never uploaded)", t.startswith("SUCCESS"), t)

        t = await tool("create_content_preparation_template", template=json.dumps({"version": 1}))
        cpt = find(r"Template ID\s*: (\S+)", t)
        check("create_content_preparation_template", t.startswith("SUCCESS") and cpt, t)
        t = await tool("create_content_preparation_template", template="<x/>", content_type="application/xml")
        check("an unknown template type is refused with 415 and a ProblemDetails", "HTTP 415" in t and "title:" in t, t)

        t = await tool("create_content_hosting_configuration", name="mcp", ingest_base_url="http://origin.example.org/media",
                       entry_point_relative_path="manifest.mpd", certificate_id=cert)
        check("create_content_hosting_configuration", t.startswith("SUCCESS"), t)
        t = await tool("get_content_hosting_configuration")
        etag = find(r'ETag\s*: ("[^"]+")', t)
        check("get_content_hosting_configuration has the entity tag and the AF's baseURL", etag and "canonicalDomainName" in t and '"baseURL"' in t, t)
        t = await tool("patch_content_hosting_configuration", patch=json.dumps({"name": "mcp patched"}), if_match=etag)
        check("patch_content_hosting_configuration (merge patch, If-Match)", t.startswith("SUCCESS"), t)
        t = await tool("patch_content_hosting_configuration", patch=json.dumps({"name": "again"}), if_match=etag)
        check("a stale If-Match is answered 412 and explained", "HTTP 412" in t and "If-Match" in t, t)
        t = await tool("get_content_hosting_configuration")
        held = json.loads(t[t.index("{"):])
        for d in held["distributionConfigurations"]:
            d.pop("canonicalDomainName", None)
            d.pop("baseURL", None)
        held["name"] = "mcp replaced"
        t = await tool("replace_content_hosting_configuration", configuration=held)
        check("replace_content_hosting_configuration", t.startswith("SUCCESS"), t)
        t = await tool("delete_server_certificate", certificate_id=cert)
        check("a certificate in use is refused with 409", "HTTP 409" in t, t)
        t = await tool("purge_content_hosting_cache", pattern=".*")
        check("purge_content_hosting_cache", t.startswith("SUCCESS"), t)

        t = await tool("create_policy_template", template={
            "externalReference": f"mcp-{suffix}", "applicationSessionContexts": [{"dnn": "internet"}],
            "qoSSpecifications": [{"componentReference": "video", "qosReference": "q1", "downlinkQoSSpecification": {"maximumAuthorisedBitRate": "20 Mbps"}}],
            "chargingSpecification": {"sponsorId": "s1", "sponsoringStatus": "SPONSOR_ENABLED"}})
        pt = find(r"Template ID\s*: (\S+)", t)
        check("create_policy_template", t.startswith("SUCCESS") and pt, t)
        t = await tool("get_policy_template", policy_template_id=pt)
        check("get_policy_template has a state", '"state"' in t, t)
        t = await tool("replace_policy_template", policy_template_id=pt, template={
            "externalReference": f"mcp-{suffix}-2", "chargingSpecification": {"sponsorId": "s2"}})
        check("replace_policy_template", t.startswith("SUCCESS"), t)
        t = await tool("patch_policy_template", policy_template_id=pt, patch=json.dumps([{"op": "replace", "path": "/externalReference", "value": f"mcp-{suffix}-3"}]),
                       patch_type="json-patch")
        check("patch_policy_template (json patch)", t.startswith("SUCCESS"), t)

        t = await tool("create_consumption_reporting_configuration", reporting_interval=10, sample_percentage=100.0, access_reporting=True)
        check("create_consumption_reporting_configuration", t.startswith("SUCCESS"), t)
        t = await tool("replace_consumption_reporting_configuration", configuration={"reportingInterval": 20})
        check("replace_consumption_reporting_configuration", t.startswith("SUCCESS"), t)
        t = await tool("patch_consumption_reporting_configuration", patch=json.dumps({"reportingInterval": 30}))
        check("patch_consumption_reporting_configuration", t.startswith("SUCCESS"), t)
        t = await tool("get_consumption_reporting_configuration")
        check("get_consumption_reporting_configuration", '"reportingInterval": 30' in t, t)

        t = await tool("create_metrics_reporting_configuration", sampling_period=2, reporting_start_offset=1, reporting_duration=30,
                       metrics=["urn:3GPP:ns:PSS:DASH:QM10#BufferLevel"], positive_crossing_thresholds={"urn:3GPP:ns:PSS:DASH:QM10#BufferLevel": [1.5]})
        mrc = find(r"Config ID\s*: (\S+)", t)
        check("create_metrics_reporting_configuration (Release 19 properties)", t.startswith("SUCCESS") and mrc, t)
        t = await tool("replace_metrics_reporting_configuration", metrics_reporting_configuration_id=mrc,
                       configuration={"samplingPeriod": 4, "scheme": "urn:3GPP:ns:PSS:DASH:QM10", "reportingInterval": 10})
        check("replace_metrics_reporting_configuration", t.startswith("SUCCESS"), t)
        t = await tool("patch_metrics_reporting_configuration", metrics_reporting_configuration_id=mrc, patch=json.dumps({"reportingInterval": 20}))
        check("patch_metrics_reporting_configuration", t.startswith("SUCCESS"), t)
        t = await tool("get_metrics_reporting_configuration", metrics_reporting_configuration_id=mrc)
        check("get_metrics_reporting_configuration", '"reportingInterval": 20' in t, t)

        edp = {"eventId": "MS_QOE_METRICS", "dataAccessProfiles": [{"dataAccessProfileId": "nwdaf", "targetEventConsumerTypes": ["NWDAF"],
                                                                      "parameters": ["urn:3GPP:ns:PSS:DASH:QM10#AvgThroughput"]}]}
        t = await tool("create_event_data_processing_configuration", configuration=edp)
        edp_id = find(r"Config ID\s*: (\S+)", t)
        check("create_event_data_processing_configuration", t.startswith("SUCCESS") and edp_id, t)
        t = await tool("replace_event_data_processing_configuration", event_data_processing_configuration_id=edp_id,
                       configuration=dict(edp, authorizationUrl="https://consumer.example.org/authorize"))
        check("replace_event_data_processing_configuration", t.startswith("SUCCESS"), t)
        t = await tool("patch_event_data_processing_configuration", event_data_processing_configuration_id=edp_id,
                       patch=json.dumps({"authorizationUrl": "https://consumer.example.org/other"}))
        check("patch_event_data_processing_configuration", t.startswith("SUCCESS"), t)
        t = await tool("get_event_data_processing_configuration", event_data_processing_configuration_id=edp_id)
        check("get_event_data_processing_configuration", "consumer.example.org/other" in t, t)

        t = await tool("get_provisioning_session")
        check("the session lists the ids of its sub-resources", all(i in t for i in (cert, cpt, pt, mrc, edp_id)), t)

        for name, args in (("delete_event_data_processing_configuration", {"event_data_processing_configuration_id": edp_id}),
                           ("delete_metrics_reporting_configuration", {"metrics_reporting_configuration_id": mrc}),
                           ("delete_consumption_reporting_configuration", {}),
                           ("delete_policy_template", {"policy_template_id": pt}),
                           ("delete_content_hosting_configuration", {}),
                           ("delete_content_preparation_template", {"content_preparation_template_id": cpt}),
                           ("delete_server_certificate", {"certificate_id": cert})):
            t = await tool(name, **args)
            check(name, t.startswith("SUCCESS"), t)
        t = await tool("get_policy_template", policy_template_id=pt)
        check("a destroyed Policy Template is gone", "HTTP 404" in t or "HTTP 410" in t, t)
        t = await tool("delete_provisioning_session")
        check("delete_provisioning_session", t.startswith("SUCCESS"), t)
        sid_deleted, sid = sid, None
        t = await tool("enumerate_provisioning_sessions")
        check("the collection no longer lists it", sid_deleted not in t, t)
    finally:
        if sid:
            print("cleanup:", (await tool("delete_provisioning_session", provisioning_session_id=sid)).splitlines()[0])


if __name__ == "__main__":
    asyncio.run(main(sys.argv[1] if len(sys.argv) > 1 else "http://127.0.0.23:7777"))
    print(f"{sum(results)} of {len(results)} steps passed")
    sys.exit(0 if all(results) else 1)
