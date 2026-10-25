# Resource bounds and recovery questions

Inventory resources actually reached by the selected input: buffered bodies, header/argument values, parser nesting, regex/transformation work, rule counts, collection entries, allocations, files, or log output. Include a resource only when the source path establishes it.

For each resource, ask who owns it, who influences its size/work, which configuration or implementation bounds it, where accounting occurs, what happens at the boundary, and how it is released.

Does partial processing preserve the intended rule semantics? Does a rejected/failed case leave a usable legitimate follow-up transaction? Is the suspected amplification reachable under the actual caller and build?

Use bounded local cases with declared limits when execution is selected. Do not turn a review into uncontrolled load or infer service-wide denial of service from an isolated allocation without an impact path.

Entry points: [body processors](../components/body-processors.md), [rules/parser](../components/rules-parser.md), [logging/storage](../components/logging-storage.md), and [finding validation](finding-validation.md).
