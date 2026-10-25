# Memory, ownership, and lifecycle questions

Trace creation, storage, borrowing, ownership transfer, cleanup, and callback use for every object touched by the selected path. Read declarations, implementations, and actual callers together.

- Do engine/ruleset objects outlive transactions that borrow their pointers?
- Are buffers valid for their explicit lengths or required null termination?
- Is cleanup matched to allocation, including `new`/`delete`, `strdup`/`free`, smart pointers, and API cleanup helpers?
- Can an error or early return skip cleanup, repeat it, or leave a retained reference to expired storage?
- Does callback data remain valid during the call, and is a copy needed afterward?
- Can an overflow, narrowing conversion, signedness change, or stale range alter an allocation or access?

Do not infer ownership from `const` or a raw pointer type alone. Assess suspected effects under actual supported caller behavior and record the legitimate control.

Entry points: [API boundary](../components/api-boundary.md), [C++ guide](../../cpp-guide.md), and [failure/recovery](runtime-failure-recovery.md). Sanitizer use is optional and separately evidenced.
