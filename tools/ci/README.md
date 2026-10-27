# Runner maintenance

The `Runner maintenance` workflow checks GitHub's published runner images every
Monday at 08:17 UTC. It can also be run manually. It proposes runner additions and
lifecycle changes through one updating pull request.

## Runner data

`.github/runner-matrix.json` contains the reviewed Linux, macOS and Windows runner
lists consumed by `ci_new.yml`. Its metadata job exports the lists for the QA
matrices. Compiler, feature and architecture settings remain in the QA workflow.

The updater adds newer standard runner generations only when they match the
configured family and host architecture. Aliases, preview images, paid
large/xlarge runners, announced retirements and explicitly ignored labels are
excluded from additions. The Linux `pcre1_labels` list is an explicit opt-in;
new Linux runners do not receive PCRE1 coverage automatically.

The Windows 2025 exclusion records the dependency on the
[Windows build migration in #3715](https://github.com/owasp-modsecurity/ModSecurity/pull/3715).
Review that exclusion after the build migration is integrated.

The legacy Ubuntu 22.04 job in `ci.yml` stays fixed. The manifest's `pinned`
entries keep that coverage visible to the monitor. Existing runners are retained
even when deprecated, retired or absent from the catalog; a maintainer must review
their replacement or removal. A retirement marker does not make an unavailable
runner usable.

Lifecycle details and announced retirement dates are recorded in the manifest and
in `.github/runner-lifecycle.md`. Uncertain dates require review. A failed source
request or an invalid source format stops the update rather than removing runner
coverage. The report has no per-run timestamp, so unchanged data does not create a
weekly change.

Retirement dates are accepted only from repository owners, organization members,
collaborators, or publishers explicitly listed in `trusted_announcement_authors`.
Other relevant announcements and notices that cannot be resolved are kept as
canonical issue links in `review_notes` and the report. Untrusted notices do not
set retirement dates or block runner additions.

## Enable the automation

The setup must be on the repository's default branch for its schedule to run.
Use that default branch for manual publishing as well.
In the fork, enable the `Runner maintenance` workflow under **Actions** and enable
**Settings → Actions → General → Allow GitHub Actions to create and approve pull
requests**.

The publisher uses the repository's `GITHUB_TOKEN`; no personal access token is
needed. It writes only the runner manifest and lifecycle report. The updater does
not change workflow code. GitHub may require a maintainer to approve CI runs on
pull requests created with `GITHUB_TOKEN`. Review and merge each runner change
through the usual PR process.

GitHub disables scheduled workflows on newly forked public repositories by
default and can disable them after 60 days without repository activity. If the
schedule stops, check the workflow's enablement in the Actions tab.

## Local checks

The updater and tests use Python's standard library:

```sh
python3 -m unittest discover -s test/ci -v
python3 tools/ci/runner_matrix.py outputs
```

An offline update can use a saved runner-images README and an Announcement
Issues API response, avoiding network access:

```sh
python3 tools/ci/runner_matrix.py update \
  --catalog runner-images-readme.md \
  --announcements runner-announcements.json \
  --today 2026-10-27
```

The optional `--manifest` and `--report` arguments select output paths. The workflow
uses `outputs --github-output` for matrix data and `update --summary` for its job
summary.

## Sources

- [GitHub runner catalog and image policies](https://github.com/actions/runner-images).
- [Runner retirement announcements](https://github.com/actions/runner-images/labels/Announcement).
- [Workflow enablement and scheduled-workflow inactivity](https://docs.github.com/en/actions/how-tos/manage-workflow-runs/disable-and-enable-workflows).
- [Repository workflow permissions](https://docs.github.com/en/repositories/managing-your-repositorys-settings-and-features/enabling-features-for-your-repository/managing-github-actions-settings-for-a-repository).
- [GITHUB_TOKEN and approval of automated PR workflows](https://docs.github.com/en/actions/concepts/security/github_token).
