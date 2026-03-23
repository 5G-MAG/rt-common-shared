# 5G-MAG M1 Interface MCP Server

An [MCP (Model Context Protocol)](https://modelcontextprotocol.io/) server that exposes the **3GPP M1 interface** (TS 26.512) as AI-callable tools, enabling LLM agents to configure 5G Media Streaming (5GMS) sessions through natural language.

## Overview

This server wraps the 5GMS Application Function's M1 REST API into a full set of tools covering the complete provisioning lifecycle — create, inspect, and delete sessions and their configurations.

The standard 3-step provisioning workflow is:

```
Step 1 → create_provisioning_session
Step 2 → create_content_hosting_configuration
Step 3 → create_consumption_reporting_configuration
```

Additional tools allow you to inspect and manage existing resources at any time.

## Features

- Full 3-step 5GMS provisioning workflow via MCP tools
- GET and DELETE tools for all major resources
- Enumerate all provisioning sessions via the 5G-MAG management API
- Session state persisted across tool calls (M1 URL, MAF URL, and session ID remembered automatically)
- Supports both DASH and HLS entry points
- JSON template support for content hosting configuration
- Clear, structured error messages with troubleshooting hints
- Modular codebase — each resource group lives in its own tool module
- Compatible with any MCP client (Claude Desktop, Claude Code, custom agents)

## Requirements

- Python 3.10+
- A running [5G-MAG Reference Tools](https://github.com/5G-MAG/rt-5gms-application-function) Application Function instance with the M1 interface accessible

## Installation

```bash
git clone https://github.com/aaronmontilla/M1-mcp.git
cd M1-mcp
pip install -r requirements.txt
```

### Dependencies

| Package | Purpose |
|---------|---------|
| `mcp[cli]` | MCP server framework (FastMCP) |
| `httpx` | Async HTTP client for M1 API calls |

## Usage

### Running the server

```bash
python server.py
```

The server communicates over stdio and is intended to be launched by an MCP host (e.g. Claude Desktop).

### Connecting to Claude Desktop

Add the following to your `claude_desktop_config.json`:

```json
{
  "mcpServers": {
    "5gms-m1": {
      "command": "python",
      "args": ["/path/to/M1-mcp/server.py"]
    }
  }
}
```

### Connecting to Claude Code

```bash
claude mcp add 5gms-m1 python /path/to/M1-mcp/server.py
```

## Tools

### Provisioning Sessions

| Tool | Method | Description |
|------|--------|-------------|
| `create_provisioning_session` | POST | Create a new provisioning session (Step 1) |
| `get_provisioning_session` | GET | Retrieve details for an existing session |
| `enumerate_provisioning_sessions` | GET | List all provisioning sessions (uses MAF management API) |
| `delete_provisioning_session` | DELETE | Delete a provisioning session and all its configurations |

#### `create_provisioning_session`

| Parameter | Required | Default | Description |
|-----------|----------|---------|-------------|
| `asp_id` | Yes | — | Application Service Provider ID |
| `app_id` | Yes | — | Application ID |
| `provisioning_session_type` | No | `DOWNLINK` | `DOWNLINK` or `UPLINK` |
| `m1_url` | Yes (first call) | — | Base URL of M1 interface, e.g. `http://192.168.1.100:7778` |

#### `enumerate_provisioning_sessions`

| Parameter | Required | Default | Description |
|-----------|----------|---------|-------------|
| `maf_url` | Yes (first call) | — | Base URL of the 5G-MAG management API, e.g. `http://192.168.1.100:7779` |

---

### Content Hosting Configuration

| Tool | Method | Description |
|------|--------|-------------|
| `create_content_hosting_configuration` | POST | Define media ingest origin and distribution entry point (Step 2) |
| `get_content_hosting_configuration` | GET | Retrieve the current content hosting configuration |
| `delete_content_hosting_configuration` | DELETE | Remove the content hosting configuration |

#### `create_content_hosting_configuration`

| Parameter | Required | Default | Description |
|-----------|----------|---------|-------------|
| `name` | Yes | — | Friendly name for the configuration |
| `ingest_base_url` | Yes | — | Origin server base URL |
| `entry_point_relative_path` | Yes | — | Path to `.mpd` (DASH) or `.m3u8` (HLS) manifest |
| `entry_point_content_type` | No | `application/dash+xml` | MIME type of the manifest |
| `domain_name_alias` | No | None | CDN hostname alias |
| `dash_profiles` | No | `["urn:mpeg:dash:profile:isoff-live:2011"]` | DASH profile URNs |
| `ingest_pull` | No | `true` | Pull vs push ingest |
| `ingest_protocol` | No | HTTP pull URN | Ingest protocol URN |

---

### Consumption Reporting Configuration

| Tool | Method | Description |
|------|--------|-------------|
| `create_consumption_reporting_configuration` | POST | Enable viewer analytics reporting (Step 3) |
| `get_consumption_reporting_configuration` | GET | Retrieve the current consumption reporting configuration |

#### `create_consumption_reporting_configuration`

All parameters are optional.

| Parameter | Required | Default | Description |
|-----------|----------|---------|-------------|
| `reporting_interval` | No | `10` | Seconds between client reports |
| `sample_percentage` | No | `100.0` | Percentage of clients that report |
| `location_reporting` | No | `true` | Include geographic location |
| `access_reporting` | No | `true` | Include network access info |
| `provisioning_session_id` | No | state | Override the session ID from state |
| `m1_url` | No | state | Override the M1 URL from state |

---

### Metrics Reporting Configuration

| Tool | Method | Description |
|------|--------|-------------|
| `create_metrics_reporting_configuration` | POST | Create a new metrics reporting configuration |
| `get_metrics_reporting_configuration` | GET | Retrieve an existing metrics reporting configuration |
| `delete_metrics_reporting_configuration` | DELETE | Remove a metrics reporting configuration |

#### `create_metrics_reporting_configuration`

| Parameter | Required | Default | Description |
|-----------|----------|---------|-------------|
| `scheme` | No | `urn:3gpp:5gms:metrics-reporting:qoe-metrics` | Metrics reporting scheme URN |
| `reporting_interval` | No | `10` | Seconds between client reports |
| `sample_percentage` | No | `100.0` | Percentage of clients that report |
| `sampling_period` | No | None | Seconds between local metric samples |
| `metrics` | No | AF default | List of metric URNs to collect (e.g. `urn:3GPP:ns:PSS:DASH:QM10#BufferLevel`) |
| `url_filters` | No | None | URL patterns to restrict reporting scope |
| `data_network_name` | No | None | DNN/APN to scope the configuration to a specific network |
| `slice_scope` | No | None | List of S-NSSAI objects (e.g. `[{"sst": 1, "sd": "000001"}]`) |
| `provisioning_session_id` | No | state | Override the session ID from state |
| `m1_url` | No | state | Override the M1 URL from state |

## Content Hosting Configuration Template

The file `content_hosting_config_template.json` is used as a base for `create_content_hosting_configuration`. You can edit it to add additional fields that will be merged with the values you provide at runtime:

```json
{
    "name": "My 5GMS Stream",
    "ingestConfiguration": {
        "pull": true,
        "protocol": "urn:3gpp:5gms:content-protocol:http-pull-ingest",
        "baseURL": "https://example.com/media/"
    },
    "distributionConfigurations": [
        {
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
       [calls create_provisioning_session(asp_id="acme", app_id="live-event-1", m1_url="http://10.0.0.5:7778")]
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
├── state.py                             # Shared session state (M1 URL, MAF URL, session ID)
├── tools/
│   ├── __init__.py                      # Imports all tool modules (self-registration)
│   ├── provisioning.py                  # create / get / enumerate / delete provisioning sessions
│   ├── content_hosting.py               # create / get / delete content hosting configurations
│   ├── consumption_reporting.py         # create consumption reporting configurations
│   └── metrics_reporting.py             # get / delete metrics reporting configurations
├── content_hosting_config_template.json # Base template for create_content_hosting_configuration
├── requirements.txt                     # Python dependencies
└── README.md                            # This file
```

## Standards Reference

- **3GPP TS 26.512** — 5G Media Streaming (5GMS); Protocols
- **M1 Interface** — Provisioning interface between AF and AS
- **5G-MAG Reference Tools** — Open-source 5GMS implementation

## Related Projects

- [5G-MAG Reference Tools](https://github.com/5G-MAG/rt-5gms-application-function) — The Application Function this server talks to
- [Model Context Protocol](https://modelcontextprotocol.io/) — The protocol used to expose tools to AI agents

## License

MIT — see [LICENSE](LICENSE) for details.
