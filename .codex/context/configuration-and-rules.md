# Understanding configuration and SecRules from the code

## Scope

This explanation applies to the inspected official `v3/master` revision
`effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`.
It provides orientation to existing implementations and test cases.
It is not a complete directive reference or a claim that a web server
has been successfully deployed.

## 1. Distinguish three layers

| Layer | Purpose in this repository | Source entry point |
| --- | --- | --- |
| Library configuration | Directives set ruleset properties, such as the engine mode, access to bodies, and size limits. | [Parser actions for engine mode and body access][engine-grammar], [limits][limits-grammar] |
| SecRules | Rules combine a variable selection, an operator, and actions. The parser creates `RuleWithOperator` objects for them. | [SecRule grammar][rule-grammar] |
| Calling application or connector | Supplies HTTP data to the library and uses its interventions to decide how processing should continue. The main v3 repository contains the library; connectors are separate projects. | [README: Connectors][connectors], [transaction API][transaction-header] |

SecLang is the rule language read by this project. C++ is the language
of the engine implementation described here. A textual rule is not
translated into a separate C++ program: the grammar creates variable,
operator, and action objects that are later evaluated against a
`Transaction`. Sources: [parser object construction][rule-grammar],
[RuleWithOperator::evaluate][rule-evaluation].

## 2. The engine mode affects actual behavior

`modsecurity.conf-recommended` starts with
`SecRuleEngine DetectionOnly`. This is the content of that example
configuration, not evidence of which configuration an application has
actually loaded. The parser distinguishes `Off`, `On`, and
`DetectionOnly`; `RuleWithActions::executeAction` runs disruptive
actions only in `EnabledRuleEngine`.
Sources: [example configuration][recommended-engine],
[engine grammar][engine-grammar], [action execution][actions].

| Value | What the source establishes here |
| --- | --- |
| `On` | The parser sets `EnabledRuleEngine`; the engine-state check permits disruptive rule actions. |
| `Off` | The parser sets `DisabledRuleEngine`; existing engine regressions expect no blocking in their disabled-engine scenario. |
| `DetectionOnly` | The parser sets `DetectionOnlyRuleEngine`; the central action check does not execute disruptive rule actions. |

Sources: [parser values][engine-grammar], [action check][actions],
[engine regressions][engine-tests]. The JSON expectations are existing
test cases; this document does not establish that they have just been run.

The `ctl:ruleEngine=...` action can set a transaction-specific state.
`Transaction::getRuleEngineState()` uses that state when set and otherwise
uses the ruleset's state. Explaining a particular request may therefore
require more than inspecting a single global directive.
Sources: [ctl implementation][ctl-engine],
[getRuleEngineState][engine-state].

## 3. Read an existing rule in full

The following rule appears exactly as shown in `modsecurity.conf-recommended`:

```apache
SecRule REQUEST_HEADERS:Content-Type "^application/json" \
     "id:'200001',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
```

Source: [the example configuration's JSON rule][recommended-json].

| Part | Meaning in this rule | Code evidence |
| --- | --- | --- |
| `SecRule` | Creates a rule with variables and an operator. | [Grammar branch][rule-grammar] |
| `REQUEST_HEADERS:Content-Type` | Selects the named entry in the request-header collection. | [Header variables in the grammar][header-variables] |
| `"^application/json"` | Regular expression used in the example configuration. Operator resolution and regex execution are implemented in the parser and `Rx`. | [Operator grammar][operators-grammar], [Rx][rx] |
| `id:'200001'` | Rule identifier. For rules that are not appended as chain members, `Driver::addSecRule` checks for missing IDs and duplicate operator-rule IDs. | [Driver::addSecRule][driver-rules] |
| `phase:1` | Assigns the rule to the request-header phase. | [Phase::init][phases] |
| `t:none,t:lowercase` | Suppresses earlier transformations for this evaluation, then applies the specified lowercase conversion. | [Transformation flow][transformations], [LowerCase implementation][lowercase] |
| `pass` | The action implementation frees the existing intervention contents and resets the intervention. Its execution is subject to the disruptive-action check. | [Pass][pass], [action check][actions] |
| `nolog` | Sets `ruleMessage.m_saveMessage` to `false`. | [NoLog][nolog] |
| `ctl:requestBodyProcessor=JSON` | Sets the requested body processor for the transaction. | [Body-processor action][body-ctl] |

Start by identifying the selected input, then the transformations,
the operator, and finally the actions and engine mode. This follows
`RuleWithOperator::evaluate`: evaluate variables, apply transformations,
execute the operator, and run further actions on matches.
Source: [RuleWithOperator::evaluate][rule-evaluation].

### Variable lists and collections

The grammar includes variable lists with `PIPE`, plus exclusion and
count modifiers. It also distinguishes forms such as `ARGS`,
`ARGS:<Element>`, and selection by regular expression. These are
entry points for a rule that examines multiple inputs, excludes
particular targets, or compares a count. An existing count example is
`SecRule &ARGS "@ge 1000"` in the recommended configuration.
Sources: [variable grammar][variable-grammar],
[argument-count rule][arguments-rule].

`TX` has a separate `InMemoryPerProcess` object for each `Collections`
instance. The other collections, `GLOBAL`, `IP`, `SESSION`, `USER`,
and `RESOURCE`, are passed into the transaction's collections object;
the `ModSecurity` constructor chooses their LMDB or in-memory backend
according to `WITH_LMDB`. Do not assume that all variables are stored
persistently or shared across processes.
Sources: [Collections constructor][tx-collection],
[backend selection][collections].

## 4. Phases and when data becomes available

The rule language maps the usual numeric phase values as follows:

| SecRule setting | Phase name selected by the parser |
| --- | --- |
| `phase:1` | `RequestHeadersPhase` |
| `phase:2` | `RequestBodyPhase` |
| `phase:3` | `ResponseHeadersPhase` |
| `phase:4` | `ResponseBodyPhase` |
| `phase:5` | `LoggingPhase` |

Source: [Phase::init mapping][phases]. The same function also accepts
`request`, `response`, and `logging`, mapping them to rule-language
phases 2, 4, and 5. It additionally recognizes `phase:0` as
`ConnectionPhase`; that alone does not prove this phase executes at
a particular connector call. Check the mapping before equating
internal enum values with rule-language numbers. For the API sequence,
see [architecture.md](architecture.md).

The example configuration selects the JSON processor in phase 1.
`processRequestBody()` later processes XML, JSON, or multipart data
according to the selection and compiled libraries, setting error
variables among other state. A body rule can only assess the data
that this processing path actually supplied.
Sources: [JSON-processor selection][recommended-json],
[body processing][body-processing].

## 5. Transformations, actions, and chaining

### Transformations

`executeTransformations()` starts with a copy of the value under
inspection. It considers ruleset defaults, the rule's transformations,
and rule updates. `t:none` affects which earlier transformations
execute. With `multiMatch`, intermediate values are also collected
for operator checks. Without `multiMatch`, the final resulting value
is added at the end. An explanation must therefore retain the
transformation order.
Source: [executeTransformations][transformations].

### Actions and `SecDefaultAction`

Actions do not all wait for the same condition. The code distinguishes,
among other groups, actions that run independently of a subsequent
chained rule's result and actions that run after a full match.
`SetVar` belongs to the first group in the inspected flow. The final
disruptive action is handled elsewhere.
Sources: [executeActionsIndependentOfChainedRuleResult and executeActionsAfterFullMatch][actions],
[call order in the rule evaluator][rule-evaluation].

`SecDefaultAction` is not an arbitrary text suffix in the parser.
Among other checks, the parser requires a disruptive action,
prohibits `t:none` within default actions, and allows only one
such default action set per phase and configuration context.
These constraints are relevant when diagnosing loading errors.
Source: [SecDefaultAction grammar][default-actions].

### `chain`

When appending a chained rule, the driver inherits the preceding
rule's phase and rejects disruptive actions in the appended chain
member. At runtime, `RuleWithOperator::evaluate` recursively calls
the child rule; in the shown path, actions after a full match are
reached only if that further match succeeds. This does not imply
that no earlier action could have run: independent actions are
invoked before the chain check.
Sources: [chaining in the driver][driver-rules],
[evaluation order][rule-evaluation], [action groups][actions].

## 6. Bodies, limits, and error states

The values in `modsecurity.conf-recommended` are concrete examples:

| Setting | Value in the file |
| --- | --- |
| `SecRequestBodyAccess` | `On` |
| `SecRequestBodyLimit` | `13107200` |
| `SecRequestBodyNoFilesLimit` | `131072` |
| `SecRequestBodyLimitAction` | `Reject` |
| `SecRequestBodyJsonDepthLimit` | `512` |
| `SecArgumentsLimit` | `1000` |
| `SecResponseBodyAccess` | `On` |
| `SecResponseBodyMimeType` | `text/plain text/html text/xml` |
| `SecResponseBodyLimit` | `524288` |
| `SecResponseBodyLimitAction` | `ProcessPartial` |

Sources: [request settings][recommended-request],
[response settings][recommended-response]. This table inventories
the file. It does not recommend sizes for your application or
describe every internal default.

When a request-body limit is exceeded, `appendRequestBody()` sets
`INBOUND_DATA_ERROR`. For the configured `ProcessPartial` action,
the shown branch writes only the remaining available space and
returns `false`. For `Reject`, it sets a disruptive intervention
with status 403 only when the rule engine is enabled. The response
path similarly checks size and action and sets `OUTBOUND_DATA_ERROR`.
Sources: [request buffering][request-limit],
[response buffering][response-limit].

**Distinguish comments from implementation:** A comment in the example
configuration describes an automatic switch to `ProcessPartial` in
`DetectionOnly`. However, the cited `appendRequestBody()` branch
directly establishes only that it checks the configured limit action
and suppresses rejection outside `EnabledRuleEngine`. This
documentation does not assert a general automatic switch that is
not implemented at that location.
Sources: [configuration comment][recommended-request],
[implemented branch][request-limit].

On XML or JSON errors, body processing sets variables such as
`REQBODY_ERROR`, `REQBODY_ERROR_MSG`, and
`REQBODY_PROCESSOR_ERROR`. The example configuration contains
explicit rules for these conditions. Investigate such error
variables together with the loaded rules; a failed parser operation
is not automatically the same event as an HTTP rejection carried
out by the connector.
Sources: [body-processor error handling][body-processing],
[parser-error rules][recommended-errors], [disruptive-action check][actions].

## 7. Regex behavior is also a build decision

`Rx::init()` creates a `Regex` for a static pattern; parameters
containing a macro produce a transaction-specific pattern in
`Rx::evaluate()`. An invalid static pattern can therefore be caught
when loading the rule. Evaluation respects a configured
`m_pcreMatchLimit` and reports regex errors through transaction
variables.
Source: [Rx implementation][rx].

`kRxMultiLine` defaults to `true` and is set to `false` by
`MODSEC_REGEX_DOLLAR_ENDONLY`. The `Regex` constructor then
uses either `DOTALL | MULTILINE` or `DOTALL | DOLLAR_ENDONLY`
for PCRE2 or PCRE. The build option
`--enable-regex-dollar-endonly=yes` sets the corresponding macro.
Regressions for this particular build use the
`regex-dollar-endonly` resource; a skipped case in another build
does not confirm its semantics.
Sources: [Rx switch][rx], [Regex constructor][regex-constructor],
[configure switch][regex-option], [existing regressions][regex-test].

## 8. Some familiar directives are not implemented

A directive name appearing in examples does not establish that its
behavior is fully implemented. The inspected grammar includes
the following distinctions:

| Directive | What the grammar actually does | Source |
| --- | --- | --- |
| `SecRequestBodyInMemoryLimit` | Raises a parser error referring users to web server configuration. | [Grammar, 1655–1662][limits-grammar] |
| `SecPcreMatchLimitRecursion` | Accepts the branch without an effect in the shown parser code; the unsupported-feature message is commented out. Do not equate it with the implemented `SecPcreMatchLimit`. | [Grammar, 1696–1707][limits-grammar] |
| `SecTmpDir` | The shown branch sets no value; the unsupported-feature error message is commented out. | [Grammar, 1748–1758][compatibility] |
| `SecDataDir` | No assignment is implemented at this location either; a comment refers to backend behavior. | [Grammar, 1759–1767][compatibility] |
| `SecStatusEngine` | No action is implemented in the shown grammar branch; the error message is commented out. | [Grammar, 1781–1785][compatibility] |

This table describes inspected locations. It is not a complete
list of differences from ModSecurity v2. For another directive,
trace the scanner, grammar action, stored property, and runtime
consumer first.

## 9. Loading configuration and establishing behavior

`Include` is handled in the scanner: the code locates resources
relative to the current file context, expands the resolved path
expression, and opens further scanner buffers. On failure, it
reports the affected file path. A missing include can therefore
explain a loading error even if an individual rule appears
syntactically correct.
Source: [Include handling][include].

The rule checker loads files or SecLang text through `RulesSet`,
but creates no HTTP transaction of its own. Successful loading
therefore establishes a different part of the behavior from a
regression test. Existing regressions combine rules, request and
response data, and expected results; `secruleengine.json`, for
example, contains different engine states and expected HTTP codes.
Sources: [rule checker][rules-check],
[engine regressions][engine-tests].

The resulting workflow for changes is:

1. Trace the affected directive or rule to concrete implementations.
2. Record the required build capabilities and engine mode.
3. Read a relevant existing regression; for a behavior change,
   add the specific observable case.
4. Report loading checks separately from behavior tests.
5. Save results and remaining uncertainties in the current state.

These steps are project working rules. The corresponding commands,
checked against the source, are in
[build-and-test.md](build-and-test.md).

## Sources
[engine-grammar]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1281-L1308
[limits-grammar]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1628-L1708
[rule-grammar]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1116-L1166
[connectors]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L35-L49
[transaction-header]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h
[rule-evaluation]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_operator.cc#L199-L353
[recommended-engine]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/modsecurity.conf-recommended#L1-L7
[actions]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_actions.cc#L202-L324
[engine-tests]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/test-cases/regression/secruleengine.json#L1-L123
[ctl-engine]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/ctl/rule_engine.cc#L25-L59
[engine-state]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1770-L1777
[recommended-json]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/modsecurity.conf-recommended#L25-L37
[header-variables]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L2111-L2122
[operators-grammar]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L914-L950
[rx]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/operators/rx.cc#L29-L100
[driver-rules]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/driver.cc#L76-L124
[phases]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/phase.cc#L25-L72
[transformations]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_actions.cc#L327-L441
[lowercase]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/transformations/lower_case.cc
[pass]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/disruptive/pass.cc#L32-L40
[nolog]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/no_log.cc
[body-ctl]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/ctl/request_body_processor_json.cc#L28-L35
[variable-grammar]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1854-L1978
[arguments-rule]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/modsecurity.conf-recommended#L60-L68
[tx-collection]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/collection/collections.cc#L37-L56
[collections]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L62-L78
[body-processing]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L694-L790
[default-actions]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1217-L1273
[recommended-request]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/modsecurity.conf-recommended#L10-L68
[recommended-response]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/modsecurity.conf-recommended#L163-L187
[request-limit]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L929-L971
[response-limit]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1181-L1230
[recommended-errors]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/modsecurity.conf-recommended#L70-L98
[regex-constructor]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/utils/regex.cc#L90-L117
[regex-option]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L284-L292
[regex-test]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/test-cases/regression/operator-rx-dollar-endonly.json
[compatibility]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1748-L1785
[include]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-scanner.ll#L1285-L1342
[rules-check]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/tools/rules-check/rules-check.cc#L32-L93
