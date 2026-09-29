<p align="center">
  <img src=".github/banner.svg" width="100%" alt="Reference Tools · Common Tools: Common Shared Tools">
</p>

<p align="center">
  Files, components and example configurations shared by several 5G-MAG repositories.
</p>

<p align="center">
  <img alt="Status: under development"
    src="https://img.shields.io/badge/Status-Under%20Development-e67e22">
  <a href="https://github.com/5G-MAG/rt-common-shared/releases"><img alt="Version"
    src="https://img.shields.io/github/v/release/5G-MAG/rt-common-shared?label=Version"></a>
  <a href="LICENSE"><img alt="License: 5G-MAG Public License v1.0"
    src="https://img.shields.io/badge/License-5G--MAG%20PL%20v1.0-blue"></a>
</p>

<p align="center">
  <a href="https://www.5g-mag.com/reference-tools/common-tools/">Project page</a> &nbsp;&middot;&nbsp;
  <a href="https://github.com/5G-MAG/rt-common-shared/issues">Issues</a> &nbsp;&middot;&nbsp;
  <a href="https://www.5g-mag.com/contributing">Contributing</a>
</p>

---

## At a glance

|  |  |
|---|---|
| **Part of** | [Common Tools](https://www.5g-mag.com/reference-tools/common-tools/) |

## Introduction

This repository holds files, components and example configurations that several 5G-MAG Reference
Tools repositories use in common. There is no top-level build: each folder stands on its own, and
all but `open5gs-tools` have a README of their own, linked below.

| Folder | Contents |
|---|---|
| [5gms](5gms/README.md) | 5G Media Streaming (5GMS): example configurations and common scripts for the 5GMS (rt-5gms-\*) Reference Tools |
| [mbms](mbms/README.md) | MBMS and LTE-based 5G Broadcast: example configurations for the LTE-based 5G Broadcast (rt-mbms-\*) Reference Tools, and a description of the [MBMS Service Announcement formats](mbms/MBMS-service-announcement-files.md) |
| [simple-express-server](simple-express-server/README.md) | a simple HTTP server, based on express.js, for hosting static files |
| [open5gs-tools](open5gs-tools/) | Open5GS tools: scripts related to the OpenAPI generator |
| [data-reporting/5G_APIs-overrides](data-reporting/5G_APIs-overrides/README.md) | Data Reporting 5G_APIs overrides: modified versions of the OpenAPI YAML files from the 5G_APIs repository, specific to Data Reporting, for use with a Data Collection Application Function |
| [avcodec-build](avcodec-build/README.md) | a helper script to build FFmpeg libraries for Android |
| [m1-mcp-server](m1-mcp-server/README.md) | M1 Interface MCP Server: an MCP (Model Context Protocol) server that exposes the 3GPP M1 interface (TS 26.512) as tools an AI can call, so that LLM agents can configure 5G Media Streaming sessions in natural language |
| [docker-monitor](docker-monitor/README.md) | Docker Monitor: a web-based monitor of the status of Docker containers, grouped by service, shared by 5G-MAG Docker-based projects such as [rt-5gms-examples](https://github.com/5G-MAG/rt-5gms-examples) and [rt-mbs-examples](https://github.com/5G-MAG/rt-mbs-examples) |

## Contributing

Contributions are welcome. How to raise an issue, fork the repository and open a pull request, and
the Contributor License Agreement required before code can be merged, are described at
<https://www.5g-mag.com/contributing>.

## License

Distributed under the 5G-MAG Public License v1.0. See [LICENSE](LICENSE). Open source software
used directly by scripts in this repository is listed in [ATTRIBUTION_NOTICE](ATTRIBUTION_NOTICE).
