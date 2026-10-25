# Rules and parser review

## Source entry points

`RulesSet::loadFromUri()` uses `Parser::Driver` and merges parsed rules/properties. `Driver::addSecRule()` assigns rules and checks chain/ID constraints. Sources: [loading](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rules_set.cc#L50-L109), [driver parsing](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/driver.cc#L128-L198), [rule construction](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/driver.cc#L76-L124).

Read the relevant `.ll`/`.yy` input and concrete action/operator implementation. The parser Makefile identifies generated files; a token or class name alone does not establish accepted and implemented behavior. Sources: [parser build](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/Makefile.am#L1-L43), [explicitly unsupported grammar example](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L2726-L2729).

## Review questions

- Who can supply the rule text, include path, remote source, or configuration value in this scenario?
- Does token recognition lead to rejection, a stored property, rule construction, or an empty action?
- Are quoting, escapes, macros, lengths, numeric ranges, duplicate IDs, and chain constraints handled consistently?
- What state is retained after a failed load or merge, and what does the caller do with the return/error text?
- Does an accepted setting reach the runtime branch whose behavior is being claimed?

Distinguish SecRules numbers from internal enum values: `phase:1` maps to `RequestHeadersPhase`, whose internal value is `2`. `processURI()` populates data without evaluating `UriPhase`. Sources: [phase action](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/phase.cc#L25-L72), [enum](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/modsecurity.h#L97-L170), [URI processing](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L337-L443).

## Select relevant checks

[Configuration/input injection](../checks/config-injection.md), [memory lifecycle](../checks/memory-lifecycle.md), [resource bounds](../checks/resource-exhaustion.md), [rule/control bypass questions](../checks/modsecurity-bypass.md), [file/storage boundaries](../checks/filesystem-storage.md), and [finding validation](../checks/finding-validation.md).

Use matching accepted/rejected cases and verify their actual execution. Preserve the distinction between a configuration-author privilege and an unauthenticated HTTP client's control.
