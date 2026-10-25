# Optional core security review guides

These documents are reusable review instructions, not findings or completed scans. Use them only for the task selected by the user. They introduce no default security-scan requirement.

The organization is adapted from the [connector reference's security index](https://github.com/Easton97-Jens/ModSecurity-conector/blob/42fd5a8e895aca3a36b09eb09fe5c7cf9c3399e3/.codex/context/security/README.md#L1-L23). That repository supplies the design pattern; core-library behavior must be established from this repository's own pinned sources.

## Reading order

1. Choose an [assessment profile](../profiles/README.md) and record its scope.
2. For a requested deep review, read the [shared core assessment procedure](core-deep-assessment.md).
3. Select the relevant [component guides](components/README.md).
4. Load only the check modules linked by the selected components and questions.
5. Use the [security policy](../security-policy.md) and [protocol/build boundary guide](../protocol-and-hardening.md) to keep evidence and scope explicit.

## Shared guidance

| Guide | Use |
| --- | --- |
| [Core deep assessment](core-deep-assessment.md) | Plan a source-backed investigation and validate candidates. |
| [Analysis services](analysis-services-policy.md) | Reconcile actually available external analysis when requested. |
| [Quality scoring](quality-scoring-policy.md) | Define an optional evidence-based score before scored work. |
| [Components](components/README.md) | Identify library boundaries and relevant checks. |

## Evidence boundary

A result belongs to its component, source revision, caller, configuration, dependency/backend choices, and executed cases. Share evidence across components only when the report identifies the common implementation and explains why the same result covers each claim. A library build, test definition, generated report, or rule match does not establish a live connector outcome.

The component set is a navigation aid, not proof that every file or possible input has been reviewed. Missing evidence remains visible.
