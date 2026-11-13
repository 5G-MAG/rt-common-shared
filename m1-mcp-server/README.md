# 5G-MAG M1 Interface MCP Server

An [MCP (Model Context Protocol)](https://modelcontextprotocol.io/) server that exposes the **3GPP M1 interface** (TS 26.512) as AI-callable tools, enabling LLM agents to configure 5G Media Streaming (5GMS) sessions through natural language.

## Overview

This server wraps the 5GMS Application Function's M1 REST API (the Release 19 Maf_Provisioning API of TS 26.510 V19.2.0, base path `/3gpp-maf-provisioning/v1`) into tools covering the provisioning lifecycle of a downlink media streaming service: Provisioning Sessions, Content Protocols, Server Certificates, Content Hosting, Content Preparation Templates, Policy Templates, Consumption Reporting, Metrics Reporting and Event Data Processing. Only the methods that the OpenAPI files define are used (the AF answers 405 to anything else, OPTIONS and HEAD included). Edge resources, content publishing (uplink), RTC, event exposure, eMBMS/MBS and remote control are not offered.

The standard 3-step provisioning workflow is:

```
Step 1 → create_provisioning_session
Step 2 → create_content_hosting_configuration
Step 3 → create_consumption_reporting_configuration
```

Every resource has get, replace (PUT), patch (PATCH) and delete tools; the bodies of the create and replace tools carry the Release 19 schemas (tool input schemas generated from `models.py`).

## Features

- Full 3-step 5GMS provisioning workflow via MCP tools
- Create, get, replace, patch and delete tools for every Maf_Provisioning resource
- Tool input schemas of the Release 19 resources (required properties, enumerations, limits); read-only properties are not part of a request
- Entity tags: every answer shows the ETag, and the update and delete tools take it as `if_match` (TS 26.510 clause 7.1.4.4; 412 explained)
- ProblemDetails of error answers shown, with explanations for 405 (Allow), 412, 413, 415
- Enumerate all provisioning sessions through the M1 interface (`GET /provisioning-sessions`)
- Session state persisted across tool calls (M1 URL and session ID remembered automatically)
- Supports both DASH and HLS entry points
- Modular codebase — each resource group lives in its own tool module
- Compatible with any MCP client (Claude Desktop, Claude Code, custom agents)

## Requirements

- Python 3.10+
- A running [5G-MAG Reference Tools](https://github.com/5G-MAG/rt-media-application-function) Application Function instance with the M1 interface accessible

## Installation

```bash
git clone https://github.com/5G-MAG/rt-common-shared.git
cd m1-mcp-server
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

### Dependencies

| Package | Purpose |
|---------|---------|
| `mcp[cli]` (>= 1.2, < 2) | MCP server framework (FastMCP; mcp 2.x renamed it) |
| `httpx` | Async HTTP client for M1 API calls |

## Usage

### Running the server

```bash
source .venv/bin/activate
python server.py
```

The server communicates over stdio and is intended to be launched by an MCP host (e.g. Claude Desktop).

### Connecting to Claude Desktop

Add the following to your `claude_desktop_config.json`, using the **venv Python interpreter** so the installed dependencies are available:

```json
{
  "mcpServers": {
    "5gms-m1": {
      "command": "/path/to/M1-mcp/.venv/bin/python",
      "args": ["/path/to/M1-mcp/server.py"]
    }
  }
}
```

On Windows replace `.venv/bin/python` with `.venv\Scripts\python.exe`.

### Connecting to Claude Code

```bash
claude mcp add 5gms-m1 /path/to/M1-mcp/.venv/bin/python /path/to/M1-mcp/server.py
```

### Connecting to OpenCode

Add the following to your `opencode.json`, using the **venv Python interpreter** so the installed dependencies are available:

```json
{
  "$schema": "https://opencode.ai/config.json",
  "mcp": {
    "5gms-m1": {
      "type": "local",
      "command": [
        "/path/to/M1-mcp/.venv/bin/python",
        "/path/to/M1-mcp/server.py"
      ],
      "enabled": true
    }
  }
}
```

On Windows replace `.venv/bin/python` with `.venv\Scripts\python.exe`.

## Tools

Every tool takes `m1_url` (remembered after the first call) and, where a Provisioning Session is needed, `provisioning_session_id` (the one created last when omitted). Update and delete tools take `if_match`, a strong entity tag shown by an earlier answer.

| Module | Tools (HTTP) |
|--------|--------------|
| Provisioning Sessions | `create_provisioning_session` (POST), `get_provisioning_session` (GET), `enumerate_provisioning_sessions` (GET), `delete_provisioning_session` (DELETE), `get_content_protocols` (GET) |
| Server Certificates | `create_server_certificate` (POST), `reserve_server_certificate` (POST ?csr=true), `upload_server_certificate` (PUT application/x-pem-file), `get_server_certificate` (GET), `delete_server_certificate` (DELETE) |
| Content Hosting | `create_content_hosting_configuration` (POST, one distribution), `create_content_hosting_configuration_from_schema` (POST, full representation), `get_…`, `replace_…` (PUT), `patch_…` (PATCH), `delete_…`, `purge_content_hosting_cache` (POST form) |
| Content Preparation Templates | `create_content_preparation_template`, `get_…`, `replace_…`, `patch_…`, `delete_content_preparation_template` |
| Policy Templates | `create_policy_template`, `get_…`, `replace_…`, `patch_…`, `delete_policy_template` |
| Consumption Reporting | `create_consumption_reporting_configuration`, `get_…`, `replace_…`, `patch_…`, `delete_consumption_reporting_configuration` |
| Metrics Reporting | `create_metrics_reporting_configuration`, `get_…`, `replace_…`, `patch_…`, `delete_metrics_reporting_configuration` |
| Event Data Processing | `create_event_data_processing_configuration`, `get_…`, `replace_…`, `patch_…`, `delete_event_data_processing_configuration` |

`patch_…` tools take the patch as JSON text and `patch_type` `merge-patch` (`application/merge-patch+json`, the default) or `json-patch` (`application/json-patch+json`).

### `create_provisioning_session`

| Parameter | Required | Description |
|-----------|----------|-------------|
| `external_service_id` | Yes | Identifier of the session towards the Media Client, a reverse FQDN, e.g. `com.example.live-event-1` |
| `app_id` | Yes | Application ID |
| `asp_id` | No | Application Service Provider ID |
| `location_reporting` | No | Require the Media Session Handler to report UE location (a property of the Provisioning Session) |
| `m1_url` | Yes (first call) | Base URL of the M1 interface, e.g. `http://192.168.1.100:7777` |

The Provisioning Session type is always `MS_DOWNLINK`.

### `create_content_hosting_configuration`

| Parameter | Required | Default | Description |
|-----------|----------|---------|-------------|
| `name` | Yes | — | Label of the configuration |
| `ingest_base_url` | Yes | — | Origin base URL (`ingestConfiguration.baseURL`) |
| `entry_point_relative_path` | Yes | — | Path of the manifest relative to the ingest base URL |
| `entry_point_content_type` | No | `application/dash+xml` | `application/dash+xml` or `application/vnd.apple.mpegurl` |
| `domain_name_alias` | No | — | Alternative FQDN (needs `certificate_id` with a matching name) |
| `certificate_id` | No | — | A Server Certificate of the session |
| `dash_profiles` | No | isoff-live 2011 | Profile URIs of the entry point |
| `ingest_pull` | No | `true` | `false` for a PUSH ingest (then the protocol has to be a DASH-IF term; the AF assigns the base URL) |
| `ingest_protocol` | No | `urn:3gpp:5gms:content-protocol:http-pull` | Term of TS 26.512 table 8.1-1 |

The AF assigns `baseURL` and `canonicalDomainName` of a pull distribution. For several distributions, caching, geo-fencing, URL signing or a push distribution use `create_content_hosting_configuration_from_schema`.

### `create_consumption_reporting_configuration`

`reporting_interval` (default 10 s, > 0), `sample_percentage` (default 100.0), `access_reporting` (default true). The UE location is reported when the Provisioning Session has `locationReporting`; it is not a property of this configuration.

### `create_metrics_reporting_configuration`

`sampling_period` (required, > 0), `scheme` (default `urn:3GPP:ns:PSS:DASH:QM10`), `reporting_interval`, `sample_percentage`, `metrics`, `url_filters`, `data_network_name`, `slice_scope` (`[{"sst": 1, "sd": "000001"}]`), `reporting_start_offset`, `reporting_duration`, `positive_crossing_thresholds`, `negative_crossing_thresholds` (`{metric URI: [values]}`), `location_filter` (LocationArea5G objects).

## Content Hosting Configuration Template

The file `content_hosting_config_template.json` is used as a base for `create_content_hosting_configuration`. You can edit it to add additional fields that will be merged with the values you provide at runtime:

```json
{
    "name": "My 5GMS Stream",
    "ingestConfiguration": {
        "mode": "PULL",
        "protocol": "urn:3gpp:5gms:content-protocol:http-pull",
        "baseURL": "https://example.com/media/"
    },
    "distributionConfigurations": [
        {
            "distributionId": "d1",
            "entryPoint": {
                "relativePath": "stream/manifest.mpd",
                "contentType": "application/dash+xml",
                "profiles": ["urn:mpeg:dash:profile:isoff-live:2011"]
            }
        }
    ]
}
```

## Example Workflow

```
User: Set up a 5GMS stream for my live event.

Agent: Step 1 — I'll create a provisioning session.
       [calls create_provisioning_session(external_service_id="com.acme.live-event-1", app_id="live-event-1", m1_url="http://10.0.0.5:7777")]
       → Session ID: abc-123

Agent: Step 2 — Now I'll configure the content hosting.
       [calls create_content_hosting_configuration(
           name="Live Event Stream",
           ingest_base_url="https://origin.acme.com/",
           entry_point_relative_path="live/event1/manifest.mpd"
       )]
       → Content hosting configured.

Agent: Step 3 — Finally, enabling consumption analytics.
       [calls create_consumption_reporting_configuration(reporting_interval=30)]
       → All done! Your 5GMS session is ready.
```

## Project Structure

```
M1-mcp/
├── server.py                            # Entry point — imports mcp instance and tool modules
├── mcp_instance.py                      # Shared FastMCP server instance
├── state.py                             # Shared session state (M1 URL, session ID)
├── m1_http.py                           # Requests to the AF with the HTTP rules of TS 26.510 clause 7.1
├── models.py                            # Release 19 request bodies (Pydantic): the schemas of the tools
├── tools/
│   ├── __init__.py                      # Imports all tool modules (self-registration)
│   ├── _common.py                       # create / retrieve / replace / patch / destroy helpers
│   ├── provisioning.py                  # provisioning sessions, content protocols
│   ├── certificates.py                  # server certificates
│   ├── content_hosting.py               # content hosting configuration, cache purge
│   ├── content_preparation.py           # content preparation templates
│   ├── policy_templates.py              # policy templates
│   ├── consumption_reporting.py         # consumption reporting configuration
│   ├── metrics_reporting.py             # metrics reporting configurations
│   └── event_data_processing.py         # event data processing configurations
├── tests/
│   ├── test_tools.py                    # unit tests against a local stand-in for the AF (pytest)
│   └── live_check.py                    # life cycle of every tool against a running AF
├── content_hosting_config_template.json # Base template for create_content_hosting_configuration
├── requirements.txt                     # Python dependencies
└── README.md                            # This file
```

## Tests

```bash
python -m pytest tests/test_tools.py        # no AF needed (needs pytest in the venv)
python tests/live_check.py http://host:7777  # runs every tool against a running AF and deletes what it created
```

## Standards Reference

- **3GPP TS 26.510 V19.2.0** — Media delivery; Interactions and APIs (Maf_Provisioning at M1)
- **3GPP TS 26.512 V19.3.0** — 5G Media Streaming (5GMS); Protocols (content protocol terms, table 8.1-1)
- **5G-MAG Reference Tools** — Open-source 5GMS implementation

## Related Projects

- [5G-MAG Reference Tools](https://github.com/5G-MAG/rt-media-application-function) — The Application Function this server talks to
- [Model Context Protocol](https://modelcontextprotocol.io/) — The protocol used to expose tools to AI agents

## Acknowledgement

This work has been performed in the framework of the **AGENTIC6G: AUTONOMOUS MULTI-AGENT AGENTIC AI SYSTEM FOR 6G NETWORKS** project (Grant Agreement No. 101290342), funded by the Smart Networks and Services Joint Undertaking (SNS JU) under the European Union's Horizon Europe research and innovation programme.

The SNS JU receives support from the European Union's Horizon Europe research and innovation programme and the SNS JU members (public and private).

Views and opinions expressed are however those of the author(s) only and do not necessarily reflect those of the European Union or the SNS JU. Neither the European Union nor the granting authority can be held responsible for them.

## License

This software is licensed under the **5G-MAG Public License (v1.0)** — see [LICENSE](../LICENSE) for details.
