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
git remote -v
git rev-parse HEAD
python3 .codex/bin/verify-context.py
```

Preserve existing changes. Do not automatically reset to a new branch tip.
Check that `upstream` actually points to the official repository.
Do not assume the remote name merely because this guide uses it.

## 2. Identify a new official baseline

After verifying the remote, the following fetch is an intentional
network operation:

```sh
git fetch upstream
git rev-parse upstream/v3/master
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

```sh
python3 - <<'PY'
import json
import subprocess
from pathlib import Path

lock = json.loads(Path('.codex/context/upstream-lock.json').read_text(encoding='utf-8'))
commit = lock['upstream']['commit']
raw = subprocess.check_output(['git', '--no-lazy-fetch', 'ls-tree', '-r', '-z', commit])
rows = ['mode\ttype\tobject\tpath\n']
for item in raw.split(b'\0'):
    if item:
        metadata, path = item.decode('utf-8').split('\t', 1)
        mode, kind, oid = metadata.split()
        rows.append(f'{mode}\t{kind}\t{oid}\t{path}\n')
Path('.codex/audit/source-index.tsv').write_text(''.join(rows), encoding='utf-8')
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
