# Optional security review policy

## When to use this material

This is a working convention for an explicitly requested security assessment, or for bounded security questions that are relevant to an authorized change. It does not require a security scan for every task, prescribe an installed scanner, or record a completed assessment. Ordinary documentation work does not activate the full security workflow.

Select a [profile](profiles/README.md), identify the actual code and configuration in scope, and use only the relevant [component guides](security/components/README.md) and check modules. An assessment task does not by itself authorize product fixes, dependency upgrades, deployment, or changes to external repositories. An existing implementation request retains its own authorized scope.

## Establish the question before the conclusion

Record the intended invariant, the input controlled by a caller or attacker, its required privileges, the affected asset, the trust boundary, and the observable consequence. Read the implementation and its callers before deciding whether an input is untrusted or a control is missing. A suspicious pattern, advisory, or scanner message is a candidate, not a validated vulnerability.

For selected behavior, compare a relevant negative case with a legitimate control. Keep source reasoning, executed results, limitations, and proposed hardening separate. Use [finding validation](security/checks/finding-validation.md) for a candidate that warrants deeper investigation.

## Library scope

libmodsecurity accepts data through its public API; web server connectors are separate projects. Library rule matching, an intervention request, connector enforcement, and a client-visible outcome are distinct evidence boundaries. Sources: [upstream scope](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L21-L46), [transaction API](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L335-L419), [intervention implementation](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1350-L1389).

Do not import connector-specific host routes, transport modes, private IPC protocols, release gates, or Phase-4 migration targets into the core library's current behavior. [Protocol and hardening](protocol-and-hardening.md) explains the integration boundary.

## Evidence and result handling

Bind every observation to the source revision, component, effective configuration, optional features, toolchain, and actual command or source path. Record unperformed checks as `not_run` and unavailable prerequisites as `blocked`, with reasons. A failed prerequisite is not an executed failure of the target test. Use `not_applicable` only with a technical scope explanation.

Retain bounded, redacted evidence. Do not commit credentials, private keys, sensitive request bodies, unfiltered environments, or private customer logs. For a validated issue, state impact, reachability, counterevidence, confidence, and remaining limitations; do not invent a severity, CVE, exploit, or universal safety guarantee.

Conclude with the achieved review coverage and unresolved work. No generic score, completed review, or clean scanner output constitutes production approval.
