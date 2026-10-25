# Change workflow

Use this policy for core-library features, fixes, refactors, configuration,
build changes, and substantial documentation changes. The amount of planning
and verification should match the requested change.

## Establish the change

1. Read the active task and relevant saved context under
   [task-workflow.md](task-workflow.md).
2. Establish current behavior from the affected source, callers, and tests.
   Separate the user's desired behavior from what the code currently does.
3. Identify ownership using
   [architecture-and-boundaries.md](architecture-and-boundaries.md).
4. Identify materially affected interfaces, rule syntax, defaults, data handling,
   error paths, build options, compatibility, and resource use.
5. Define an observable acceptance criterion and a proportionate check before
   editing. A short explanation is sufficient for a small change.

Use the current source and [build-and-test.md](build-and-test.md) to discover
real test programs and commands. Do not invent a target or apply a command from
another repository merely because the task looks similar.

## Features and fixes

For a feature, explain relevant valid, invalid, and omitted inputs; defaults;
error behavior; and compatibility consequences. Implement the complete scoped
behavior. Do not conceal an unfinished path behind a success return or an
undocumented fallback.

For a reported defect, keep the report separate from the verified observation.
Reproduce it where practical, identify the responsible implementation, and
check a legitimate input that must continue to work. A focused regression that
fails before the fix and passes afterwards is useful when it exercises the
actual failure. If execution is unavailable, provide the strongest source-based
analysis and state what remains unverified.

Documentation corrections normally need source and reference checks, not a new
C++ test. Do not add tests that merely restate the implementation or broaden a
small edit into an unrelated test campaign.

## Core-specific implementation checks

| Change area | Questions to resolve from the affected code |
| --- | --- |
| C++/C interface or object state | Who owns each object and buffer? Which callers, cleanup paths, exported declarations, or layouts are affected? |
| Transaction processing | Which phase, configuration, body-access setting, error path, and intervention path actually executes? |
| Rule syntax, actions, operators, or variables | Does the parser accept the intended form? Where are defaults and evaluation semantics defined? Which existing tests cover it? |
| Parser inputs | How do the `.ll`/`.yy` change and tracked generated files stay consistent under the repository's generation process? |
| Optional dependencies or build options | What happens with the feature enabled and disabled? Which tests are built or skipped in that configuration? |
| Logging and retained data | Does the change affect content, ownership, lifetime, output errors, or unintended disclosure? |

The public core interfaces are in
[modsecurity.h](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/modsecurity.h),
[rules_set.h](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/rules_set.h), and
[transaction.h](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h).
The baseline requires C++17 in
[configure.ac](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L63).
Parser source/generated-file relationships are recorded in
[src/Makefile.am](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/Makefile.am#L17-L28).
These are investigation entry points, not universal ownership or safety guarantees.

Follow [`.editorconfig`](../../.editorconfig) and neighboring code. Keep the
change cohesive, preserve unrelated work, and avoid incidental reformatting or
dependency updates. Do not weaken checks or hide errors to obtain a passing run.

## Verify and hand over

Run the narrowest meaningful checks for the affected behavior and any required
repository gates that apply. For a fix, include the original reproduction and
the relevant control where feasible. Broaden testing only to resolve a concrete
remaining risk or satisfy an applicable requirement.

Record the exact checks run, their outcomes, relevant configuration, and missing
coverage. Keep source reasoning, runtime evidence, and inference separate.
Update the affected explanation and durable task state using
[documentation-and-traceability.md](documentation-and-traceability.md).
Apply [definition-of-done.md](definition-of-done.md) before reporting the result.
