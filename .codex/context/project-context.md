# Project context: understanding and working with ModSecurity v3

## Reliable basis

This introduction describes the official ModSecurity source revision **`effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`** on `v3/master`. The sources at the end are pinned to that commit. This explanation provides a starting point; for a concrete change, read the affected implementations next.

`AGENTS.md`, `CODEX_START.md`, and `.codex/` are working conventions added to this repository. They explain the code and record work results. They are not features of the upstream library.

## 1. What ModSecurity is for

ModSecurity v3 provides **libmodsecurity**, a library for inspecting HTTP data using rules. The code can load rules, accept request and response data, evaluate conditions, and produce messages and an intervention state. The central build target is `libmodsecurity.la`. Sources: [project description][scope], [library target][build], [transaction API][transaction].

Distinguish three components:

| Component | Role | Where this distinction appears |
| --- | --- | --- |
| **Library** | Processes supplied data according to the loaded ruleset. | `Transaction`, `RulesSet`, and their implementations. [Sources][transaction] [Rule evaluation][rules-evaluate] |
| **Connector or calling application** | Supplies HTTP data to the library and responds to its results in the relevant server or application context. | The API receives data as parameters; upstream describes connectors as separate projects. [Sources][scope] [API example][example] |
| **Ruleset and configuration** | Determines the variables, conditions, actions, phases, and settings to use. | SecLang grammar and `modsecurity.conf-recommended`. [Sources][grammar] [Configuration][config] |

A library change, a new rule, and a web server connector change are therefore different tasks. The repository's example configuration and an external ruleset must also be considered separately; the sources described here establish the contents of this library and its included files. Further reading: [architecture](architecture.md) and [rules and configuration](configuration-and-rules.md).

## 2. Which languages matter here

The library is built with **C++17**: `configure.ac` explicitly requires that standard, and the Windows CMake build sets it as well. Classes such as `modsecurity::Transaction` and `modsecurity::RulesSet` are declared in headers under `headers/modsecurity/`; their implementations are mostly under `src/`. Sources: [C++17 with Autotools][cxx17], [C++17 on Windows][cxx17-win], [public API][transaction].

There is also a **C API** with functions such as `msc_init()`, `msc_new_transaction()`, and `msc_process_request_body()`. These functions delegate to C++ objects. A program can use the C interface without calling the C++ classes directly. This does not imply complete equivalence between every capability of the two interfaces. Sources: [C entry point][c-engine], [transaction wrappers][c-transaction], [C declarations][c-api].

**SecLang, or SecRules**, is the input language for rules and directives. Its scanner and grammar sources are `.ll` and `.yy` files, which are distinct from C++ implementation code. Shell, JSON, Python, Lua, and build formats appear in other areas. The exact mapping is in the [repository map](repository-map.md). Sources for the rule language: [parser build][parser-build], [grammar][grammar].

The public header contains the version macros `3`, `0`, and `17`, together `3.0.17`. This is information in the inspected source code. On its own, it establishes neither a published release version for this commit nor the version of an installed library. Source: [version macros][version].

## 3. The three core classes

| Class | Plain explanation | Concrete responsibilities in the code |
| --- | --- | --- |
| `ModSecurity` | Shared library context. | Creates collection backends and holds connector information and the logging callback. [Constructor][engine], [API][engine-api] |
| `RulesSet` | Loaded rules and their settings. | `loadFromUri()` loads a file; `load()` loads rule text; `evaluate()` evaluates the rules for a phase. [Loading][load], [evaluation][rules-evaluate] |
| `Transaction` | State of a single HTTP transaction. | Accepts connection data, URI, headers, and bodies; holds variables, messages, and interventions. [API][transaction], [lifetime][lifetime] |

The caller supplies the engine and ruleset to the transaction. The transaction stores the pointers but does not delete the two objects itself. Those objects must remain valid while they are used. The existing chunked-body example destroys the transaction first, then the ruleset, and finally the engine. Sources: [constructor and destructor][lifetime], [example][example]. The [C++/C guide](cpp-guide.md) explains the precise ownership and cleanup rules.

## 4. Inputs and results handled by the library

| Input | Typical API entry point | Meaning |
| --- | --- | --- |
| Rules and directives | `RulesSet::loadFromUri()` or `load()` | Builds the ruleset and its settings. [Source][load] |
| Connection and request line | `processConnection()` and `processURI()` | Addresses, ports, URI, HTTP method, and protocol version. [Source][transaction] |
| Request/response headers | `addRequestHeader()` and `addResponseHeader()` | Supplies header names and values. [Source][transaction] |
| Request/response body | `appendRequestBody()` and `appendResponseBody()` | Supplies data buffers with explicit lengths; size limits can already take effect at this point. [Request][append], [response][response-body] |

| Result | What it tells you | Limit of that conclusion |
| --- | --- | --- |
| Loading result and parser errors | Whether the loading path could take in rules and which errors it reports. | It does not test a later HTTP transaction. [Source][load] |
| Transaction variables and match data | Which data and matches evaluation determines or stores. | Their meaning depends on the phase and evaluation path. [Source][rule-evaluate] |
| Intervention | A state the caller must query, containing a status, URL, log text, and disruptive flag, among other fields. | The caller must implement the actual HTTP response to that state. [Source][intervention] |
| Rule messages, debug output, and audit output | Information about matches, processing, or selected transactions. | The output paths have different configurations and conditions. [Logging][logging], [callback][callback] |

A successful method call is therefore not automatically a decision to allow a request. For example, in its reject branch, `appendRequestBody()` can set an intervention and still return `true`. Interpret the return value and intervention according to their respective contracts. Source: [request body limit path][append].

## 5. Typical transaction sequence

The existing chunked-body example shows a full API sequence. The following order summarizes it in plain terms; an actual connector may stop further HTTP processing after an intervention. Source: [example program][example].

1. Create the library context and ruleset; load rules and check for loading errors.
2. Create a transaction using those objects; supply connection data, URI, and request headers.
3. Execute the request header phase; check the intervention state.
4. Supply request body data; invoke body processing and the request body phase; check for an intervention.
5. Perform the corresponding steps for response headers and the response body while the application continues processing.
6. Complete logging and release transaction resources.

The methods have conditions and early returns. For example, `processLogging()` skips both the logging phase and audit selection when the engine is disabled. Request and response bodies have their own access, format, and limit conditions. Sources: [logging][logging], [request processing][request-body], [response processing][response-body].

The numbering also requires attention: SecRules phases and the internal C++ enum do not align because the enum also contains `UriPhase`. The `Phase` action class translates the rule specification; `processURI()` populates data without calling `RulesSet::evaluate(UriPhase, ...)`. The full table is in [architecture.md](architecture.md). Sources: [enum][phases], [translation][phase-action], [URI processing][uri].

## 6. What "evaluating a rule" means in the code

A rule can select data through variables, transform those data, test a condition with an operator, and execute actions. `RuleWithOperator::evaluate()` connects these steps. It also accounts for exclusions and rule chains. Sources: [rule evaluation][rule-evaluate], [transformations][transformations].

An operator tests the specific condition; a transformation changes the comparison value. Actions can, for example, add a message, set a value, or request an intervention. For rule chains, actions before and after a complete chain match must be considered separately. Sources: [rule evaluation][rule-evaluate], [action control][actions].

The included configuration starts with `SecRuleEngine DetectionOnly`. In `executeAction()`, disruptive actions run only with `EnabledRuleEngine`. A match or the presence of a `deny` action alone therefore does not prove that a request was actually blocked. Sources: [configuration][config], [engine condition for actions][actions].

## 7. Learning effectively and preparing changes

| Goal | Recommended starting point | What to read next in the code |
| --- | --- | --- |
| Get an overview | [Repository map](repository-map.md) | Root build files, public headers, and relevant subdirectories. |
| Understand HTTP data flow | [Architecture](architecture.md) | `src/transaction.cc` and an existing API example. |
| Map C++ and C correctly | [C++/C guide](cpp-guide.md) | Signature, implementation, callers, and cleanup path together. |
| Understand a rule or directive | [Configuration and rules](configuration-and-rules.md) | Scanner, grammar, and concrete operator/action implementation. |
| Locate a defect or feature | [Feature map](feature-map.md) | The implementations and existing tests mapped there. |
| Verify your own build | [Build and tests](build-and-test.md) | `configure.ac`, the platform build, and actual test output. |

This reading sequence is a recommendation for this working repository. Before changing code, record the desired observable behavior, then identify the relevant files and existing checks. Finding a symbol or an existing test case does not replace an executed check of the behavior.

## 8. How new chats continue existing work

The working convention is to read `AGENTS.md`, the [documentation index](../README.md), and the recorded current work state first, then open the chapters and sources relevant to the task. At the end, hand over results, evidence, checks actually performed, and remaining steps in a traceable form. The [session workflow](session-workflow.md) and [handoff template](handoff-template.md) describe this process.

These files deliberately provide readable project context. They guarantee neither automatic memory across chats nor access to files unavailable in a new session. Unknown facts remain unresolved until the relevant sources have been read or checks have actually been performed.

## Sources at the pinned commit

[scope]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L21-L46
[build]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/Makefile.am#L11-L14
[transaction]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L335-L419
[rules-evaluate]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rules_set.cc#L121-L234
[example]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/examples/using_bodies_in_chunks/simple_request.cc#L95-L282
[grammar]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L934-L1218
[config]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/modsecurity.conf-recommended#L1-L76
[cxx17]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L49-L63
[cxx17-win]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/CMakeLists.txt#L102-L103
[c-engine]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L462-L482
[c-transaction]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1804-L1904
[c-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L677-L767
[parser-build]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/Makefile.am#L4-L43
[version]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/modsecurity.h#L191-L201
[engine-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/modsecurity.h#L275-L313
[engine]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L62-L109
[load]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rules_set.cc#L50-L110
[lifetime]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L113-L182
[append]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L929-L971
[rule-evaluate]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_operator.cc#L199-L360
[intervention]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1350-L1390
[logging]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1304-L1346
[callback]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L193-L212
[request-body]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L674-L870
[response-body]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1117-L1230
[phases]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/modsecurity.h#L100-L171
[phase-action]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/phase.cc#L25-L72
[uri]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L337-L443
[transformations]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_actions.cc#L329-L441
[actions]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_actions.cc#L202-L325
