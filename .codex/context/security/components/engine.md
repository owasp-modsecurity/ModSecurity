# Engine and rule-execution review

## Source entry points

Read `src/rules_set.cc`, `src/rule_with_operator.cc`, `src/rule_with_actions.cc`, and the relevant transaction phase methods. `RulesSet::evaluate()` selects a phase collection and handles control flow such as skip/allow and disruptive interruption; disruptive actions in `executeAction()` are conditional on `EnabledRuleEngine`. Sources: [phase loop](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rules_set.cc#L121-L232), [rule evaluation](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_operator.cc#L199-L353), [action guard](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_actions.cc#L299-L324).

## Review questions

- Which variable values, exclusions, transformations, operators, and chain members can the selected input reach?
- What happens before a complete chain match, after a match, after a failed condition, or after an intervention?
- Which engine state applies, and does the proposed expectation distinguish match, action, and intervention?
- Are per-transaction variables and shared collection state separated along the actual path?
- Does an exception, early return, skipped rule, or limit preserve the documented state and a legitimate follow-up case?

Do not infer rule execution from a successful phase-method return. `processRequestHeaders()` returns successfully when the engine is disabled; response headers and logging also have disabled-engine guards. Sources: [request headers](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L461-L471), [response headers](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L992-L1097), [logging](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1304-L1346).

## Select relevant checks

[Rule/control bypass questions](../checks/modsecurity-bypass.md), [API sequencing](../checks/api-sequencing.md), [memory lifecycle](../checks/memory-lifecycle.md), [concurrency](../checks/concurrency-races.md), [resource bounds](../checks/resource-exhaustion.md), and [finding validation](../checks/finding-validation.md).

Record the exact rules, engine state, phase, input, expected control, observed result, and limitations. A core-library result does not prove connector enforcement.
