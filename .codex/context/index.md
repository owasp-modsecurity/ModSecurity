# ModSecurity context index

The root `AGENTS.md` is a reusable agreement for any repository. First read
[repository-context.md](repository-context.md) for this checkout's identity,
source constraints, startup checks, and file-language requirements. Then read
[current-state.md](current-state.md), [decisions.md](decisions.md), and
[upstream-lock.json](upstream-lock.json), followed by only the guides and policies
relevant to the task. The routing is recorded in [manifest.toml](manifest.toml).
These `.codex` files remain specific to the ModSecurity core project.

## Technical guides: learn the actual repository

| Question | Primary guide |
| --- | --- |
| What does libmodsecurity provide? | [Project context](project-context.md) |
| Where are source, languages, builds, tests, and utilities? | [Repository map](repository-map.md) |
| How do transactions, rules, phases, and interventions interact? | [Architecture](architecture.md) |
| How do C++17, ownership, callbacks, and the C API work here? | [C++ guide](cpp-guide.md) |
| Where is a feature implemented and tested? | [Feature map](feature-map.md) |
| How do SecRules, configuration, operators, and transformations connect? | [Configuration and rules](configuration-and-rules.md) |
| How do I prepare, build, and test this checkout? | [Build and test](build-and-test.md) |
| Which comments/examples or apparent features need caution? | [Fact check](fact-check.md) |
| Which source revisions and limits support these explanations? | [Provenance](provenance.md) |
| How do I adopt a newer upstream revision? | [Upstream sync](upstream-sync.md) |
| How do I select the right checkout or pull-request head? | [Working with this repository](using-this-fork.md) |

The technical guides cite immutable official ModSecurity source. An inventory
entry is not a claim that every function was reviewed. Read actual affected code
before changing it.

## Working policies: one primary owner per decision

| Task or decision | Primary owner |
| --- | --- |
| Instruction conflicts, product authority, current user intent | [Policy precedence](policy-precedence.md) |
| Task scope, requirements, assumptions, proportional planning | [Task workflow](task-workflow.md) |
| Missing prerequisites and feasibility | [Feasibility](feasibility-policy.md) |
| Core, connectors, dependencies, and source ownership | [Architecture and boundaries](architecture-and-boundaries.md) |
| Feature, fix, refactor, behavior change | [Change workflow](change-workflow.md) |
| Commands and actual tool availability | [Command execution](command-execution-policy.md) |
| Optional RTK summaries and Codex setup | [RTK guide](rtk.md), supporting the command policy |
| Checks, test selection, execution evidence | [Testing and evidence](testing-and-evidence.md) |
| CPU, memory, build output, and storage | [Resources and storage](resources-and-storage.md) |
| Languages, native build files, generated inputs | [Tooling and language](tooling-and-language.md) |
| Project-local Python helpers | [Python policy](python-policy.md) |
| Git state, branches, commits, remotes | [Git policy](git-policy.md) |
| Push, PR, and observed CI results | [Delivery and CI](delivery-and-ci.md) |
| Review feedback and failed CI | [PR remediation](pr-remediation.md) |
| Integration into `v3/master` | [Master integration](master-integration.md) |
| Task-owned temporary data and cleanup | [Cleanup and restoration](cleanup-and-restoration.md) |
| English documentation, citations, handovers | [Documentation and traceability](documentation-and-traceability.md) |
| Parallel work and background processes | [Subagents and background](subagents-and-background.md) |
| Security-sensitive tasks | [Security policy](security-policy.md) |
| Isolated runtime prerequisites for a security task | [Security runtime preflight](security-runtime-preflight.md) |
| Library input boundaries and targeted hardening | [Protocol and hardening](protocol-and-hardening.md) |
| Observations, confirmed findings, remediation | [Findings policy](findings-policy.md) |
| Optional SonarQube use when actually configured | [SonarQube policy](sonarqube-policy.md) |
| Completion and honest delivery | [Definition of done](definition-of-done.md) |

## Assessment profiles: load only for the requested assessment

| Request | Profile |
| --- | --- |
| Focused core-library behavior review | [Core library](profiles/core-library-assessment.md) |
| Functional coverage across core components | [All components](profiles/all-components-assessment.md) |
| Security review of a selected component | [Component security](profiles/component-security-assessment.md) |
| Security review across core components | [All components security](profiles/all-components-security-assessment.md) |
| Build, CI, dependencies, and supply chain | [Build and CI](profiles/build-ci-supply-chain-assessment.md) |
| Claims, citations, results, and evidence integrity | [Evidence integrity](profiles/evidence-integrity-assessment.md) |
| Explicit full repository assessment | [Full repository](profiles/full-repository-assessment.md) |
| Response phase and intervention behavior | [Response phase](profiles/response-phase-assessment.md) |
| Strict completeness for explicitly selected coverage | [Completeness overlay](profiles/strict-completeness-assessment.md) |

The [profiles index](profiles/README.md) and [security index](security/README.md)
route to core [component guides](security/components/README.md) and focused
`security/checks/` modules. Their presence does not report completed scans or
create an obligation to run every assessment for each edit.

## Durable session context

| File | Use |
| --- | --- |
| [Current state](current-state.md) | Actual progress, checked revision, results, and next step |
| [Decisions](decisions.md) | Durable choices with reasons and evidence |
| [Backlog](backlog.md) | Real remaining tasks and unknowns |
| [Session workflow](session-workflow.md) | Explain, change, verify, hand over |
| [Handoff template](handoff-template.md) | Record a reproducible closeout |
| [Session records](../audit/sessions/README.md) | Dated history, outside normal policy routing |
| [Start prompt](prompts/start-new-chat.md) | Copy into a new chat with repository access |
| [End prompt](prompts/end-session.md) | Finish with saved context |
| [Template adaptation](template-adaptation.md) | What was reused, adapted, or intentionally omitted |

For first-time learning: project context → repository map → architecture → C++
guide → one concrete implementation and its tests. For a defect: current state →
feature map → implementation and callers → a relevant regression → build guide.

## Validation and historical material

See [bin/README.md](../bin/README.md) for the local validators and
[audit/README.md](../audit/README.md) for inventories. History is evidence for its
stated revision, not a replacement for current source. The manifests are read by
these helpers; they are not a built-in Codex configuration mechanism.
