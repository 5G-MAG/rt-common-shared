<p align="center">
  <img src=".github/banner.svg" width="100%" alt="5G-MAG Reference Tools, Common Tools: Common Shared">
</p>

<p align="center">
  Code shared across the 5G-MAG reference tools: the HTTP server, SDP handling, and the
  Open5GS build tooling that generates OpenAPI bindings from the 3GPP 5G APIs.
</p>

<p align="center">
  <img alt="Status: under development"
    src="https://img.shields.io/badge/Status-Under_Development-yellow">
  <a href="https://github.com/5G-MAG/rt-common-shared/releases"><img alt="Version"
    src="https://img.shields.io/github/v/release/5G-MAG/rt-common-shared?label=Version&sort=semver"></a>
  <a href="LICENSE"><img alt="5G-MAG Public License v1.0"
    src="https://img.shields.io/badge/License-5G--MAG%20PL%20v1.0-blue"></a>
</p>

<p align="center">
  <a href="https://www.5g-mag.com/reference-tools">Project page</a> &nbsp;&middot;&nbsp;
  <a href="https://github.com/5G-MAG/rt-common-shared/issues">Issues</a> &nbsp;&middot;&nbsp;
  <a href="https://www.5g-mag.com/contributing">Contributing</a>
</p>

---

## At a glance

|  |  |
|---|---|
| **Provides** | An HTTP server, SDP parsing and generation, and the `generate_openapi` tooling the Open5GS-based network functions build with |
| **Role** | Shared library and build tooling; not a deployable component of its own |
| **Built with** | C and C++, meson |
| **Used by** | [rt-mbs-function](https://github.com/5G-MAG/rt-mbs-function), [rt-mbs-transport-function](https://github.com/5G-MAG/rt-mbs-transport-function) and other reference tools, as a git submodule |
| **Part of** | [5G-MAG Reference Tools](https://www.5g-mag.com/reference-tools) |

## Introduction

Files, components and common example configurations share by multiple 5G-MAG repositories.

### 5G Media Streaming (5GMS)

Includes example configurations and common scripts for the 5GMS (rt-5gms-\*) Reference Tools.

More information can be found in the corresponding [subfolder](5gms/README.md).

### MBMS and LTE-based 5G Broadcast (MBMS)

Includes example configurations for the LTE-based 5G Broadcast (rt-mbms-\*) Reference tools.

More information can be found in the corresponding [subfolder](mbms/README.md).

Information on MBMS Service Announcement formats can be found [here](https://5g-mag.github.io/Getting-Started/pages/lte-based-5g-broadcast/rt-common-shared/MBMS-service-announcement-files.html).

### Open5GS Tools

Includes scripts related to the OpenAPI generator.

More information can be found in the corresponding [subfolder](open5gs-tools/).

### Data Reporting 5G_APIs overrides

Includes modified versions of the OpenAPI YAML files from the 5G_APIs repository specific to Data Reporting for use with a Data Collection Application Function.

More information can be found in the corresponding [subfolder](/data-reporting/5G_APIs-overrides/README.md).

## Downloading

This repository is normally consumed as a git submodule of the project that uses it, and cloning
that project with `--recurse-submodules` brings it in. To work on it directly:

```bash
git clone https://github.com/5G-MAG/rt-common-shared.git
cd rt-common-shared
```

## Building

There is nothing to build here on its own: the libraries are built as part of the project that
includes them, and the tooling under `open5gs-tools/` is run in place.

## Contributing

Contributions are welcome. How to raise an issue, fork the repository and open a pull request, and
the Contributor License Agreement required before code can be merged, are described at
<https://www.5g-mag.com/contributing>.

## License

See [LICENSE](LICENSE).
