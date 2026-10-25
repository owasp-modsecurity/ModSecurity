# Optional component security assessment

## Trigger and mode

Use for a user-requested security review of one core component. The assessment itself is analysis; implement changes only when that is also part of the authorized task.

## Read

1. [Shared deep-review procedure](../security/core-deep-assessment.md).
2. The selected [component guide](../security/components/README.md).
3. Only its relevant check modules and any additional module required by the actual data/control path.
4. [Security policy](../security-policy.md), [protocol/build boundaries](../protocol-and-hardening.md), and the affected implementation/callers/tests.

## Scope

Start with one component and follow its concrete dependencies. Record why shared engine, parser, API, storage, or build code is included. Submodule internals and external callers remain separate source boundaries; do not claim review of material not read.

Identify controlled inputs, privileges, intended controls, legitimate behavior, and strongest counterevidence. Validate plausible candidates with the most suitable available method; no scanner is presumed installed or mandatory.

## Output

Report the declared scope, exact revision/configuration, reviewed paths, executed checks, candidates and their dispositions, evidence limits, and remaining work. Separate recommendations from actual fixes and verified outcomes.

Completion describes the reconciled assessment scope, not proof that the component is free of defects or approved for production.
