# Resources and storage

## Scope

This policy governs task-owned build, analysis, and temporary data. It does not
assume a particular machine directory, exported environment variable, storage
budget service, or numeric quota. Discover the actual workspace permissions
and capacity when the task needs substantial resources.

Keep maintained source and documentation in the repository. Put transient
downloads, bulky logs, compiler databases, scanner output, and runtime data in
an appropriate task-owned location supported by the active environment. Record
the location and retention needs without embedding machine-specific paths in
the repository's general instructions.

## Respect the actual build layout

The checked-in `.gitignore` lists common Autotools outputs, libraries, test
executables, and the Windows build directory. `build.sh` rewrites
`src/headers.mk` and creates/removes build intermediates. These are concrete
in-checkout effects to account for when preparing a build.
Sources: [ignored build products][ignore], [bootstrap operations][bootstrap].

An ignored path is not proof that its contents are disposable or task-owned.
Inspect it before cleanup. Do not claim that an arbitrary relocated build
directory is supported without checking the actual build instructions and
paths. When isolation is useful, use a task-owned checkout or worktree that
preserves the supported source layout.

Follow [build-and-test.md](build-and-test.md) for installation prefixes. Avoid
system-wide installation for ordinary source investigation when a local build
or writable task-specific prefix satisfies the task.

## Capacity and concurrency

Before a large build, download, or runtime assessment, inspect free disk space
and available memory. Estimate output and retained evidence, choose a sensible
job count, and avoid oversubscribing memory or disk. Do not use every CPU merely
because it is available, or set a global `MAKEFLAGS` that changes unrelated work.

Compiler parallelism, multiple builds, background processes, and assistant
agents consume different resources. Account for their combined load. If work
grows beyond the available capacity, reduce concurrency or stop it cleanly
while preserving useful results.

Run independent jobs concurrently only when their build files, generated
configuration, caches that require exclusive use, temporary files, logs, and
other mutable outputs do not conflict. Reconfiguring the same checkout under
one build while another compiles it is not independent work.

Do not delete another task's files, silently relocate data into a constrained
filesystem, or install tools globally to avoid a resource limitation.

## Submodule boundaries

The core repository declares `others/libinjection`, `others/mbedtls`,
`test/test-cases/secrules-language-tests`, and `bindings/python` as submodules.
Treat their working directories, commits, and generated data as separate
ownership scopes. The mbedTLS prerequisite check refers to the nested
`tf-psa-crypto/utilities/base64.c` path.
Sources: [submodule declarations][submodules], [dependency checks][dependencies].

Do not substitute the reference connector repository's Framework/MRTS layout
or storage variables for these actual dependencies. Dependency initialization
and updates follow the task's scope; a resource policy does not authorize a
gitlink change or an unrequested dependency upgrade.

## Retention and cleanup

Track which temporary directories, processes, and evidence belong to this task.
Retain enough evidence to explain the result, and remove only data that is no
longer needed and whose ownership is clear. Follow
[cleanup-and-restoration.md](cleanup-and-restoration.md) for worktrees and
repository state. If a retained artifact is unavailable to the next session,
say so instead of treating a local path as durable shared evidence.

[ignore]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.gitignore
[bootstrap]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build.sh#L1-L33
[submodules]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.gitmodules#L1-L12
[dependencies]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L65-L107
