from mcp.server.fastmcp import FastMCP

mcp = FastMCP(
    name="5G-MAG M1 Interface",
    instructions=(
        "You help users configure 5G Media Streaming (5GMS) sessions via the 3GPP M1 interface. "
        "Always guide users through the 3-step creation workflow in order:\n"
        "  Step 1 → create_provisioning_session\n"
        "  Step 2 → create_content_hosting_configuration\n"
        "  Step 3 → create_consumption_reporting_configuration\n"
        "Use GET tools to inspect existing resources and DELETE tools to clean them up. "
        "Explain each parameter in plain language and suggest the next step after every success."
    ),
)
