# Adaptation of the connector repository layout

## Requested template and inspected revision

The original contributor requested the `.codex` structure from
[`Easton97-Jens/ModSecurity-conector`](https://github.com/Easton97-Jens/ModSecurity-conector/tree/42fd5a8e895aca3a36b09eb09fe5c7cf9c3399e3/.codex),
branch `master`, inspected at commit
`42fd5a8e895aca3a36b09eb09fe5c7cf9c3399e3` on 2026-10-25.
Its tracked `.codex` files, root `AGENTS.md`, `README.md`, `.gitignore`, and root
build orchestration were examined for structure and repository-specific assumptions.
The [reference inventory](../audit/template-source-index.tsv) records the selected
Git objects. This is not a claim that all connector product source was audited.

The template provides organization and workflow ideas only. Technical statements
about libmodsecurity come from the official source revision in
[upstream-lock.json](upstream-lock.json). The connector repository's policies,
reports, and runtime claims are not evidence of core library behavior.

## Mapping to this repository

| Connector template | ModSecurity adaptation |
| --- | --- |
| Concise root `AGENTS.md` | Reusable English agreement for any repository; core-specific rules are in `context/repository-context.md` |
| `.codex/context/index.md` and `manifest.toml` | Routed technical guides, policy owners, profiles, and saved state |
| Core operational policies | Same domains, adapted to Autotools, C++/C APIs, `test/`, and `v3/master` |
| `context/profiles/` | Core-library, component, build/CI, response-phase, and evidence assessment workflows |
| `context/security/checks/` | Focused review modules tied to library input, lifetime, rules, logging, and dependencies |
| `context/security/connectors/` | `context/security/components/` for actual core areas |
| `.codex/bin/` | Read-only structure, catalog-digest, and source-reference validators |
| `.codex/audit/` | Complete pinned main-source inventory, template inventory, and dated session records |
| Structure and inheritance manifests | Local routing and policy-digest catalog, without invented child repositories |
| `.codex/inheritance-templates/` | Explicit opt-in reuse template and explanation; no active submodule inheritance |
| Historical reports outside normal policy routing | Session/audit records separated from current state and policy owners |

Detailed repository and C++ guides, source provenance, start/end prompts, and
current-state/decision/backlog files supplement the template to meet the original contributor's
request for reusable context across chats. All new content is English; the
reference repository's English/German companion convention is not adopted.
The root agreement is portable; the `.codex` context remains core-specific.
The setup prescribes no chat language; each person chooses.

## Repository-specific rules that were not copied

- Parent/Framework/MRTS topology, connector host/profile matrices, historical PR
  states, and migration outcomes do not describe this core checkout.
- Reference-machine paths, fixed storage roots, numeric limits, required global
  skills, RTK wrappers, authentication wrappers, and service configurations are
  not assumed to exist here.
- SonarQube, runtime hosts, external test frameworks, and quality gates are
  conditional tools or separate task scope, not asserted installations.
- A reference policy's target behavior is not relabeled as implemented core
  behavior. In particular, external host response commitment must not be inferred
  from a library phase result.
- The template's `3.2` policy version is not a ModSecurity release or a version of
  this new local schema. The adapted manifests use their own schema version.

The original contributor subsequently requested RTK guidance. The [RTK guide](rtk.md) adds
conditional use based on RTK's own pinned source and official Codex documentation.
It does not import the template's machine-specific wrappers or make installation
mandatory. The reusable root agreement retains native-tool fallback.

The four upstream submodule gitlinks remain unchanged. No policies are installed
inside them. A future task must inspect and authorize its own repository boundary
before adopting shared instructions there.

## Maintaining the adaptation

Use the local index as the routing authority for these added docs. Update the
catalog after a deliberate policy edit and run the local validators. Do not
periodically overwrite this adaptation from the connector repository: review
relevant changes, record their source revision, and determine whether they apply
to the core. User instructions and actual code remain authoritative.
