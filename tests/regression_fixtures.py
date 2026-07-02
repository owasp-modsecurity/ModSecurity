"""Loads tests/regression/fixtures/*.json (produced by dump_regression_fixtures.pl
from tests/regression/*.t) into native Python structures.

qr// dicts ({"__regex__": 1, "pattern": ..., "flags": ...}) become compiled
re.Pattern objects; base64-encoded request bodies become bytes.
"""

import base64
import os
import re
from pathlib import Path
from typing import Any, Dict, List

from .apache_server import _resolve_httpd_and_modules_dir, _resolve_server_root

FIXTURES_DIR = Path(__file__).resolve().parent / "regression" / "fixtures"

# Mirrors dump_regression_fixtures.pl's @path_keys: fixtures are dumped with
# this-machine paths replaced by "##KEY##" placeholders (JSON forbids raw
# control-byte delimiters in strings, and plain text needs no escaping when
# substituted directly into the serialized JSON), so the checked-in fixtures
# don't hard-code whatever machine happened to generate them. Substituted
# back to real local paths here, before any test uses them.
_PATH_KEYS = (
    "DEBUG_LOG", "AUDIT_LOG", "ERROR_LOG", "HTTPD_CONF",
    "DATA_DIR", "TEMP_DIR", "UPLOAD_DIR", "CONF_DIR", "LOGS_DIR", "HTDOCS",
    "TEST_SERVER_ROOT", "REGRESSION_DIR", "SCRIPT_DIR", "DIST_ROOT",
    "SERVER_ROOT", "MODULES_DIR", "RUNASUSER",
)


def _resolve_local_env() -> Dict[str, str]:
    """Mirrors the %ENV block dump_regression_fixtures.pl used when it
    generated the fixtures, resolved for *this* machine instead."""
    script_dir = Path(__file__).resolve().parent
    reg_dir = script_dir / "regression"
    sroot_dir = reg_dir / "server_root"
    logs_dir = sroot_dir / "logs"
    httpd_path, modules_dir = _resolve_httpd_and_modules_dir(script_dir)

    return {
        "SERVER_ROOT": _resolve_server_root(httpd_path),
        "TEST_SERVER_ROOT": str(sroot_dir),
        "DATA_DIR": str(sroot_dir / "data"),
        "TEMP_DIR": str(sroot_dir / "tmp"),
        "UPLOAD_DIR": str(sroot_dir / "upload"),
        "CONF_DIR": str(sroot_dir / "conf"),
        "MODULES_DIR": modules_dir,
        "LOGS_DIR": str(logs_dir),
        "SCRIPT_DIR": str(script_dir),
        "REGRESSION_DIR": str(reg_dir),
        "DIST_ROOT": str(script_dir.parent),
        "AUDIT_LOG": str(logs_dir / "modsec_audit.log"),
        "DEBUG_LOG": str(logs_dir / "modsec_debug.log"),
        "ERROR_LOG": str(logs_dir / "error.log"),
        "HTTPD_CONF": str(sroot_dir / "conf" / "httpd.conf"),
        "HTDOCS": str(sroot_dir / "htdocs"),
        "RUNASUSER": os.environ.get("USER") or os.environ.get("LOGNAME") or os.environ.get("USERNAME") or "unknown",
    }


def _substitute_placeholders(json_text: str, env: Dict[str, str]) -> str:
    for key in _PATH_KEYS:
        json_text = json_text.replace(f"##{key}##", env[key])
    return json_text

# Perl regex modifier letters -> Python re flags. 'x'/'p'/'a'/'d'/'l'/'u' are
# not used by any pattern in the current test corpus and are intentionally
# not mapped.
_PERL_FLAG_TO_RE = {
    "s": re.DOTALL,
    "m": re.MULTILINE,
    "i": re.IGNORECASE,
    "x": re.VERBOSE,
}


def _decode_value(val: Any) -> Any:
    if isinstance(val, dict) and val.get("__regex__"):
        flags = 0
        for ch in val.get("flags", ""):
            flags |= _PERL_FLAG_TO_RE.get(ch, 0)
        # Log/response buffers are bytes (arbitrary encodings/binary content
        # can appear), so patterns must be bytes patterns too.
        return re.compile(val["pattern"].encode("utf-8"), flags)
    if isinstance(val, dict) and val.get("__unsupported_coderef__"):
        return _UnsupportedCoderef()
    if isinstance(val, dict):
        return {k: _decode_value(v) for k, v in val.items()}
    if isinstance(val, list):
        return [_decode_value(v) for v in val]
    return val


class _UnsupportedCoderef:
    """Marker for a Perl coderef field that couldn't be captured statically
    (e.g. request => sub {...}). A fixture using one of these should be
    skipped, not silently dropped - see `unsupported` on the test entry."""


def _decode_request(req: Dict[str, Any]) -> Dict[str, Any]:
    if req is None:
        return None
    decoded = dict(req)
    if req["__type__"] == "http_request":
        decoded["content"] = base64.b64decode(req["content"])
    elif req["__type__"] == "raw":
        decoded["data"] = base64.b64decode(req["data"])
    return decoded


def load_fixture_file(path: Path) -> List[Dict[str, Any]]:
    import json

    json_text = _substitute_placeholders(path.read_text(), _resolve_local_env())
    entries = json.loads(json_text)
    decoded = []
    for entry in entries:
        out = dict(entry)
        for key in ("match_log", "match_response", "match_file"):
            if key in out:
                out[key] = _decode_value(out[key])
        if "request" in out:
            out["request"] = _decode_request(out["request"])
        decoded.append(out)
    return decoded


def discover_fixtures() -> List[Dict[str, Any]]:
    """Returns a flat list of test entries across every fixture file, each
    tagged with its source file/type/index for readable pytest test IDs."""
    results = []
    for path in sorted(FIXTURES_DIR.glob("*/*.json")):
        type_dir = path.parent.name
        for i, entry in enumerate(load_fixture_file(path)):
            entry["_source"] = f"{type_dir}/{path.stem}.t"
            entry["_index"] = i
            results.append(entry)
    return results
