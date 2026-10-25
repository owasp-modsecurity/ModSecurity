# Findings and remediation policy

Use durable finding records for significant defects, unresolved blockers, or
evidence gaps that another session must understand. A routine small correction
can be explained in the task record without creating a separate finding system.
This policy does not turn every task into a security assessment.

## Separate observations from conclusions

Distinguish a reported symptom, a source observation, a reproducible defect,
an unverified hypothesis, and a missing test result. A static warning or a
suspicious name is an investigation lead, not automatically a confirmed bug or
security vulnerability.

For each independently remediable cause, retain the useful subset of:

- a stable local identifier and concise description;
- affected repository, source revision, paths, and symbols;
- expected behavior and the observed difference;
- configuration, inputs, reproduction, or source reasoning;
- impact and confidence, with uncertainty stated separately;
- ownership, status, and an observable acceptance criterion;
- the proposed next step and validation needed for closure.

Use [backlog.md](backlog.md) for outstanding items and link to a bounded record
in [audit sessions](../audit/sessions/) when the detail is substantial.
Use [fact-check.md](fact-check.md) for documentation/source discrepancies.
Do not invent a finding identifier that appears to come from an external tracker.

## Evidence and ownership

Tie observations to the exact source and actual executed commands. Retain only
the diagnostic content needed to reproduce or explain them, with secrets and
sensitive payloads removed. Avoid whole environment dumps or unreviewed large
artifacts.

Keep core-library evidence, a submodule's evidence, and an external connector's
evidence distinct under
[architecture-and-boundaries.md](architecture-and-boundaries.md). A failure
observed through a connector does not by itself locate the defect in this
library. A core-only test does not prove the connector's end-to-end behavior.

## Remediation states

A practical record can move from **open** through **investigated**, **fixed**,
and **verified**, with **blocked**, **not reproduced**, or **out of scope** used
when those descriptions fit. These are descriptions of evidence, not mandatory
ceremonial steps.

- **Fixed** means the relevant change exists; identify its files or revision.
- **Verified** means the acceptance criterion was checked with appropriate
  evidence, including the original failure and legitimate behavior where feasible.
- **Not reproduced** means the attempted reproduction did not establish the
  reported defect under the stated conditions. It is not proof of absence.
- **Accepted risk** requires an explicit decision by the responsible user or
  owner, including one already recorded in the active task; an assistant must
  not invent acceptance on their behalf.

A commit, push, PR, merge, or unrelated passing check does not automatically
close a finding. If source review supports a fix but execution remains missing,
state that distinction instead of describing runtime verification as complete.

## Scope of follow-up

Fix task-owned findings within the authorized scope using
[change-workflow.md](change-workflow.md). Record unrelated concerns without
silently expanding the task or modifying a dependency. Broaden investigation
only when needed to establish the requested result or when the user requests it.

External issues, review replies, security reports, or other messages require
the user's authorization for that communication. Preserve any authorization
already granted; do not publish sensitive findings as a routine side effect
of maintaining local context.
