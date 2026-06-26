# ---------------------------------------------------------------------------
# Shared session state — persisted across tool calls within a single MCP session
# ---------------------------------------------------------------------------

_state: dict = {
    "m1_url": None,
    "maf_url": None,
    "provisioning_session_id": None,
}


def get_m1_url() -> str | None:
    return _state["m1_url"]


def set_m1_url(url: str) -> None:
    _state["m1_url"] = url.rstrip("/")


def get_maf_url() -> str | None:
    return _state["maf_url"]


def set_maf_url(url: str) -> None:
    _state["maf_url"] = url.rstrip("/")


def get_session_id() -> str | None:
    return _state["provisioning_session_id"]


def set_session_id(session_id: str) -> None:
    _state["provisioning_session_id"] = session_id


def clear_session() -> None:
    _state["provisioning_session_id"] = None
