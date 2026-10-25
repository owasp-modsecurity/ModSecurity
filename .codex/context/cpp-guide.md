# Understanding C++ and C in the ModSecurity source code

## Basis and purpose

This guide explains the language features and API boundaries actually used in the repository, based on `owasp-modsecurity/ModSecurity` at commit **`effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`**, branch `v3/master`. It is a guide to reading and modifying the code, not a complete C++ course or evidence of a successful build. All linked implementations are pinned to the same source revision.

For the library's processing sequence, read [architecture.md](architecture.md) first. This guide then focuses on interpreting function signatures, data ownership, return values, and existing code patterns correctly.

## 1. Which language is used where

| Area | Evidenced use | Implication for the work |
| --- | --- | --- |
| C++17 | `configure.ac` requires `AX_CXX_COMPILE_STDCXX(17, noext, mandatory)`; the Windows CMake build sets `CMAKE_CXX_STANDARD 17`. | C++17 is the evidenced baseline for language features and compiler errors. [Autotools][cxx17], [Windows][cxx17-win] |
| C++ classes and `.cc` files | `ModSecurity`, `RulesSet`, and `Transaction` are declared in public headers and implemented in `src/`. | Headers show the calling contract; implementations show the actual behavior. [Engine header][engine-api], [ruleset header][rules-api], [transaction header][transaction-api] |
| C interface | `msc_*` functions are defined with `extern "C"`; C callers receive opaque types. | C callers access the C++ objects through functions. [Opaque types][c-types], [C wrappers][c-construct] |
| SecRules configuration | `modsecurity.conf-recommended` contains `SecRuleEngine`, `SecRule`, and action lists, among other things. | This is the engine's input language; a rule change is not automatically a C++ change. [Configuration][recommended] |
| Scanner and parser | `src/parser/Makefile.am` lists `seclang-parser.yy` and `seclang-scanner.ll` as sources and generates the corresponding files. | A language change starts with the grammar or scanner and the resulting objects. [Parser build][parser-build], [driver][driver-rules] |

The C++ API uses the namespace **`modsecurity`**, as in `modsecurity::RulesSet`. The `::` qualifies the type with its namespace. One README section contains older identifiers such as `ModSecurity::Rules`; the current headers and runnable example sources use `modsecurity::RulesSet`. Do not copy those README examples as current signatures without checking them. Sources: [current declarations][rules-api], [current example][example-setup], [inconsistent README example][readme-example].

## 2. Headers, implementations, and the public interface

A header declaration describes the name, parameter types, and return type. The corresponding method body describes what actually happens during a call. For `RulesSet::loadFromUri(const char *uri)`, the declaration is in `headers/modsecurity/rules_set.h`; the body in `src/rules_set.cc` uses `Parser::Driver`, propagates parse errors, and merges the loaded data. A comment can provide a starting point but does not replace this comparison. Sources: [declaration][rules-api], [loading implementation][load].

Many headers under `headers/modsecurity/` are installed according to `src/Makefile.am`. Changes there can therefore affect library consumers. The ABI is the binary interface used by already compiled consumers; in some places, the code explicitly retains old signatures for ABI reasons, as in `AnchoredSetVariable::resolveRegularExpression()`. **Working rule for new changes:** do not treat signatures, class layout, and ownership in these headers as purely local implementation details. Sources: [installed headers][installed-headers], [ABI comment][abi].

## 3. Classes, constructors, and lifetime

`ModSecurity` and `Transaction` have constructors and destructors. A constructor establishes the initial state; a destructor releases managed resources. Both classes explicitly delete copy operations with `= delete`. For example, this prevents a normal copy from duplicating a transaction with the same resource pointers. Sources: [ModSecurity][engine-api], [Transaction][transaction-api].

An existing example of automatic object lifetime is `test/common/modsecurity_test_context.h`: the context contains `ModSecurity` and `RulesSet` as direct members; `create_transaction()` creates a transaction with pointers to those members. The method returns a newly constructed object directly. Read this code in conjunction with the required C++17 version; a deleted copy constructor is not a reason to add a copy operation to resolve an error without first investigating its cause. Sources: [test context][test-context], [C++17 requirement][cxx17].

### Who owns each resource?

The following table distinguishes actual ownership management from references alone. The lifetime requirements for borrowed pointers are inferred from constructors, destructors, and callers.

| Resource | Creation or handoff | Release or lifetime |
| --- | --- | --- |
| Engine collections | `ModSecurity` creates collection backends with `new`. | `ModSecurity::~ModSecurity()` deletes them. [Source][engine-life] |
| `Transaction::m_ms`, `m_rules` | The caller supplies the engine and ruleset; the constructor stores the pointers. | The transaction does not delete them. Both must remain valid while they are used. [Source][transaction-life] |
| Transaction JSON/XML processors | The constructor creates them depending on build macros. | The destructor deletes them under the same macros. [Source][transaction-life] |
| Transaction collection `TX` | `Collections` creates `InMemoryPerProcess("TX")`. | `Collections::~Collections()` deletes it; the other collection pointers are supplied externally. [Source][collections] |
| Rules in `Rules` | `std::vector<std::shared_ptr<Rule>>`. | The smart pointers manage the rule objects they hold. [Source][rules-storage] |
| New parser rule object | `Driver::addSecRule(std::unique_ptr<RuleWithActions>)`. | Ownership moves into `std::shared_ptr` for phase storage or a chain child. [Driver][driver-rules], [chain fields][chain-fields] |
| Operator in a `RuleWithOperator` | The constructor takes an `Operator *`. | `RuleWithOperator::~RuleWithOperator()` deletes it and the variable objects it has taken over. [Source][operator-life] |
| Transformation and action objects | `RuleWithActions` holds several pointers and pointer containers. | The destructor deletes the objects managed there. [Source][actions-life] |

`std::unique_ptr` expresses exclusive ownership; `std::shared_ptr` allows shared ownership. A raw pointer alone does not say who must release the object. The repository uses all three forms. Mechanically converting every raw pointer would therefore miss the actual ownership boundaries. First read construction, storage, handoff, and release together. The table provides evidenced examples; this recommendation is a working practice derived from them.

### `const` does not mean "never needs to be freed"

`AnchoredSetVariable::resolve()` allocates copies with `new VariableValue(...)` and places pointers to them in a `std::vector<const VariableValue *>`. Rule processing explicitly deletes these elements after evaluation. Here, `const` restricts modification of the object through that pointer; it does not describe resource ownership. `resolveFirst()` has a different contract and returns a `std::unique_ptr<std::string>`. Sources: [allocation in resolve()][resolve], [use and delete][evaluate], [resolveFirst()][resolve-first].

`const std::string&` is a reference to an existing string. For example, `VariableValue::getValue()` and `getKey()` return references to members. Such a reference cannot be used after the `VariableValue` is deleted. Keep this lifetime in mind when changing the evaluation loop. Sources: [getters and members][variable-value], [deletion sites][evaluate].

## 4. Strings, bytes, and lengths

The API distinguishes several forms of text input. `processURI()` accepts null-terminated C strings. The header overload without lengths computes sizes using `strlen()`. The overload with `key_n` and `value_n` instead passes exactly those lengths to `std::string::assign()`. C callers see the same distinction between `msc_add_request_header()` and `msc_add_n_request_header()`, with corresponding functions for response headers. Sources: [URI processing][uri], [header overloads][header-lengths], [C declarations][c-trans-api].

| Input form | Behavior in the code | Consequence for a caller |
| --- | --- | --- |
| `const char *` for URI, method, and HTTP version | Used as a C string. | Ensure valid null termination and lifetime for the call. [Source][uri] |
| Header pointer without a length | Uses `strlen()`. | Do not use it for an arbitrary network slice that is not null-terminated. [Source][header-lengths] |
| Header pointer with `size_t` lengths | Creates string values with explicit lengths. | Supply the actual valid buffer bounds. [Source][header-lengths] |
| Body pointer with a length | `appendRequestBody()` writes `len` bytes, or the remaining capacity under the limit, into the stream. | The supplied byte count governs the input; check the return value and the intervention separately. [Source][request-append] |

`reinterpret_cast<const char *>(buf)` in the append path changes the pointer's type for the stream call; it does not perform URL, Unicode, or character-set decoding. URI decoding occurs explicitly through `utils::uri_decode()` in `processURI()`. Explain changes to input interpretation in terms of the relevant processing, rather than a cast alone. Sources: [append path][request-append], [URI decoding][uri].

`processURI()` keeps both raw and derived URI data and computes offsets. `VariableValue` can contain origin details that later contribute to match references. A seemingly simple switch from a raw string to a decoded one can therefore affect comparison values, variable names, and diagnostic positions at the same time. This is a question to investigate based on the data flow, not a claim about a particular defect. Sources: [URI][uri], [VariableValue][variable-value], [match processing][evaluate].

## 5. Inheritance and virtual calls

`Rule` defines virtual `evaluate()` methods and a virtual destructor. `RuleWithActions` extends this type; `RuleWithOperator` adds the operator and target variables. This allows `RulesSet::evaluate()` to invoke the concrete evaluation through a general rule pointer. The code also uses `dynamic_cast`, for example to check whether a rule has action metadata. Sources: [Rule base class][rule-base], [RuleWithActions][chain-fields], [concrete operator][operator-life], [phase loop][rules-evaluate].

A new rule type therefore involves several separate questions: How does the parser create the object? Which phase receives it? Which virtual evaluation runs? Who owns the objects involved? A new name in the grammar alone does not answer these questions. The existing call sites are the starting point for such a change. Sources: [driver][driver-rules], [phase loop][rules-evaluate], [operator evaluation][evaluate].

## 6. Do not assign one meaning to all return values

The repository has no single universal contract for `int` return values. Read the meaning for each function individually. In particular, a successful API call does not establish that the request is allowed or that a particular rule phase actually ran.

| Function/path | Evidenced result | Common misreading |
| --- | --- | --- |
| `RulesSet::loadFromUri()`, `load()` | `-1` for the errors handled there; otherwise the number of rules taken in. [Source][load] | Treating `0` as an error in every case or confusing the count with a decision to allow a request. |
| `processRequestHeaders()` | Returns `true`/`1` even when the engine is disabled. [Source][request-phase] | Inferring from `1` that rules were executed. |
| `appendRequestBody()` | The partial branch returns `false`; a reject branch may set an intervention and still return `true`. [Source][request-append] | Checking only the return value and overlooking the intervention. |
| `Transaction::intervention()` | Returns the disruptive state; when it returns `false`, the function body does not write to the output structure. [Source][intervention] | Interpreting the return value and structure independently of each other. |
| `msc_rules_add_file()` | Passes through the loading result; on a negative result, it provides an error message allocated with `strdup`. [Source][c-rules] | Treating an error buffer as a static string. |

Read constructor wrappers carefully as well: in the inspected code, `msc_init()`, `msc_create_rules_set()`, and `msc_new_transaction()` simply use `new`. The transaction comment describes a possible `NULL` return value; however, the wrapper contains neither `new (std::nothrow)` nor a `catch` block. Do not document a general guarantee that all C++ errors are automatically translated into a C error code here. Sources: [engine creation][c-engine], [ruleset creation][c-rules], [transaction comment and wrapper][c-construct].

## 7. Mapping the C++ API to the C API

The C wrappers mostly delegate to the C++ objects. For C, the object structures are hidden; the `msc_*` functions provide access through pointers. `extern "C"` specifies C language linkage for these functions; it does not mean their implementation is written in C. Sources: [C types][c-types], [wrappers][c-engine], [transaction wrappers][c-construct].

| Task | C++ entry point | C entry point |
| --- | --- | --- |
| Create and destroy the engine | `ModSecurity`, destructor | `msc_init()`, `msc_cleanup()` |
| Identify the connector | `setConnectorInformation()` | `msc_set_connector_info()` |
| Create and destroy a ruleset | `RulesSet`, destructor | `msc_create_rules_set()`, `msc_rules_cleanup()` |
| Load a rule file | `loadFromUri()` | `msc_rules_add_file()` |
| Load rules from text | `load()` | `msc_rules_add()` |
| Create a transaction | `Transaction(ms, rules, logCbData)` | `msc_new_transaction()` |
| Supply a custom transaction ID | `Transaction(ms, rules, id, logCbData)` | `msc_new_transaction_with_id()` |
| Supply connection and URI data | `processConnection()`, `processURI()` | `msc_process_connection()`, `msc_process_uri()` |
| Append body data | `appendRequestBody()`, `appendResponseBody()` | `msc_append_request_body()`, `msc_append_response_body()` |
| Execute rule phases | `processRequestHeaders()`, `processRequestBody()`, `processResponseHeaders()`, `processResponseBody()` | Corresponding `msc_process_*` functions using C naming |
| Retrieve an intervention | `intervention()` | `msc_intervention()` |
| Complete logging | `processLogging()` | `msc_process_logging()` |
| Destroy a transaction | Destructor | `msc_transaction_cleanup()` |

Sources for the mapping: [engine declarations][engine-api], [ruleset declarations][rules-api], [C++ transaction][transaction-api], [C transaction][c-trans-api]. The table selects central calls; the headers contain additional functions. It does not claim complete functional equivalence between the C and C++ interfaces.

### Use the corresponding cleanup functions

`msc_rules_error_cleanup()` uses `free()` for the error text allocated by the rule wrappers. `msc_intervention_cleanup()` frees an intervention's buffers and resets its fields. The engine, ruleset, and transaction instead use their respective object cleanup functions, which execute `delete` internally. Do not substitute `free()` for `delete` or vice versa based on the visible pointer type. Sources: [rule errors and cleanup][c-rules], [intervention cleanup][c-cleanup], [engine cleanup][c-engine].

Another counterexample to the assumption that `const char *` always means a borrowed buffer: in the inspected code, `Transaction::getResponseBody()` returns a buffer allocated with `strdup()`, whereas `msc_who_am_i()` returns a `c_str()` pointer into the engine's string. These two return values have different storage origins. The response getter is also described in its comment as an unused skeleton; it must not be used as evidence of comprehensive response rewriting or arbitrary binary-data processing. Sources: [response getter][response-getter], [who_am_i][c-engine].

## 8. Callbacks: type and lifetime belong together

`ModSecLogCb` has the form `void (*)(void *, const void *)`. The engine passes user data and either text or a `RuleMessage`. The C++ overload `setServerLogCb(cb, properties)` determines the representation. The overload without properties selects `TextLogProperty`; `msc_set_log_cb()` uses exactly that variant. Interpret a `void *` pointer only according to the property actually selected. Sources: [callback type and properties][callback-api], [property selection and C wrapper][callback-setup].

In the text path, `ModSecurity::serverLog()` creates a local string and passes its `c_str()` during the callback. In the object path, it passes the address of a `RuleMessage`; rule processing explicitly documents its limited lifetime and copies messages for later audit use. **Derived caller rule:** copy data inside the callback if they are needed after it returns; do not retain these pointers without checking their lifetime. Sources: [callback execution][callback-call], [RuleMessage lifetime][message-life].

`logCbData` is supplied when the transaction is created. The test context uses the address of its `std::stringstream`; the callback converts it back to the appropriate type and writes the message into it. This concrete example shows why the callback function, context type, and context lifetime must agree. Sources: [test context][test-context], [transaction constructor][transaction-life].

## 9. Preprocessor and build variants

Macros determine which implementations are included in the code. Examples are `WITH_LMDB` for selecting collection backends, `WITH_LIBXML2` and `WITH_YAJL` for body processors, and `NO_LOGS` for debug macros. Reading a source path successfully does not prove that it is included in a particular binary. For platform-specific or feature-specific questions, examine the build configuration and actual compiler definitions. Sources: [engine selection][engine-life], [processor selection][transaction-life], [debug macros][c-types].

The SecRules grammar is also more than a list of intended features. It includes explicitly unsupported actions, such as `ACTION_APPEND` with `ACTION_NOT_SUPPORTED("Append", ...)`. Finding a token or name in the repository is therefore insufficient evidence that the feature is available. Source: [grammar case][unsupported-action].

## 10. Using existing examples effectively

| Source | Useful for | Points to keep in mind |
| --- | --- | --- |
| `examples/using_bodies_in_chunks/simple_request.cc` | API order, chunk input, intervention checks, and cleanup order. | A demonstration program whose helper prints output; it does not establish the HTTP effect of a production connector. [Source][example-flow] |
| `test/common/modsecurity_test_context.h` | Engine and ruleset as members, transaction construction, and a text callback. | The returned transaction refers to the context. [Source][test-context] |
| `examples/simple_example_using_c/test.c` | C names, rule-file loading, and error text. | The shown cleanup path does not include `msc_transaction_cleanup(transaction)`; do not copy it as a complete resource-management pattern. [Source][c-example] |
| README C++ excerpt | A historical introduction to the API concept. | Check names and argument lists against the current headers; they do not all match here. [Source][readme-example] |

## 11. Reading sequence before a concrete change

The following sequence is a recommendation for this working repository:

1. Find the public signature and its actual callers; compare names and parameter types.
2. Read the concrete implementation, including early returns, build macros, and error branches.
3. For every affected pointer, trace creation, ownership transfer, and release.
4. Follow the data flow to the rule phase, intervention, or log output, keeping the two phase numbering systems in [architecture.md](architecture.md) in mind.
5. Read the relevant existing tests and build instructions, and record the scope actually verified.
6. Record unresolved points as unresolved. Finding a symbol name, comment, or example does not replace evidence that the behavior works.

This recommendation follows from the evidenced differences between headers, implementations, and examples above. It does not assert unexamined language rules, unproven thread safety, or automatically successful tests.

[cxx17]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L49-L63
[cxx17-win]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/CMakeLists.txt#L102-L103
[engine-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/modsecurity.h#L275-L331
[rules-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/rules_set.h#L49-L107
[transaction-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L335-L419
[c-types]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L20-L80
[c-trans-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L677-L767
[c-construct]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1804-L1829
[c-engine]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L411-L482
[c-rules]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rules_set.cc#L277-L345
[c-cleanup]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L2153-L2195
[load]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rules_set.cc#L50-L109
[recommended]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/modsecurity.conf-recommended#L1-L76
[parser-build]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/Makefile.am#L1-L43
[driver-rules]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/driver.cc#L46-L124
[chain-fields]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/rule_with_actions.h#L38-L109
[readme-example]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L233-L255
[example-setup]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/examples/using_bodies_in_chunks/simple_request.cc#L131-L170
[example-flow]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/examples/using_bodies_in_chunks/simple_request.cc#L95-L282
[installed-headers]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/Makefile.am#L37-L67
[abi]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/anchored_set_variable.cc#L124-L139
[engine-life]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L62-L109
[transaction-life]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L113-L182
[collections]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/collection/collections.cc#L37-L56
[test-context]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/common/modsecurity_test_context.h#L13-L39
[rules-storage]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/rules.h#L41-L87
[operator-life]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_operator.cc#L54-L79
[actions-life]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_actions.cc#L141-L178
[resolve]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/anchored_set_variable.cc#L70-L111
[resolve-first]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/anchored_set_variable.cc#L114-L122
[evaluate]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_operator.cc#L199-L353
[variable-value]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/variable_value.h#L38-L125
[uri]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L337-L443
[header-lengths]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L618-L654
[request-append]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L929-L971
[rule-base]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/rule.h#L65-L105
[rules-evaluate]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rules_set.cc#L121-L232
[request-phase]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L461-L471
[intervention]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1350-L1389
[response-getter]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1234-L1249
[callback-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/modsecurity.h#L205-L267
[callback-setup]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L389-L413
[callback-call]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L193-L212
[message-life]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_actions.cc#L495-L525
[unsupported-action]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L2726-L2729
[c-example]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/examples/simple_example_using_c/test.c#L26-L76
