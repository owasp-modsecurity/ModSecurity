# Rule and control bypass questions

Use when the task asks whether a specific intended control can be avoided. Define that control, effective rules, engine state, and trusted/caller-controlled inputs first.

- Do variables and transformations represent the bytes relevant to the intended decision?
- Do chain behavior, negation, exclusions, `ctl`, rule removal, or phase selection change the reachable control?
- What happens for empty/split/truncated input, parser errors, body limits, and optional processors?
- Does the expectation distinguish DetectionOnly behavior from disruptive action execution?
- Can a successful method return be mistaken for a completed phase or an allow decision?

Configured exclusions, detection-only operation, or caller misuse are not automatically library vulnerabilities. Establish reachability, intended policy, counterevidence, and a legitimate control before claiming a bypass.

Keep rule evaluation, match, intervention request, caller action, and client outcome separate. Entry points: [engine](../components/engine.md), [rules/parser](../components/rules-parser.md), [body processors](../components/body-processors.md), and [finding validation](finding-validation.md).
