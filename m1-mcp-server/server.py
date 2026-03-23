#!/usr/bin/env python3
"""
5G-MAG M1 Interface MCP Server
================================
Entry point. Imports the shared FastMCP instance and all tool modules
(each module self-registers its tools via the @mcp.tool() decorator).

Tool modules
------------
  tools/provisioning.py          create / get / enumerate / delete provisioning sessions
  tools/content_hosting.py       create / get / delete content hosting configurations
  tools/consumption_reporting.py create consumption reporting configurations
  tools/metrics_reporting.py     get / delete metrics reporting configurations
"""

from mcp_instance import mcp
import tools  # noqa: F401 — side-effect: registers all tools with `mcp`


if __name__ == "__main__":
    mcp.run()
