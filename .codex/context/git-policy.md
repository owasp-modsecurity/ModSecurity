# Git policy for ModSecurity core

This policy supports reviewable work in the actual checkout. It does not replace
the user's task instructions or authorization already established in the
conversation. Use [master-integration.md](master-integration.md) for merging
into the target branch.

## Repository and branch identity

| Role | Repository | Branch selection |
| --- | --- | --- |
| Canonical project and upstream PR base | `owasp-modsecurity/ModSecurity` | `v3/master` |
| Working checkout and PR head | Resolve from the current task and actual checkout/PR metadata | Verify the current head; do not assume a historical context branch |
| Push destination | Resolve the authorized repository and effective push URL | Use the task's authorized head branch; a fork may host it without becoming the PR base |

The source baseline is separately recorded in
[upstream-lock.json](upstream-lock.json). A branch name is not a fixed source
revision. See [using-this-fork.md](using-this-fork.md) and
[provenance.md](provenance.md) for context.

Before editing or delivering, inspect the relevant repository state:

```sh
git status --short
git branch --show-current
git rev-parse HEAD
```

Inspect remote identities with the redacted check below. A previous fork, branch,
or PR recorded in the history is not the default destination for a new task.

Use the established working branch for this task. For later independent work,
choose a suitable task branch or isolated worktree when it helps preserve other
work. Do not make a new worktree or branch a mandatory ritual. Do not switch,
reset, or recreate the user's active branch merely because a generic guide
suggests a different starting point.

## Inspect remote identities without credentials

Do not print raw remote URLs, Git configuration dumps, or environment variables.
The following Python 3 example reads configured remote names and Git's effective
fetch/push URLs, including URL rewriting. It captures raw output and errors before
display, removes URL user-info, query strings, and fragments, and hides local
paths and URL forms it cannot recognize. Only recognized network hosts and simple
two-component repository paths are shown; do not treat hidden output as a match.
It neither contacts a remote nor changes configuration. If Python is unavailable,
use an equivalent local redaction method rather than installing it for this check.

```sh
python3 - <<'PY'
import json
import re
import subprocess
from urllib.parse import urlsplit

def git_output(*args):
    try:
        result = subprocess.run(
            ['git', '--no-optional-locks', '-c', 'core.fsmonitor=false', *args],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            encoding='utf-8', errors='replace', check=False)
    except OSError:
        raise SystemExit('Remote inspection failed; raw details withheld.')
    if result.returncode:
        raise SystemExit('Remote inspection failed; raw output withheld.')
    return result.stdout

def identity(raw):
    hidden = '[local or nonstandard URL hidden]'
    if (any(ord(c) <= 32 or ord(c) == 127 for c in raw)
            or re.match(r'^[A-Za-z]:[\\/]', raw)):
        return hidden
    try:
        if '://' in raw:
            parsed = urlsplit(raw)
            if parsed.scheme not in {'https', 'http', 'ssh', 'git', 'ftp', 'ftps'}:
                return hidden
            host, port, path = parsed.hostname, parsed.port, parsed.path
            if not host or not re.fullmatch(r'[A-Za-z0-9.:-]+', host):
                return hidden
            host = '[' + host + ']' if ':' in host else host
            prefix = parsed.scheme + '://' + host + (':' + str(port) if port is not None else '')
        else:
            match = re.fullmatch(
                r'(?:[^/@]+@)?(\[[0-9A-Fa-f:.]+\]|[A-Za-z0-9.-]+):(.+)', raw)
            if not match:
                return hidden
            prefix = 'ssh://' + match.group(1)
            path = re.split(r'[?#]', match.group(2), maxsplit=1)[0]
        path = path.strip('/')
        if not re.fullmatch(r'[A-Za-z0-9_.-]+/[A-Za-z0-9_.-]+', path):
            return hidden
        return prefix + '/' + path
    except ValueError:
        return hidden

for remote in git_output('remote').splitlines():
    for purpose, options in [('fetch', []), ('push', ['--push'])]:
        urls = git_output('remote', 'get-url', *options, '--all', '--', remote)
        print(json.dumps({'remote': remote, 'purpose': purpose,
                          'destinations': [identity(url) for url in urls.splitlines()]},
                         ensure_ascii=True))
PY
```

This intentionally limited display is not a general secret detector: do not use
it for URLs whose host or repository path itself contains confidential material.
For hidden or unusual forms, establish the intended public repository identity
without copying the raw value into logs. Never use a redacted URL as a replacement
remote setting or as proof of access, ownership, or authorization.

## Preserve work and scope

Inspect unstaged and staged changes before staging or committing. Preserve
pre-existing tracked and untracked work. Stage explicit task-owned paths, for
example `git add -- <task-files>` with the actual intended paths supplied.
Do not use broad staging in a mixed working tree.

Do not perform destructive cleanup, discard changes, rewrite published history,
force-push, or delete branches as routine housekeeping. Those actions need to
be part of a clearly authorized task and must respect repository protections.
Resolve ordinary conflicts within an authorized change when the intended
result is clear; ask when a conflict exposes an unresolved product decision.

Submodule source, history, and gitlinks remain bounded by
[architecture-and-boundaries.md](architecture-and-boundaries.md). A core commit
does not authorize publishing a change to a dependency repository.

## Remote delivery

When commit, push, or PR work is in scope, retain that authorization across the
task. Verify the actual repository identity, fetch and effective push
destinations, selected branch, and PR base before a remote write. Do not assume
that a remote named `origin` points to the authorized head repository or that one
named `upstream` has the intended meaning.

A destination mismatch requires investigation. Do not silently choose a
different repository, create a replacement repository, or rewrite remotes to
hide the mismatch. Complete the reviewable local work while any genuinely
missing destination decision is resolved.

Keep commits coherent and their messages factual. After an authorized push,
check that the intended remote branch and PR head identify the delivered
commit. A local commit or a successful command invocation alone is insufficient
evidence of the final remote state.

Creating a PR does not by itself authorize merging it. Do not bypass branch
protections, push directly to `v3/master` as a shortcut, or claim a merge before
it is observed. Record actual delivery facts in
[current-state.md](current-state.md) or the relevant
[session record](../audit/sessions/).
