#!/usr/bin/env python3
"""
5G-MAG M1 Interface MCP Server
================================
Entry point. Imports the shared FastMCP instance and all tool modules
(each module self-registers its tools via the @mcp.tool() decorator).

The tools speak the Maf_Provisioning API of TS 26.510 V19.2.0 (reference point M1, base path /3gpp-maf-provisioning/v1) and only the
methods that its OpenAPI files define. Edge resources, content publishing (uplink), RTC, event exposure, eMBMS/MBS and remote control
are not offered.

Tool modules
------------
  tools/provisioning.py          create / get / enumerate / delete provisioning sessions, get content protocols
  tools/certificates.py          create / reserve / upload / get / delete server certificates
  tools/content_hosting.py       create / get / replace / patch / delete content hosting configurations, purge the cache
  tools/content_preparation.py   create / get / replace / patch / delete content preparation templates
  tools/policy_templates.py      create / get / replace / patch / delete policy templates
  tools/consumption_reporting.py create / get / replace / patch / delete the consumption reporting configuration
  tools/metrics_reporting.py     create / get / replace / patch / delete metrics reporting configurations
  tools/event_data_processing.py create / get / replace / patch / delete event data processing configurations
"""

from mcp_instance import mcp
import tools  # noqa: F401 — side-effect: registers all tools with `mcp`


if __name__ == "__main__":
    mcp.run()
