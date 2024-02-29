# 5G-MAG Reference Tools 5GMS common files

This directory contains files that are shared by two or more of these repositories:
- [rt-5gms-application](https://github.com/5G-MAG/rt-5gms-application)
- [rt-5gms-application-function](https://github.com/5G-MAG/rt-5gms-application-function)
- [rt-5gms-application-provider](https://github.com/5G-MAG/rt-5gms-application-provider)
- [rt-5gms-application-server](https://github.com/5G-MAG/rt-5gms-application-server)
- [rt-5gms-media-session-handler](https://github.com/5G-MAG/rt-5gms-media-session-handler)
- [rt-5gms-media-stream-handler](https://github.com/5G-MAG/rt-5gms-media-stream-handler)

The `scripts` directory contains scripts which may be used in building the
projects from the repositories named above.

The `5G_APIs-overrides` directory contains files that can be used with the `open5gs-tools/scripts/generate_openapi` script to override or supplement the OpenAPI YAML files from the [5G\_APIs](https://forge.3gpp.org/rep/all/5G_APIs.git) repository. This is for OpenAPI YAML files related to 5GMS that are used by two or more of the repositories noted above.
