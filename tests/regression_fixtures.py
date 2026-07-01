"""Loads tests/regression/fixtures/*.json (produced by dump_regression_fixtures.pl
from tests/regression/*.t) into native Python structures.

qr// dicts ({"__regex__": 1, "pattern": ..., "flags": ...}) become compiled
re.Pattern objects; base64-encoded request bodies become bytes.
"""

import base64
import re
from pathlib import Path
from typing import Any, Dict, List

FIXTURES_DIR = Path(__file__).resolve().parent / "regression" / "fixtures"

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

    entries = json.loads(path.read_text())
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
