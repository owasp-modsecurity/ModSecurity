#!/usr/bin/env python3
"""Fetch a fresh fork clone and check out the immutable official v3 source."""
import argparse
from datetime import datetime, timezone
import json
from pathlib import Path
import subprocess
import sys

sys.dont_write_bytecode = True
from build import Blocked, FORK, ORIGINAL, SUBMODULES, UPSTREAM, source_metadata


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--destination', type=Path, required=True,
                        help='new, absent directory; never reuses a checkout')
    parser.add_argument('--build', action='store_true',
                        help='bootstrap/configure/make with installed Debian dependencies')
    options = parser.parse_args()
    destination = options.destination.resolve()
    if options.destination.is_symlink() or destination.exists():
        parser.error('--destination already exists; choose a new directory')
    destination.parent.mkdir(parents=True, exist_ok=True)
    # Logs live beside the new clone, never inside its versioned source tree.
    logs = destination.parent / (destination.name + '-preparation')
    if logs.exists():
        parser.error('preparation log directory already exists: ' + str(logs))
    logs.mkdir()
    manifest = {'schema': 1, 'status': 'BLOCKED', 'fork': FORK, 'upstream': UPSTREAM,
                'original_commit': ORIGINAL, 'destination': str(destination),
                'created_utc': datetime.now(timezone.utc).isoformat(), 'steps': []}

    def run(name, command, cwd=None, timeout=180):
        with (logs / (name + '.log')).open('w') as log:
            result = subprocess.run(command, cwd=cwd, stdout=log,
                                    stderr=subprocess.STDOUT, timeout=timeout, check=False)
        manifest['steps'].append({'name': name, 'exit': result.returncode,
                                  'log': name + '.log'})
        if result.returncode:
            raise Blocked(name + ' failed; see ' + str(logs / (name + '.log')))

    try:
        run('clone-fork', ['git', 'clone', '--no-checkout', '--no-tags', FORK, str(destination)])
        run('add-upstream', ['git', 'remote', 'add', 'upstream', UPSTREAM], destination)
        run('fetch-original', ['git', 'fetch', '--no-tags', 'upstream', ORIGINAL], destination)
        run('checkout-original', ['git', 'switch', '--detach', ORIGINAL], destination)
        run('pinned-submodules', ['git', 'submodule', 'update', '--init', '--recursive',
                                 '--', *SUBMODULES], destination, 600)
        manifest.update(source_metadata(destination))
        if options.build:
            run('bootstrap', ['sh', '-e', './build.sh'], destination, 300)
            run('configure', ['./configure', '--enable-assertions=yes',
                '--enable-parser-generation=no', '--without-geoip', '--with-yajl',
                '--with-pcre2', '--with-curl', '--with-libxml', '--with-lua',
                '--with-lmdb', '--with-maxmind', '--with-ssdeep=/usr'], destination, 300)
            run('make', ['make', '-j2'], destination, 1800)
            manifest.update(source_metadata(destination))
        manifest['status'] = 'PASS'
        status = 0
    except (Blocked, FileNotFoundError, OSError, subprocess.TimeoutExpired) as exc:
        manifest['reason'] = str(exc)
        status = 2
    (logs / 'preparation-manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    print(manifest['status'], manifest.get('reason', 'pinned original prepared at ' + str(destination)))
    return status


if __name__ == '__main__':
    sys.exit(main())
