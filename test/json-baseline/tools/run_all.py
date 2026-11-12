#!/usr/bin/env python3
"""Run every selected v3 suite; unmet requirements remain ordinary failures."""
import argparse
from datetime import datetime, timezone
import json
import os
from pathlib import Path
import re
import subprocess
import sys

sys.dont_write_bytecode = True
from build import (Blocked, ORIGINAL, PROBES, TESTS, active_canary,
                   linked_library, runtime_environment, sha256, source_metadata)

STATUSES = ('PASS', 'FAIL', 'SKIP', 'BLOCKED', 'XFAIL')


def zeros():
    return {status: 0 for status in STATUSES}


def verify_manifest(directory):
    try:
        manifest = json.loads((directory / 'build-manifest.json').read_text())
    except (OSError, ValueError) as exc:
        raise Blocked('Readable build-manifest.json is required') from exc
    if manifest.get('status') != 'PASS' or manifest.get('original_commit') != ORIGINAL:
        raise Blocked('Probe build did not establish the pinned original baseline')
    metadata = source_metadata(Path(manifest['source_root']))
    library = Path(manifest['library']['path'])
    if sha256(library) != manifest['library']['sha256']:
        raise Blocked('libModSecurity changed after the recorded build')
    environment = runtime_environment(Path(manifest['library']['directory']))
    loaders = {}
    for name, filename in PROBES.items():
        executable = directory / name
        record = manifest['probes'].get(name, {})
        if not executable.is_file() or sha256(executable) != record.get('sha256'):
            raise Blocked('Probe missing or changed after build: ' + name)
        if sha256(TESTS / filename) != record.get('source_sha256'):
            raise Blocked('Test source changed after probe build: ' + filename)
        loaders[name] = linked_library(executable, library, environment)
        if loaders[name]['sha256'] != manifest['library']['sha256']:
            raise Blocked('Loaded library hash differs: ' + name)
    active_canary(directory / 'probe', environment)
    return manifest, metadata, environment, loaders


def validated_counts(report):
    counts = report.get('counts')
    if not isinstance(counts, dict) or set(counts) != set(STATUSES):
        raise ValueError('report lacks a complete count summary')
    if any(type(number) is not int or number < 0 for number in counts.values()):
        raise ValueError('report contains invalid counts')
    entries = report.get('cases', []) + report.get('sensitivity', [])
    if not entries:
        raise ValueError('report contains no actual cases or assertion controls')
    actual = zeros()
    for entry in entries:
        actual[entry['status']] += 1
    if actual != counts:
        raise ValueError('summary counts do not match individual report records')
    if counts['XFAIL']:
        raise ValueError('this package does not mask requirements with XFAIL')
    return counts.copy()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--probes', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True,
                        help='new directory for logs and reports')
    parser.add_argument('--profile', choices=('characterization', 'requirements', 'all'),
                        default='all')
    options = parser.parse_args()
    directory, output = options.probes.resolve(), options.output.resolve()
    if options.output.is_symlink() or output.exists():
        parser.error('--output must name a new directory')
    # Source-tree and test-tree writes are not allowed, including native logs.
    if output.is_relative_to(TESTS.parents[1]):
        parser.error('--output must be outside the test repository')
    try:
        source_path = json.loads((directory / 'build-manifest.json').read_text()).get('source_root')
        if source_path and output.is_relative_to(Path(source_path).resolve()):
            parser.error('--output must be outside the ModSecurity source tree')
    except (OSError, ValueError):
        pass  # Report an absent/malformed manifest as a blocked suite below.
    output.mkdir(parents=True)
    summary = {'schema': 1, 'profile': options.profile, 'original_commit': ORIGINAL,
               'created_utc': datetime.now(timezone.utc).isoformat(),
               'counts': zeros(), 'suites': [],
               'count_scope': 'Individual case/assertion-control counts; a suite-level BLOCKED/FAIL is explicitly labelled when no valid case report exists.'}
    scripts = []
    if options.profile in ('characterization', 'all'):
        scripts += [('request-baseline', 'run.py', ['--sensitivity']),
                    ('request-expanded', 'run_extended.py', ['--sensitivity']),
                    ('audit-baseline', 'run_audit.py', []),
                    ('audit-expanded-characterization', 'run_extended_audit.py',
                     ['--suite', 'characterization']),
                    ('native-json', None, [])]
    if options.profile in ('requirements', 'all'):
        scripts += [('request-requirements', 'run_requirements.py', []),
                    ('audit-requirements', 'run_extended_audit.py',
                     ['--suite', 'requirements'])]
    try:
        manifest, metadata, environment, loaders = verify_manifest(directory)
        source = Path(metadata['source_root'])
        if output.is_relative_to(source):
            raise Blocked('--output must be outside the ModSecurity source tree')
        summary['build_manifest'] = manifest
        summary['runtime_verification'] = {'source': metadata, 'loaders': loaders,
                                           'active_json_canary': True}
        preflight = None
    except (Blocked, FileNotFoundError, KeyError, OSError, ValueError) as exc:
        preflight = ('BLOCKED', str(exc))
    except (RuntimeError, subprocess.TimeoutExpired) as exc:
        preflight = ('FAIL', str(exc))
    for name, script, extra in scripts:
        record = {'name': name, 'counts': zeros(), 'report': name + '.json',
                  'stdout': name + '.stdout.log', 'stderr': name + '.stderr.log'}
        if preflight:
            status, reason = preflight
            record.update(status=status, reason=reason,
                          count_scope='suite; cases not executed')
            record['counts'][status] = 1
        else:
            report_path = output / record['report']
            if script is None:
                command = [str(source / 'test/regression_tests'),
                           str(TESTS.parent / 'test-cases/regression/request-body-parser-json.json')]
                cwd = output / 'native-work'
                cwd.mkdir()
            else:
                command = [sys.executable, str(TESTS / script)]
                cwd = output
                if script == 'run_audit.py':
                    command += [str(directory / 'audit-probe'), '--report', str(report_path)]
                elif script == 'run_extended_audit.py':
                    command += [str(directory / 'extended-audit-probe'), '--report', str(report_path)]
                else:
                    command += ['--probe', str(directory / 'probe'), '--output', str(report_path)]
                command += extra
            record['command'] = command
            try:
                if script is None:
                    regression = source / 'test/.libs/regression_tests'
                    record['loader'] = linked_library(regression, Path(manifest['library']['path']), environment)
                with (output / record['stdout']).open('w') as stdout, \
                     (output / record['stderr']).open('w') as stderr:
                    result = subprocess.run(command, cwd=cwd, env=environment,
                        stdout=stdout, stderr=stderr, timeout=180, check=False)
                record['exit'] = result.returncode
                if result.returncode == 127:
                    diagnostic = (output / record['stderr']).read_text()
                    if 'error while loading shared libraries' in diagnostic:
                        raise Blocked('Runtime shared-library dependency unavailable: ' + diagnostic[-1000:])
                if script is None:
                    text = re.sub(r'\x1b\[[0-9;]*m', '', (output / record['stdout']).read_text())
                    if 'JSON support was not enabled' in text:
                        raise Blocked('Native runner lacks JSON support')
                    match = re.search(r'Ran a total of:\s*(\d+) regression tests - '
                        r'(All tests passed\.|(\d+) failed\.)\s*(\d+) skipped test\(s\)\.\s*'
                        r'(\d+) disabled test\(s\)\.', text)
                    if not match:
                        raise ValueError('Native regression summary missing or malformed')
                    total, failed, skipped, disabled = (int(match[1]), int(match[3] or 0),
                                                        int(match[4]), int(match[5]))
                    fixture = Path(command[1])
                    cases = json.loads(fixture.read_text())
                    expected = sum(bool(case.get('enabled', 1)) for case in cases)
                    counts = zeros()
                    counts.update(PASS=total-failed, FAIL=failed, SKIP=skipped, BLOCKED=disabled)
                    if total + skipped + disabled != expected:
                        raise ValueError('Native runner did not account for every enabled fixture')
                    native = {'counts': counts, 'enabled_cases': expected,
                              'fixture_sha256': sha256(fixture),
                              'disabled_are_blocked': True}
                    report_path.write_text(json.dumps(native, indent=2) + '\n')
                    record['counts'] = counts
                else:
                    try:
                        content = report_path.read_text()
                    except OSError as exc:
                        raise ValueError('Executed runner produced no readable case report') from exc
                    record['counts'] = validated_counts(json.loads(content))
                counts = record['counts']
                expected_exit = counts['FAIL'] if script is None else (1 if counts['FAIL'] or counts['BLOCKED'] else 0)
                if result.returncode < 0 or result.returncode != expected_exit:
                    raise RuntimeError('Runner crashed or returned an unexpected status: ' + str(result.returncode))
                if result.returncode == 0 and (counts['FAIL'] or counts['BLOCKED']):
                    raise RuntimeError('Runner returned success despite failed/blocked cases')
                if result.returncode and not (counts['FAIL'] or counts['BLOCKED']):
                    raise RuntimeError('Runner failed without a failed/blocked case report')
                record['status'] = 'FAIL' if counts['FAIL'] else 'BLOCKED' if counts['BLOCKED'] else 'PASS'
            except (Blocked, FileNotFoundError) as exc:
                record.update(status='BLOCKED', reason=str(exc), count_scope='suite infrastructure')
                record['counts']['BLOCKED'] += 1
            except (OSError, ValueError, KeyError, TypeError, RuntimeError, subprocess.TimeoutExpired) as exc:
                record.update(status='FAIL', reason=str(exc), count_scope='suite infrastructure plus any validated case counts')
                record['counts']['FAIL'] += 1
        summary['suites'].append(record)
        for status in STATUSES:
            summary['counts'][status] += record['counts'][status]
        print(record['status'], name, json.dumps(record['counts']), record.get('reason', ''))
    summary['exit'] = int(bool(summary['counts']['FAIL'] or summary['counts']['BLOCKED']))
    (output / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary['counts']))
    return summary['exit']


if __name__ == '__main__':
    sys.exit(main())
