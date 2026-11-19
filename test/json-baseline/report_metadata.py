"""Source attribution from the orchestrator's verified probe-build manifest."""
import hashlib
import json
import os
from pathlib import Path
import re


def production_commit(probe):
    """Return the recorded source SHA, or None for an unverified standalone run.

    run_all.py verifies the source, loaded library and probe before forwarding
    this manifest. Recheck the executable identity here rather than copying an
    unrelated historical commit into a newly generated individual report.
    """
    location = os.environ.get('MODSECURITY_JSON_BUILD_MANIFEST')
    if not location:
        return None
    manifest_path = Path(location).resolve(strict=True)
    manifest = json.loads(manifest_path.read_text())
    reference = manifest.get('original_commit')
    executable = Path(probe).resolve(strict=True)
    record = manifest.get('probes', {}).get(executable.name, {})
    if (manifest.get('status') != 'PASS' or
            not isinstance(reference, str) or
            not re.fullmatch(r'[0-9a-f]{40}', reference) or
            executable.parent != manifest_path.parent or
            hashlib.sha256(executable.read_bytes()).hexdigest() != record.get('sha256')):
        raise ValueError('Build manifest does not identify this verified probe')
    return reference
