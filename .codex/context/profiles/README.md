# Optional assessment profiles

Select a profile only when it matches the user's requested task. These files are reusable plans, not completed reviews, mandatory startup scans, or claims that tools and runtimes are available.

| Profile | Select when the requested goal is |
| --- | --- |
| [Core library](core-library-assessment.md) | Exercise a bounded library behavior through actual build/test/API evidence. |
| [All components](all-components-assessment.md) | Review coverage across the six core areas without implying a deep security scan. |
| [Component security](component-security-assessment.md) | Investigate security questions in one selected component and its reachable dependencies. |
| [All-component security](all-components-security-assessment.md) | Conduct a requested deep review across the core areas. |
| [Build, CI, and supply chain](build-ci-supply-chain-assessment.md) | Check build identity, dependencies, workflows, and their evidence. |
| [Evidence integrity](evidence-integrity-assessment.md) | Determine whether existing claims are supported and correctly bound. |
| [Full repository](full-repository-assessment.md) | Assess the declared repository-wide scope and expose coverage gaps. |
| [Response phases](response-phase-assessment.md) | Check core response inspection, phase conditions, and intervention semantics. |
| [Strict completeness](strict-completeness-assessment.md) | Apply an explicitly requested execution-completeness overlay to another profile. |

Use the [component index](../security/components/README.md) for source entry points and focused checks. Assessment scope does not by itself authorize source fixes, dependency changes, remote publication, or external connector operations.

Read [security policy](../security-policy.md) for candidate/result handling and [protocol/build boundaries](../protocol-and-hardening.md) before making integration or diagnostic-build claims.
