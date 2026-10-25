# Optional evidence-based quality scoring

Use only when the user asks for a score. This is a proposed assessment convention, not an upstream quality standard or a completed rating.

## Define the score first

Before execution, record the scope/revision, criteria, observable acceptance conditions, weights, required evidence, and justified exclusions. Do not remove failed or unevidenced conditions afterward to improve the score.

A possible 100-point scheme uses four areas of 25 points each. Select five concrete criteria worth up to five points per area:

| Area | Candidate criteria to make concrete for the selected task |
| --- | --- |
| API and lifecycle | Public interface compatibility; ownership; lengths/bounds; error/cleanup contracts; callback/shared-state behavior. |
| Rules and data processing | Parser acceptance; phase/action semantics; transformations; body-processor conditions; configured limit/error handling. |
| Build, CI, and dependencies | Normal build; optional-feature identity; dependency provenance; workflow input/permission boundaries; artifact identity. |
| Evidence and maintainability | Meaningful cases/controls; revision binding; declared coverage; reproducible commands; explained unresolved work. |

These are categories for planning, not claims that each has been assessed. Define measurable conditions appropriate to the requested scope before assigning points.

## Calculation

For criterion `i`, use `points_i = weight_i * satisfied_conditions / applicable_conditions`. Missing evidence for an applicable condition earns no points; it does not automatically establish a product defect.

Calculate `score = 100 * earned_points / applicable_max_points`. Remove a condition from the denominator only with a technical `not_applicable` justification. Missing tools, access, or time are coverage gaps. With no applicable denominator, report no percentage.

If implementing a score calculator, validate empty/duplicate criteria, all satisfied, none satisfied, partial satisfaction, justified exclusions, missing evidence, and rounding near 100%. Do not publish a calculation as executed unless it was actually run.

## Report separately

Keep the assessment score, executed test counts, test success, measured code coverage, scanner/Quality-Gate outcomes, findings, and integration evidence separate. A score of 100 means only that the predeclared applicable conditions have evidence; it does not mean universal security or production readiness.
