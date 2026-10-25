# Updating the official v3 baseline deliberately

This is the chosen maintenance workflow for this documentation baseline.
It does not update itself. The initially examined baseline is pinned in
[upstream-lock.json](upstream-lock.json); historical provenance and the
branch comparison are recorded in [provenance.md](provenance.md).

## 1. Preserve and understand the existing state first

When an upstream update is requested, first read `AGENTS.md`,
`current-state.md` and the recorded decisions. Then inspect the working tree
and remotes:

```sh
git status --short
git branch --show-current
git rev-parse HEAD
python3 .codex/bin/verify-context.py
```

Preserve existing changes. Do not automatically reset to a new branch tip.
Use the [redacted remote-identity check](git-policy.md#inspect-remote-identities-without-credentials)
to verify which configured remote fetches from `owasp-modsecurity/ModSecurity`.
Do not print raw URLs or assume that a remote's name establishes its destination.
The working branch and push destination come from the current authorized task;
historical fork and PR references do not select them.

## 2. Identify a new official baseline

After verifying the remote, the following fetch is an intentional network operation.
Replace the placeholder with that existing remote's name. This example assumes
its fetch refspec updates the usual `refs/remotes/<name>/v3/master` reference;
for a different mapping, use and verify the actual refspec's destination instead.

```sh
modsecurity_source_remote='<verified-remote-name>'
git fetch -- "$modsecurity_source_remote"
git rev-parse --verify "refs/remotes/${modsecurity_source_remote}/v3/master^{commit}"
```

Use the previous commit ID from `upstream-lock.json` and the actual
new commit ID returned to read `git log` and `git diff` between
those exact revisions. First check whether the old revision is an ancestor
of the new one. A different relationship needs an explanation and must not
be treated as an ordinary forward update.

The name `v3/master` is a moving reference. The new full commit ID becomes
the documentation baseline only after it has been examined. A version macro
in a header or a README statement alone proves neither a release nor
tested compatibility.

## 3. Review the code update and documentation together

How the code is brought into the working branch depends on the requested
Git workflow and existing changes. Afterwards:

1. Read changed public headers, implementations, grammar, build files,
   submodule pins and tests.
2. Revise affected technical chapters against the new code. Pay particular
   attention to claims about phases, ownership, optional features and tests.
3. Point source links to verified lines at the new commit. Do not simply
   replace the old SHA string throughout the documentation.
4. Explicitly account for deleted or renamed files and changed submodules
   in the inventory and explanations.
5. Keep historical session notes tied to their original baseline. Refer to
   the revised technical chapters for current facts.

## 4. Regenerate the provenance data

`upstream-lock.json` contains the commit, tree, observation date, inventory
counts and submodule pins. These values come from the actual Git history,
not an estimated version number. `source-index.tsv` has the columns
`mode`, `type`, `object`, `path` and contains every entry in the new baseline
commit as read with `git ls-tree -r`.

The Python sequence below regenerates **only the inventory**, after the
new verified commit has already been deliberately entered in `upstream-lock.json`.
It does not automatically update explanations, metadata or submodules.

Run it from the repository root only when regenerating this inventory is part of
the authorized update and this task owns the file. First coordinate with any other
writer. The existing target must be a regular tracked file with no staged or
unstaged changes. If it contains earlier work, preserve and resolve that work
within its actual scope; do not reset, stash, or commit it merely to bypass this
guard. The snippet checks again before replacement, but does not lock out another
writer, so exclusive task ownership is still required.

```sh
python3 - <<'PY'
import json
import re
import subprocess
from pathlib import Path

target = Path('.codex/audit/source-index.tsv')

def git_bytes(*args):
    try:
        result = subprocess.run(
            ['git', '--no-lazy-fetch', '--no-optional-locks', '-c', 'core.fsmonitor=false', *args],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=False)
    except OSError:
        raise SystemExit('Inventory not replaced: Git could not be started.')
    if result.returncode:
        raise SystemExit('Inventory not replaced: a required local Git check failed.')
    return result.stdout

if git_bytes('rev-parse', '--show-prefix').strip():
    raise SystemExit('Inventory not replaced: run from the repository root.')

def clean_target():
    if (target.is_symlink() or not target.is_file()
            or target.resolve() != Path.cwd() / target):
        raise SystemExit('Inventory not replaced: target must be a regular repository file.')
    if git_bytes('status', '--porcelain=v1', '--untracked-files=all', '--', str(target)).strip():
        raise SystemExit('Inventory not replaced: preserve existing staged, unstaged, or untracked work.')
    current = target.read_bytes()
    if (current != git_bytes('show', 'HEAD:' + target.as_posix())
            or current != git_bytes('show', ':' + target.as_posix())):
        raise SystemExit('Inventory not replaced: target must match both HEAD and the index.')
    return current

original = clean_target()
lock = json.loads(Path('.codex/context/upstream-lock.json').read_text(encoding='utf-8'))
commit = lock['upstream']['commit']
if not isinstance(commit, str) or not re.fullmatch(r'[0-9a-f]{40}', commit):
    raise SystemExit('Inventory not replaced: the baseline must be a full verified commit ID.')
raw = git_bytes('ls-tree', '-r', '-z', commit)
rows = ['mode\ttype\tobject\tpath\n']
for item in raw.split(b'\0'):
    if item:
        metadata, path = item.decode('utf-8').split('\t', 1)
        mode, kind, oid = metadata.split()
        rows.append(f'{mode}\t{kind}\t{oid}\t{path}\n')
if clean_target() != original:
    raise SystemExit('Inventory not replaced: target changed during generation.')
target.write_text(''.join(rows), encoding='utf-8')
PY
```

This maintenance command writes the specified file. The ordinary
`verify-context.py` run only reads files.

## 5. Run the final checks and hand off the result

The context check must pass with the updated data. If product changes are
intentional, the normal run reports the differences; `--strict-source`
is appropriate only when everything outside the context files is expected
to match the pinned official source exactly.

Then actually run the build and regression tests appropriate to the code
change from [build-and-test.md](build-and-test.md), and record their scope.
Update `current-state.md`, decisions and open tasks. Do not present a new
source baseline as freshly verified using test results from an older
revision.
