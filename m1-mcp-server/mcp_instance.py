from mcp.server.fastmcp import FastMCP

mcp = FastMCP(
    name="5G-MAG M1 Interface",
    instructions=(
        "You help users configure 5G Media Streaming (5GMS) sessions via the 3GPP M1 interface. "
        "Always guide users through the 3-step creation workflow in order:\n"
        "  Step 1 → create_provisioning_session\n"
        "  Step 2 → create_content_hosting_configuration\n"
        "  Step 3 → create_consumption_reporting_configuration\n"
        "Use GET tools to inspect existing resources, replace_*/patch_* tools to change them (pass the ETag of a GET as if_match to avoid "
        "overwriting a change made by someone else) and DELETE tools to clean them up. "
        "Only downlink Provisioning Sessions (MS_DOWNLINK) are supported. "
        "Explain each parameter in plain language and suggest the next step after every success."
    ),
)
