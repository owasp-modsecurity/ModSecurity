#!/usr/bin/env python3
"""Build probes against a verified pinned baseline or an explicit CI commit."""
import argparse
from datetime import datetime, timezone
import hashlib
import json
import os
from pathlib import Path
import re
import shlex
import shutil
import subprocess
import sys

ORIGINAL = '1ebdc390b47ab033c8267130ce83130f235aed20'
FORK = 'https://github.com/Easton97-Jens/ModSecurity.git'
UPSTREAM = 'https://github.com/owasp-modsecurity/ModSecurity.git'
SUBMODULES = ('others/libinjection', 'others/mbedtls',
              'test/test-cases/secrules-language-tests')
PROBES = {'probe': 'probe.cc', 'audit-probe': 'audit_probe.cc',
          'extended-audit-probe': 'extended_audit_probe.cc'}
TESTS = Path(__file__).resolve().parent.parent


class Blocked(RuntimeError):
    """A required tool, dependency or trustworthy source is unavailable."""


def sha256(path):
    digest = hashlib.sha256()
    with Path(path).open('rb') as stream:
        for part in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(part)
    return digest.hexdigest()


def capture(command, *, cwd=None, env=None, timeout=30):
    try:
        result = subprocess.run(command, cwd=cwd, env=env, text=True,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                timeout=timeout, check=False)
    except FileNotFoundError as exc:
        raise Blocked('Required executable missing: ' + str(command[0])) from exc
    if result.returncode:
        raise Blocked('Command failed: ' + str(command[0]) + ': ' +
                      result.stderr[-2000:].strip())
    return result.stdout.strip()


def git(source, *arguments):
    return capture(['git', '-C', str(source), *arguments])


def untracked_files(source):
    """List non-ignored additions without splitting quoted or unusual paths."""
    return [name for name in git(source, 'ls-files', '--others',
                                '--exclude-standard', '-z', '--').split('\0')
            if name]


def source_metadata(source, source_ref=ORIGINAL):
    """Verify the working tree and gitlinks against the selected immutable ref.

    The default retains pinned-baseline verification. CI passes its checkout
    SHA explicitly, so committed production changes can be tested without
    accepting uncommitted production changes or changing historical evidence.
    """
    source = Path(source).resolve(strict=True)
    if not isinstance(source_ref, str) or not re.fullmatch(r'[0-9a-f]{40}', source_ref):
        raise Blocked('Source reference must be a full lowercase commit SHA')
    if Path(git(source, 'rev-parse', '--show-toplevel')).resolve() != source:
        raise Blocked('--source must name the ModSecurity repository root')
    if git(source, 'rev-parse', '--verify', source_ref + '^{commit}') != source_ref:
        raise Blocked('Selected source commit is unavailable')
    changes = git(source, 'diff', '--name-only', source_ref, '--').splitlines()

    def ancillary(path):
        return (path.startswith(('test/', 'doc/', '.github/')) or
                path.startswith('README') or path == '.gitignore')

    unexpected = [path for path in changes if not ancillary(path)]
    additions = [path for path in untracked_files(source) if not ancillary(path)]
    if additions:
        raise Blocked('Untracked production/build sources: ' + ', '.join(additions))
    if unexpected:
        raise Blocked('Production/build tree differs from selected commit: ' +
                      ', '.join(unexpected))
    modules = {}
    for relative in SUBMODULES:
        entry = git(source, 'ls-tree', source_ref, '--', relative).split()
        if len(entry) < 3 or entry[0] != '160000':
            raise Blocked('Selected submodule gitlink missing: ' + relative)
        expected = entry[2]
        actual = git(source / relative, 'rev-parse', 'HEAD')
        if actual != expected:
            raise Blocked('Submodule is not at its selected gitlink: ' + relative)
        if git(source / relative, 'diff', '--name-only', 'HEAD', '--'):
            raise Blocked('Submodule has modified tracked sources: ' + relative)
        if untracked_files(source / relative):
            raise Blocked('Submodule has untracked sources: ' + relative)
        modules[relative] = actual
    recursive = git(source, 'submodule', 'status', '--recursive', '--', *SUBMODULES)
    for line in recursive.splitlines():
        match = re.match(r'^([ +U-]?)([0-9a-f]{40}) (\S+)', line)
        if not match or match[1] not in ('', ' '):
            raise Blocked('A recursive submodule is uninitialized or differs from its gitlink')
        if git(source / match[3], 'diff', '--name-only', 'HEAD', '--'):
            raise Blocked('Recursive submodule has modified tracked sources: ' + match[3])
        if untracked_files(source / match[3]):
            raise Blocked('Recursive submodule has untracked sources: ' + match[3])
        modules[match[3]] = match[2]
    for relative in ('src', 'headers', 'configure.ac', 'test/regression/regression.cc'):
        if not (source / relative).exists():
            raise Blocked('Required source missing: ' + relative)
    return {'source_root': str(source), 'original_commit': source_ref,
            'source_head': git(source, 'rev-parse', 'HEAD'),
            'production_matches_original': True, 'submodules': modules,
            'ancillary_differences': changes}


def runtime_environment(library_dir):
    environment = os.environ.copy()
    previous = environment.get('LD_LIBRARY_PATH', '')
    environment['LD_LIBRARY_PATH'] = str(library_dir) + (':' + previous if previous else '')
    environment['PYTHONDONTWRITEBYTECODE'] = '1'
    return environment


def linked_library(executable, library, environment):
    """Fail if an installed libModSecurity shadows the selected source build."""
    output = capture(['ldd', str(executable)], env=environment)
    missing = [line.strip() for line in output.splitlines() if 'not found' in line]
    if missing:
        raise Blocked('Unresolved runtime dependencies: ' + '; '.join(missing))
    matches = re.findall(r'^\s*libmodsecurity\.so[^\s]*\s+=>\s+(.+?)\s+\(', output, re.M)
    if len(matches) != 1:
        raise Blocked('Cannot establish exactly one dynamic libModSecurity dependency')
    resolved = Path(matches[0]).resolve(strict=True)
    if resolved != Path(library).resolve(strict=True):
        raise Blocked('Executable resolves a different libModSecurity: ' + str(resolved))
    return {'resolved_path': str(resolved), 'sha256': sha256(resolved)}


def active_canary(probe, environment):
    rules = ('SecRuleEngine On\nSecRequestBodyAccess On\n'
             'SecAuditEngine Off\nSecAction "id:1,phase:1,pass,nolog,'
             'ctl:requestBodyProcessor=JSON"\n')
    payload = '\n'.join(('transaction', rules.encode().hex(), b'{"x":1}'.hex(),
                         b'/'.hex(), b'application/json'.hex(), '')) + '\n'
    result = subprocess.run([str(probe)], input=payload, text=True,
                            capture_output=True, env=environment, timeout=15,
                            check=False)
    if result.returncode < 0:
        raise RuntimeError('JSON canary probe crashed: signal ' + str(-result.returncode))
    if result.returncode == 77:
        raise Blocked('Actual linked library did not pass the active JSON canary')
    if result.returncode:
        raise RuntimeError('JSON canary probe failed: exit ' + str(result.returncode))
    records = [line.split('\t') for line in result.stdout.splitlines()
               if line.split('\t', 1)[0] == 'YAJL_ACTIVE']
    if records != [['YAJL_ACTIVE', '1']]:
        raise Blocked('Actual linked library did not pass the active JSON canary')
    return True


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--source-ref', default=ORIGINAL,
                        help='full source commit SHA; defaults to the pinned baseline')
    parser.add_argument('--output', type=Path, required=True,
                        help='new directory outside the source tree')
    options = parser.parse_args()
    output = options.output.resolve()
    source = options.source.resolve()
    if (options.output.is_symlink() or output.exists() or output.is_relative_to(source)
            or output.is_relative_to(TESTS.parents[1])):
        parser.error('--output must be a new directory outside both source repositories')
    output.mkdir(parents=True)
    manifest = {'schema': 1, 'status': 'BLOCKED',
                'created_utc': datetime.now(timezone.utc).isoformat(),
                'probe_sources_root': str(TESTS), 'original_commit': None,
                'requested_source_ref': options.source_ref}
    try:
        manifest.update(source_metadata(source, options.source_ref))
        library = (source / 'src/.libs/libmodsecurity.so').resolve(strict=True)
        manifest['library'] = {'path': str(library), 'sha256': sha256(library),
                               'directory': str(source / 'src/.libs')}
        compiler = shlex.split(os.environ.get('CXX', 'c++'))
        if not compiler or not shutil.which(compiler[0]):
            raise Blocked('C++ compiler unavailable')
        manifest['compiler'] = capture([*compiler, '--version']).splitlines()[0]
        cppflags = shlex.split(os.environ.get('CPPFLAGS', ''))
        ldflags = shlex.split(os.environ.get('LDFLAGS', ''))
        cflags = shlex.split(capture(['pkg-config', '--cflags', 'yajl']))
        yajl_libs = shlex.split(capture(['pkg-config', '--libs', 'yajl']))
        include = capture(['pkg-config', '--variable=includedir', 'yajl'])
        manifest['yajl_version'] = capture(['pkg-config', '--modversion', 'yajl'])
        manifest['build_options'] = {'standard': 'c++17', 'WITH_YAJL': True,
                                    'extra_cppflag_count': len(cppflags),
                                    'extra_ldflag_count': len(ldflags)}
        environment = runtime_environment(source / 'src/.libs')
        manifest['probes'] = {}
        for name, filename in PROBES.items():
            input_file = TESTS / filename
            if not input_file.is_file():
                raise Blocked('Test source unavailable: ' + filename)
            command = [*compiler, '-std=c++17', '-DWITH_YAJL', *cppflags, *cflags,
                       '-I' + str(Path(include).parent), '-I' + str(source / 'headers'),
                       '-I' + str(source), str(input_file),
                       '-L' + str(source / 'src/.libs'), *ldflags,
                       '-Wl,-rpath,' + str(source / 'src/.libs'), '-lmodsecurity',
                       *yajl_libs, '-o', str(output / name)]
            with (output / (name + '-build.log')).open('w') as log:
                result = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT,
                                        timeout=180, check=False)
            if result.returncode:
                raise RuntimeError('Probe compilation failed: ' + name +
                                   '; see ' + name + '-build.log')
            loader = linked_library(output / name, library, environment)
            manifest['probes'][name] = {'path': str(output / name),
                'sha256': sha256(output / name), 'source_file': filename,
                'source_sha256': sha256(input_file), 'loader': loader}
        manifest['active_json_canary'] = active_canary(output / 'probe', environment)
        manifest['status'] = 'PASS'
        status = 0
    except (Blocked, FileNotFoundError) as exc:
        manifest['reason'] = str(exc)
        status = 2
    except (RuntimeError, OSError, subprocess.TimeoutExpired, ValueError) as exc:
        manifest.update(status='FAIL', reason=str(exc))
        status = 1
    (output / 'build-manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    print(manifest['status'], manifest.get('reason', 'three probes built; active YAJL verified'))
    return status


if __name__ == '__main__':
    sys.exit(main())
