"""Unit tests of the tools against a local stand-in for the AF: paths, methods, headers, bodies and the reading of the answers."""
import asyncio
import json
import logging
import sys
import threading
from http.server import BaseHTTPRequestHandler, HTTPServer
from pathlib import Path

import pytest
from pydantic import ValidationError

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

import server  # noqa: F401,E402
import state  # noqa: E402
from mcp_instance import mcp  # noqa: E402
import models  # noqa: E402

logging.getLogger("httpx").setLevel(logging.WARNING)
PS = "3360f208-d93b-41f1-ba49-f19090f84b23"


class FakeAF(BaseHTTPRequestHandler):
    requests = []
    answers = {}

    def _handle(self):
        length = int(self.headers.get("Content-Length") or 0)
        body = self.rfile.read(length) if length else b""
        FakeAF.requests.append({"method": self.command, "path": self.path, "headers": dict(self.headers), "body": body})
        status, headers, payload = FakeAF.answers.get((self.command, self.path.split("?")[0]),
                                                       (405, {"Allow": "GET", "Content-Type": "application/problem+json"},
                                                        b'{"title":"Method Not Allowed","status":405}'))
        self.send_response(status)
        for k, v in headers.items():
            self.send_header(k, v)
        self.send_header("Content-Length", str(len(payload)))
        self.end_headers()
        self.wfile.write(payload)

    do_GET = do_POST = do_PUT = do_PATCH = do_DELETE = do_OPTIONS = do_HEAD = _handle

    def log_message(self, *args):
        pass


@pytest.fixture
def af():
    httpd = HTTPServer(("127.0.0.1", 0), FakeAF)
    threading.Thread(target=httpd.serve_forever, daemon=True).start()
    FakeAF.requests = []
    FakeAF.answers = {}
    state._state.update({"m1_url": None, "provisioning_session_id": None})
    yield f"http://127.0.0.1:{httpd.server_address[1]}"
    httpd.shutdown()


def answer(method, path, status, body=b"", headers=None):
    FakeAF.answers[(method, "/3gpp-maf-provisioning/v1" + path)] = (status, headers or {}, body)


def call(tool, **args):
    out = asyncio.run(mcp.call_tool(tool, args))
    content = out[0] if isinstance(out, tuple) else out
    return "".join(getattr(c, "text", "") for c in content)


def test_the_tools_are_those_of_the_maf_provisioning_api():
    names = {t.name for t in asyncio.run(mcp.list_tools())}
    for resource in ("content_hosting_configuration", "content_preparation_template", "policy_template", "consumption_reporting_configuration",
                     "metrics_reporting_configuration", "event_data_processing_configuration"):
        for verb in ("get", "replace", "patch", "delete"):
            assert f"{verb}_{resource}" in names
    for tool in ("create_server_certificate", "reserve_server_certificate", "upload_server_certificate", "get_server_certificate",
                 "delete_server_certificate", "get_content_protocols", "enumerate_provisioning_sessions", "purge_content_hosting_cache"):
        assert tool in names
    for excluded in ("edge", "publishing", "rtc", "embms", "mbs", "remote", "exposure"):
        assert not any(excluded in n for n in names), excluded


def test_a_session_is_always_downlink_and_has_no_type_parameter():
    tool = next(t for t in asyncio.run(mcp.list_tools()) if t.name == "create_provisioning_session")
    assert "provisioning_session_type" not in tool.inputSchema["properties"]


def test_the_bodies_are_the_release_19_schemas():
    tool = next(t for t in asyncio.run(mcp.list_tools()) if t.name == "create_content_hosting_configuration_from_schema")
    defs = tool.inputSchema["$defs"]
    assert set(defs["ContentHostingConfiguration"]["required"]) == {"name", "ingestConfiguration", "distributionConfigurations"}
    assert defs["IngestConfiguration"]["properties"]["mode"]["enum"] == ["PULL", "PUSH"]
    assert "distributionId" in defs["DistributionConfiguration"]["required"]
    for gone in ("canonicalDomainName", "edgeResourcesConfigurationId", "supplementaryDistributionNetworks"):
        assert gone not in defs["DistributionConfiguration"]["properties"]
    with pytest.raises(ValidationError):
        models.DistributionConfiguration(distributionId="d1", supplementaryDistributionNetworks=[])
    with pytest.raises(ValidationError):
        models.MetricsReportingConfiguration(samplingPeriod=0)
    with pytest.raises(ValidationError):
        models.ConsumptionReportingConfiguration(reportingInterval=0)


def test_enumerate_uses_the_m1_collection(af):
    answer("GET", "/provisioning-sessions", 200, b'["a"]', {"Content-Type": "application/json", "ETag": '"e"'})
    text = call("enumerate_provisioning_sessions", m1_url=af)
    assert '"a"' in text
    assert FakeAF.requests[0]["path"] == "/3gpp-maf-provisioning/v1/provisioning-sessions"


def test_a_request_without_a_body_has_no_content_type_and_nothing_but_defined_methods_is_sent(af):
    answer("GET", f"/provisioning-sessions/{PS}", 200, b'{"provisioningSessionId":"x"}', {"Content-Type": "application/json"})
    answer("DELETE", f"/provisioning-sessions/{PS}", 204)
    answer("POST", f"/provisioning-sessions/{PS}/certificates", 201, b"", {"Location": f"/x/certificates/c1"})
    call("get_provisioning_session", provisioning_session_id=PS, m1_url=af)
    call("create_server_certificate")
    call("delete_provisioning_session")
    assert [r["method"] for r in FakeAF.requests] == ["GET", "POST", "DELETE"]
    for r in FakeAF.requests:
        assert "Content-Type" not in r["headers"] and r["body"] == b""


def test_if_match_is_sent_and_patch_media_types_are_those_of_the_yaml(af):
    path = f"/provisioning-sessions/{PS}/content-hosting-configuration"
    answer("PATCH", path, 204)
    answer("PUT", path, 204)
    call("patch_content_hosting_configuration", patch='{"name":"x"}', if_match='"abc"', provisioning_session_id=PS, m1_url=af)
    call("patch_content_hosting_configuration", patch='[{"op":"remove","path":"/name"}]', patch_type="json-patch")
    call("replace_content_hosting_configuration", configuration={
        "name": "n", "ingestConfiguration": {"mode": "PULL", "protocol": "urn:3gpp:5gms:content-protocol:http-pull", "baseURL": "http://o/"},
        "distributionConfigurations": [{"distributionId": "d1"}]}, if_match='"abc"')
    first, second, third = FakeAF.requests
    assert first["headers"]["Content-Type"] == "application/merge-patch+json" and first["headers"]["If-Match"] == '"abc"'
    assert second["headers"]["Content-Type"] == "application/json-patch+json" and "If-Match" not in second["headers"]
    assert third["method"] == "PUT" and third["headers"]["Content-Type"] == "application/json" and third["headers"]["If-Match"] == '"abc"'
    assert "baseURL" in json.loads(third["body"])["ingestConfiguration"]


def test_the_purge_body_is_a_form(af):
    answer("POST", f"/provisioning-sessions/{PS}/content-hosting-configuration/purge", 200, b"3")
    text = call("purge_content_hosting_cache", pattern="/a/.*", provisioning_session_id=PS, m1_url=af)
    assert "3 cache entries" in text
    r = FakeAF.requests[0]
    assert r["headers"]["Content-Type"] == "application/x-www-form-urlencoded" and r["body"] == b"pattern=%2Fa%2F.%2A"


def test_a_push_ingest_has_no_base_url_in_the_request(af):
    answer("POST", f"/provisioning-sessions/{PS}/content-hosting-configuration", 201, b"{}")
    call("create_content_hosting_configuration", name="n", ingest_base_url="http://o/", entry_point_relative_path="m.mpd", ingest_pull=False,
         ingest_protocol="http://dashif.org/ingest/v1.2/interface-1", provisioning_session_id=PS, m1_url=af)
    assert "baseURL" not in json.loads(FakeAF.requests[0]["body"])["ingestConfiguration"]


def test_errors_are_explained_with_the_problem_details(af):
    path = f"/provisioning-sessions/{PS}/policy-templates/p1"
    answer("PUT", path, 405, b'{"title":"Method Not Allowed","status":405}', {"Allow": "GET, DELETE", "Content-Type": "application/problem+json"})
    text = call("replace_policy_template", policy_template_id="p1", template={"externalReference": "x", "chargingSpecification": {"sponsorId": "s"}},
                provisioning_session_id=PS, m1_url=af)
    assert "HTTP 405" in text and "Allow: GET, DELETE" in text and "Method Not Allowed" in text
    answer("PUT", path, 412, b'{"title":"Precondition Failed","status":412}', {"Content-Type": "application/problem+json; charset=utf-8"})
    assert "If-Match" in call("replace_policy_template", policy_template_id="p1", template={"externalReference": "x", "chargingSpecification": {}})
    answer("POST", f"/provisioning-sessions/{PS}/content-preparation-templates", 415, b'{"title":"Unsupported Media Type","status":415}',
           {"Content-Type": "application/problem+json", "Accept": "application/vnd.cmmf-encoder-configuration+json"})
    text = call("create_content_preparation_template", template="<x/>", content_type="application/xml")
    assert "HTTP 415" in text and "application/vnd.cmmf-encoder-configuration+json" in text


def test_destroy_answers_follow_the_clauses(af):
    answer("DELETE", f"/provisioning-sessions/{PS}/certificates/c1", 200)
    answer("DELETE", f"/provisioning-sessions/{PS}/content-hosting-configuration", 200)
    answer("DELETE", f"/provisioning-sessions/{PS}", 202)
    assert call("delete_server_certificate", certificate_id="c1", provisioning_session_id=PS, m1_url=af).startswith("SUCCESS")
    assert call("delete_content_hosting_configuration").startswith("SUCCESS")
    assert call("delete_provisioning_session").startswith("ERROR")  # 202 is not defined by Maf_Provisioning
