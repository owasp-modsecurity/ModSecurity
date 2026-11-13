"""Policy tests for runner additions and lifecycle reporting, without network access."""

from copy import deepcopy
from contextlib import redirect_stdout
from datetime import date
from io import StringIO
from pathlib import Path
import json
import os
import subprocess
import sys
import tempfile
import unittest
from unittest import mock
from urllib.error import HTTPError, URLError

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "ci"))

import runner_matrix


CATALOG_SOURCE = "https://raw.githubusercontent.com/actions/runner-images/main/README.md"
RETIREMENT_SOURCE = "https://github.com/actions/runner-images/issues/14254"
TODAY = date(2026, 10, 27)


def policy_manifest():
    """Provide minimal reviewed policy with explicit PCRE1 and pinned coverage."""
    return {
        "version": 1,
        "coverage": {
            "linux": {
                "family": "ubuntu",
                "architecture": "x64",
                "labels": ["ubuntu-24.04"],
                "pcre1_labels": ["ubuntu-24.04"],
                "ignored": {},
            },
            "macos": {
                "family": "macos",
                "architecture": "x64",
                "labels": ["macos-15"],
                "ignored": {},
            },
            "windows": {
                "family": "windows",
                "architecture": "x64",
                "labels": ["windows-2022"],
                "ignored": {},
            },
        },
        "pinned": {"ubuntu-22.04": "Legacy x32 coverage in ci.yml needs a reviewed replacement."},
        "lifecycle": {},
    }


def image(label, family="ubuntu", architecture="x64", **overrides):
    """Build a catalog record whose eligibility can vary independently by test."""
    record = {
        "family": family,
        "architecture": architecture,
        "preview": False,
        "deprecated": False,
        "source": CATALOG_SOURCE,
        "aliases": [label],
    }
    record.update(overrides)
    return record


def current_catalog():
    """Provide one active standard image for each configured platform."""
    return {
        "ubuntu-24.04": image("ubuntu-24.04"),
        "macos-15": image("macos-15", family="macos"),
        "windows-2022": image("windows-2022", family="windows"),
    }


class ManifestTests(unittest.TestCase):
    def load_data(self, data):
        """Exercise manifest loading within a temporary trusted checkout."""
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "runner-matrix.json"
            path.write_text(json.dumps(data), encoding="utf-8")
            with mock.patch.object(runner_matrix, "REPOSITORY_ROOT", Path(directory).resolve()):
                return runner_matrix.load_manifest(path)

    def test_valid_manifest_loads_from_a_file(self):
        """A valid file preserves reviewed policy without normalization changes."""
        expected = policy_manifest()
        self.assertEqual(self.load_data(expected), expected)

    def test_invalid_policies_are_rejected(self):
        """Reject malformed coverage before it can become workflow matrix data."""
        cases = {}
        value = policy_manifest()
        value["version"] = 2
        cases["unsupported version"] = value
        value = policy_manifest()
        del value["coverage"]["windows"]
        cases["missing platform"] = value
        value = policy_manifest()
        value["coverage"]["linux"]["labels"] = "ubuntu-24.04"
        cases["labels must be a list"] = value
        value = policy_manifest()
        value["coverage"]["linux"]["labels"].append("ubuntu-24.04")
        cases["duplicate runner"] = value
        value = policy_manifest()
        value["coverage"]["linux"]["labels"] = ["ubuntu-24.04; echo bad"]
        cases["invalid runner label"] = value
        value = policy_manifest()
        value["coverage"]["linux"]["architecture"] = "i386"
        cases["unsupported automatic architecture"] = value
        value = policy_manifest()
        value["coverage"]["linux"]["pcre1_labels"] = ["ubuntu-22.04"]
        cases["unconfigured PCRE1 runner"] = value
        value = policy_manifest()
        value["coverage"]["linux"]["ignored"] = {"ubuntu-26.04": ""}
        cases["exclusion without a reason"] = value
        for name, data in cases.items():
            with self.subTest(name=name), self.assertRaises(ValueError):
                self.load_data(data)

    def test_pcre1_coverage_requires_explicit_opt_in(self):
        """A new runner receives no PCRE1 job unless policy explicitly enables it."""
        manifest = policy_manifest()
        manifest["coverage"]["linux"]["labels"].append("ubuntu-26.04")
        output = runner_matrix.matrix_outputs(manifest)
        self.assertEqual(output["linux"], ["ubuntu-24.04", "ubuntu-26.04"])
        self.assertEqual(output["pcre1"], ["ubuntu-24.04"])
        self.assertEqual(
            output["linux_exclude"],
            [{"os": "ubuntu-26.04", "configure": {"label": "with pcre", "opt": "--with-pcre"}}],
        )
        self.assertNotIn("ubuntu-22.04", output["linux"])

    def test_review_links_require_canonical_ascii_issue_numbers(self):
        """Using a digit shorthand must not admit Unicode digits into source URLs."""
        manifest = policy_manifest()
        manifest["review_notes"] = [RETIREMENT_SOURCE]
        self.assertIs(runner_matrix.validate_manifest(manifest), manifest)
        for number in ("٣٧١٨", "３７１８"):
            with self.subTest(number=number):
                manifest["review_notes"] = [f"https://github.com/actions/runner-images/issues/{number}"]
                with self.assertRaisesRegex(ValueError, "Invalid announcement review links"):
                    runner_matrix.validate_manifest(manifest)


class SourceRequestTests(unittest.TestCase):
    def test_download_failures_preserve_the_public_failure_contract(self):
        """HTTP errors remain covered by URLError while timeouts fail the same way."""
        failures = (
            HTTPError(CATALOG_SOURCE, 429, "Too Many Requests", None, None),
            URLError("Unavailable"),
            TimeoutError("Timed out"),
        )
        for error in failures:
            with self.subTest(error=type(error).__name__), mock.patch.object(runner_matrix, "urlopen", side_effect=error):
                with self.assertRaisesRegex(RuntimeError, "runner files were not changed"):
                    runner_matrix._fetch(CATALOG_SOURCE)


class AnnouncementTrustTests(unittest.TestCase):
    @staticmethod
    def issue(association="CONTRIBUTOR", login="outside-contributor", body=None):
        """Build a relevant notice with independently configurable publisher trust."""
        return {
            "number": 99999,
            "html_url": "https://github.com/actions/runner-images/issues/99999",
            "labels": [{"name": "Announcement"}],
            "title": "[Ubuntu] Runner images retirement",
            "body": body or (
                "## Breaking changes\n"
                "The `ubuntu-24.04` runner images will be deprecated.\n"
                "## Target date\n"
                "Retirement: April 17, 2027\n"
            ),
            "author_association": association,
            "user": {"login": login},
        }

    def test_untrusted_publisher_cannot_confirm_a_fabricated_retirement(self):
        """Public issue templates cannot turn an unknown author's date into policy."""
        issue = self.issue()
        confirmed, notes = runner_matrix.announcement_data(
            [issue], [], {"ubuntu-24.04"}
        )
        self.assertEqual(confirmed, {})
        self.assertEqual(notes, [issue["html_url"]])

    def test_repository_members_and_allowlisted_publishers_can_confirm_dates(self):
        """Each approved publisher category can supply a confirmed retirement date."""
        cases = (
            ("OWNER", "repository-owner", []),
            ("MEMBER", "organization-member", []),
            ("COLLABORATOR", "repository-collaborator", []),
            ("CONTRIBUTOR", "v-AndriiKhyliuk", ["v-AndriiKhyliuk"]),
        )
        for association, login, authors in cases:
            with self.subTest(association=association, login=login):
                issue = self.issue(association=association, login=login)
                confirmed, notes = runner_matrix.announcement_data(
                    [issue], authors, {"ubuntu-24.04"}
                )
                self.assertEqual(
                    confirmed["ubuntu-24.04"]["retirement"], "2027-04-17"
                )
                self.assertEqual(confirmed["ubuntu-24.04"]["source"], issue["html_url"])
                self.assertEqual(notes, [])

    def test_trusted_notice_without_resolvable_labels_still_requires_review(self):
        """A trusted author does not make an ambiguous display name actionable."""
        issue = self.issue(
            association="MEMBER",
            login="organization-member",
            body=(
                "## Breaking changes\n"
                "These runner images will be retired.\n"
                "## Runner images affected\n"
                "- [x] Ubuntu 24.04\n"
                "- [ ] Ubuntu 26.04\n"
                "## Target date\n"
                "Retirement: April 17, 2027\n"
            ),
        )
        confirmed, notes = runner_matrix.announcement_data(
            [issue], [], {"ubuntu-24.04"}
        )
        self.assertEqual(confirmed, {})
        self.assertEqual(notes, [issue["html_url"]])


class PathBoundaryTests(unittest.TestCase):
    def setUp(self):
        """Create a trusted checkout and an unrelated neighboring directory."""
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        base = Path(temporary.name)
        self.root = base / "checkout"
        self.root.mkdir()
        self.outside = base / "checkout-other"
        self.outside.mkdir()
        root_patch = mock.patch.object(runner_matrix, "REPOSITORY_ROOT", self.root.resolve())
        root_patch.start()
        self.addCleanup(root_patch.stop)
        self.manifest = self.root / "manifest.json"
        self.original = json.dumps(policy_manifest())
        self.manifest.write_text(self.original, encoding="utf-8")
        self.report = self.root / "report.md"
        self.catalog = self.root / "catalog.md"
        self.catalog.write_text(
            (ROOT / "test/ci/fixtures/runner-catalog.md").read_text(encoding="utf-8"),
            encoding="utf-8",
        )
        self.announcements = self.root / "announcements.json"
        self.announcements.write_text("[]", encoding="utf-8")
        self.protected = self.outside / "protected.json"
        self.protected.write_text(self.original, encoding="utf-8")

    def arguments(self, command="update", **overrides):
        """Build CLI arguments with contained defaults and selected path overrides."""
        paths = {"manifest": self.manifest}
        if command == "update":
            paths.update(report=self.report, catalog=self.catalog, announcements=self.announcements)
        paths.update(overrides)
        return [command] + [item for key, path in paths.items() for item in ("--" + key, str(path))]

    def test_direct_file_operations_reject_external_and_traversal_paths(self):
        """All data sinks reject escapes, including similarly named sibling roots."""
        for path in (self.protected, self.root / ".." / "checkout-other" / "protected.json"):
            with self.subTest(path=path):
                for operation in (
                    lambda: runner_matrix.load_manifest(path),
                    lambda: runner_matrix._read_text(path),
                    lambda: runner_matrix._write_changed(path, "changed"),
                ):
                    with self.assertRaisesRegex(ValueError, "inside the repository"):
                        operation()
        self.assertEqual(self.protected.read_text(encoding="utf-8"), self.original)
        missing = self.outside / "new-directory" / "report.md"
        with self.assertRaisesRegex(ValueError, "inside the repository"):
            runner_matrix._write_changed(missing, "changed")
        self.assertFalse(missing.parent.exists())

    def test_cli_validates_every_used_path_before_reading_or_updating(self):
        """A late invalid output path cannot leave a partially updated manifest."""
        cases = (
            ("outputs", "manifest"), ("outputs", "github-output"),
            ("update", "manifest"), ("update", "report"), ("update", "catalog"),
            ("update", "announcements"), ("update", "summary"),
        )
        with mock.patch.dict(os.environ, {"GITHUB_OUTPUT": "", "GITHUB_STEP_SUMMARY": ""}):
            for command, option in cases:
                with self.subTest(command=command, option=option):
                    arguments = self.arguments(command, **{option: self.protected})
                    with mock.patch.object(runner_matrix, "load_manifest") as load:
                        with self.assertRaisesRegex(ValueError, "inside the repository"):
                            runner_matrix.main(arguments)
                        load.assert_not_called()
        self.assertEqual(self.manifest.read_text(encoding="utf-8"), self.original)
        self.assertEqual(self.protected.read_text(encoding="utf-8"), self.original)
        self.assertFalse(self.report.exists())

    def test_resolved_internal_paths_and_offline_updates_remain_supported(self):
        """Canonical aliases inside the checkout still support local fixtures."""
        (self.root / "nested").mkdir()
        alias = self.root / "nested" / ".." / "manifest.json"
        relative = os.path.relpath(self.manifest)
        self.assertEqual(runner_matrix.load_manifest(alias), policy_manifest())
        self.assertEqual(runner_matrix.load_manifest(relative), policy_manifest())
        with redirect_stdout(StringIO()):
            runner_matrix.main(self.arguments())
        self.assertTrue(self.report.is_file())

    def test_missing_configured_runners_keep_untrusted_announcement_review_links(self):
        """A vanished covered or pinned image still exposes relevant notices for review."""
        for label in ("ubuntu-22.04", "ubuntu-24.04"):
            with self.subTest(label=label):
                catalog = current_catalog()
                catalog.pop(label, None)
                rows = [
                    "## Available Images", "",
                    "| Image | Architecture | YAML Label | Included Software |",
                    "| --- | --- | --- | --- |",
                ]
                rows.extend(
                    f"| {name} | {image['architecture']} | `{name}` | software |"
                    for name, image in catalog.items()
                )
                self.catalog.write_text("\n".join(rows) + "\n", encoding="utf-8")
                self.manifest.write_text(self.original, encoding="utf-8")
                issue = AnnouncementTrustTests.issue(body=(
                    f"## Breaking changes\nThe `{label}` runner images will be deprecated.\n"
                    "## Target date\nRetirement: April 17, 2027\n"
                ))
                self.announcements.write_text(json.dumps([issue]), encoding="utf-8")
                with redirect_stdout(StringIO()):
                    runner_matrix.main(self.arguments())
                updated = json.loads(self.manifest.read_text(encoding="utf-8"))
                self.assertEqual(updated["review_notes"], [issue["html_url"]])
                self.assertEqual(updated["lifecycle"][label]["status"], "unknown")
                self.assertIsNone(updated["lifecycle"][label]["retirement"])
                self.assertTrue(updated["lifecycle"][label]["needs_review"])
                self.assertIn(issue["html_url"], self.report.read_text(encoding="utf-8"))

    def test_symlink_escape_is_rejected_for_reads_and_missing_output_files(self):
        """Existing directory links cannot redirect inputs or new output parents."""
        link = self.root / "linked"
        try:
            link.symlink_to(self.outside, target_is_directory=True)
        except OSError as error:
            if os.name != "nt":
                raise
            # Windows junctions exercise the same resolved-path boundary without
            # requiring the Create symbolic links privilege on developer machines.
            result = subprocess.run(
                f'cmd.exe /d /c mklink /J "{link}" "{self.outside}"',
                capture_output=True, check=False,
            )
            if result.returncode:
                self.skipTest(f"Directory links are unavailable: {error}")
        protected = link / "protected.json"
        missing_report = link / "new-directory" / "report.md"
        with self.assertRaisesRegex(ValueError, "inside the repository"):
            runner_matrix.load_manifest(protected)
        with self.assertRaisesRegex(ValueError, "inside the repository"):
            runner_matrix._write_changed(missing_report, "changed")
        self.assertFalse((self.outside / "new-directory").exists())
        arguments = self.arguments(catalog=protected)
        with self.assertRaisesRegex(ValueError, "inside the repository"):
            runner_matrix.main(arguments)
        self.assertEqual(self.protected.read_text(encoding="utf-8"), self.original)

    def test_only_the_matching_github_command_file_is_allowed_outside_checkout(self):
        """The environment grants one exact command file, never its directory."""
        output = self.outside / "github-output"
        summary = self.outside / "github-summary"
        with mock.patch.dict(os.environ, {"GITHUB_OUTPUT": str(output), "GITHUB_STEP_SUMMARY": str(summary)}):
            runner_matrix._append_command_file(output, "matrix=data\n", "GITHUB_OUTPUT")
            runner_matrix._append_command_file(summary, "report\n", "GITHUB_STEP_SUMMARY")
            for path, variable in (
                (self.protected, "GITHUB_OUTPUT"),
                (output, "GITHUB_STEP_SUMMARY"),
                (summary, "GITHUB_OUTPUT"),
            ):
                with self.subTest(path=path, variable=variable):
                    with self.assertRaisesRegex(ValueError, "inside the repository"):
                        runner_matrix._append_command_file(path, "changed", variable)
            runner_matrix.main(self.arguments("outputs", **{"github-output": output}))
            with redirect_stdout(StringIO()):
                runner_matrix.main(self.arguments(summary=summary))
        self.assertIn("linux=", output.read_text(encoding="utf-8"))
        self.assertIn("# Runner lifecycle", summary.read_text(encoding="utf-8"))
        self.assertEqual(self.protected.read_text(encoding="utf-8"), self.original)


class ReconciliationTests(unittest.TestCase):
    def test_new_generation_is_added_without_losing_legacy_coverage(self):
        """New standard generations preserve pinned coverage and the input manifest."""
        manifest = policy_manifest()
        before = deepcopy(manifest)
        catalog = current_catalog()
        catalog["ubuntu-26.04"] = image("ubuntu-26.04")
        updated = runner_matrix.reconcile(manifest, catalog, {}, TODAY)
        self.assertEqual(updated["coverage"]["linux"]["labels"], ["ubuntu-24.04", "ubuntu-26.04"])
        self.assertEqual(updated["pinned"], before["pinned"])
        self.assertEqual(updated["coverage"]["linux"]["pcre1_labels"], ["ubuntu-24.04"])
        self.assertEqual(manifest, before, "Reconciliation must not mutate its input")

    def test_new_arm64_ubuntu_generation_keeps_the_host_architecture(self):
        """An ARM64 policy never admits the x64 variant of a new Ubuntu runner."""
        manifest = policy_manifest()
        manifest["coverage"]["linux"]["architecture"] = "arm64"
        manifest["coverage"]["linux"]["labels"] = ["ubuntu-24.04-arm"]
        manifest["coverage"]["linux"]["pcre1_labels"] = []
        catalog = current_catalog()
        catalog.update({
            "ubuntu-24.04-arm": image("ubuntu-24.04-arm", architecture="arm64"),
            "ubuntu-26.04-arm": image("ubuntu-26.04-arm", architecture="arm64"),
            "ubuntu-26.04": image("ubuntu-26.04"),
        })
        updated = runner_matrix.reconcile(manifest, catalog, {}, TODAY)
        self.assertEqual(
            updated["coverage"]["linux"]["labels"],
            ["ubuntu-24.04-arm", "ubuntu-26.04-arm"],
        )
        self.assertEqual(updated["pinned"], manifest["pinned"])

    def test_only_standard_released_runners_of_the_requested_architecture_are_added(self):
        """Preview, retired, paid, aliased, older and other-architecture labels stay out."""
        catalog = current_catalog()
        catalog.update({
            "ubuntu-26.04": image("ubuntu-26.04"),
            "ubuntu-26.04-arm": image("ubuntu-26.04-arm", architecture="arm64"),
            "ubuntu-28.04": image("ubuntu-28.04", preview=True),
            "ubuntu-30.04": image("ubuntu-30.04", deprecated=True),
            "ubuntu-latest": image("ubuntu-latest", aliases=["ubuntu-26.04", "ubuntu-latest"]),
            "ubuntu-22.04": image("ubuntu-22.04"),
            "macos-26-large": image("macos-26-large", family="macos"),
            "macos-26-xlarge": image("macos-26-xlarge", family="macos"),
        })
        updated = runner_matrix.reconcile(policy_manifest(), catalog, {}, TODAY)
        self.assertEqual(updated["coverage"]["linux"]["labels"], ["ubuntu-24.04", "ubuntu-26.04"])
        self.assertEqual(updated["coverage"]["macos"]["labels"], ["macos-15"])
        self.assertNotIn("ubuntu-latest", updated["lifecycle"])

    def test_an_alias_of_existing_coverage_does_not_add_another_job(self):
        """Equivalent names in one catalog row do not duplicate existing coverage."""
        manifest = policy_manifest()
        manifest["coverage"]["macos"]["labels"] = ["macos-26-intel"]
        catalog = current_catalog()
        catalog["macos-26"] = image(
            "macos-26", family="macos", aliases=["macos-26-intel", "macos-26"]
        )
        updated = runner_matrix.reconcile(manifest, catalog, {}, TODAY)
        self.assertEqual(updated["coverage"]["macos"]["labels"], ["macos-26-intel"])

    def test_an_explicit_exclusion_is_preserved(self):
        """An ignored generation stays excluded and retains its review reason."""
        manifest = policy_manifest()
        reason = "The Windows build migration must be reviewed first."
        manifest["coverage"]["windows"]["ignored"]["windows-2025"] = reason
        catalog = current_catalog()
        catalog["windows-2025"] = image("windows-2025", family="windows")
        updated = runner_matrix.reconcile(manifest, catalog, {}, TODAY)
        self.assertEqual(updated["coverage"]["windows"]["labels"], ["windows-2022"])
        self.assertEqual(updated["coverage"]["windows"]["ignored"], {"windows-2025": reason})

    def test_a_missing_catalog_entry_is_unknown_and_is_not_removed(self):
        """Catalog disappearance marks uncertainty without silently deleting a job."""
        catalog = current_catalog()
        del catalog["ubuntu-24.04"]
        updated = runner_matrix.reconcile(policy_manifest(), catalog, {}, TODAY)
        self.assertEqual(updated["coverage"]["linux"]["labels"], ["ubuntu-24.04"])
        self.assertEqual(updated["lifecycle"]["ubuntu-24.04"]["status"], "unknown")
        self.assertIsNone(updated["lifecycle"]["ubuntu-24.04"]["retirement"])
        self.assertIn("ubuntu-22.04", updated["pinned"])

    def test_empty_catalog_does_not_produce_a_mass_change(self):
        """Reject a failed catalog parse rather than marking every runner absent."""
        manifest = policy_manifest()
        with self.assertRaises(ValueError):
            runner_matrix.reconcile(manifest, {}, {}, TODAY)

    def test_a_configured_retirement_is_marked_before_and_after_its_date(self):
        """Crossing a confirmed date changes lifecycle status but retains coverage."""
        announcement = {
            "ubuntu-22.04": {
                "status": "deprecated",
                "retirement": "2027-04-17",
                "source": RETIREMENT_SOURCE,
                "needs_review": False,
            }
        }
        for today, status in ((TODAY, "deprecated"), (date(2027, 4, 18), "retired")):
            with self.subTest(today=today):
                updated = runner_matrix.reconcile(
                    policy_manifest(), current_catalog(), announcement, today
                )
                entry = updated["lifecycle"]["ubuntu-22.04"]
                self.assertEqual(entry["status"], status)
                self.assertEqual(entry["retirement"], "2027-04-17")
                self.assertIn("ubuntu-22.04", updated["pinned"])
                self.assertEqual(updated["coverage"]["linux"]["labels"], ["ubuntu-24.04"])

    def test_uncertain_retirement_dates_are_not_invented(self):
        """Missing or invalid dates remain unset and require human review."""
        for retirement, needs_review in ((None, True), ("2027-13-40", False)):
            with self.subTest(retirement=retirement):
                announcement = {
                    "ubuntu-22.04": {
                        "status": "deprecated",
                        "retirement": retirement,
                        "source": RETIREMENT_SOURCE,
                        "needs_review": needs_review,
                    }
                }
                updated = runner_matrix.reconcile(
                    policy_manifest(), current_catalog(), announcement, TODAY
                )
                entry = updated["lifecycle"]["ubuntu-22.04"]
                self.assertEqual(entry["status"], "deprecated")
                self.assertIsNone(entry["retirement"])
                self.assertTrue(entry["needs_review"])

    def test_announced_retirement_blocks_addition_of_a_new_runner(self):
        """A newer catalog generation is ineligible when already marked for retirement."""
        catalog = current_catalog()
        catalog["ubuntu-26.04"] = image("ubuntu-26.04")
        announcement = {
            "ubuntu-26.04": {
                "status": "deprecated",
                "retirement": "2027-04-17",
                "source": RETIREMENT_SOURCE,
                "needs_review": False,
            }
        }
        updated = runner_matrix.reconcile(policy_manifest(), catalog, announcement, TODAY)
        self.assertEqual(updated["coverage"]["linux"]["labels"], ["ubuntu-24.04"])

    def test_repeated_inputs_produce_the_same_manifest_and_report(self):
        """Identical source data produces no recurring manifest or report churn."""
        catalog = current_catalog()
        catalog["ubuntu-26.04"] = image("ubuntu-26.04")
        first = runner_matrix.reconcile(policy_manifest(), catalog, {}, TODAY)
        second = runner_matrix.reconcile(first, catalog, {}, TODAY)
        self.assertEqual(first, second)
        self.assertEqual(runner_matrix.render_report(first), runner_matrix.render_report(second))

    def test_report_lists_the_retirement_and_legacy_reason_deterministically(self):
        """Report ordering stays stable while exposing dates, sources and pin reasons."""
        announcement = {
            "ubuntu-22.04": {
                "status": "deprecated",
                "retirement": "2027-04-17",
                "source": RETIREMENT_SOURCE,
                "needs_review": False,
            }
        }
        updated = runner_matrix.reconcile(
            policy_manifest(), current_catalog(), announcement, TODAY
        )
        report = runner_matrix.render_report(updated)
        self.assertIn("ubuntu-22.04", report)
        self.assertIn("2027-04-17", report)
        self.assertIn(RETIREMENT_SOURCE, report)
        self.assertIn(updated["pinned"]["ubuntu-22.04"], report)
        reordered = deepcopy(updated)
        reordered["lifecycle"] = dict(reversed(list(updated["lifecycle"].items())))
        self.assertEqual(report, runner_matrix.render_report(reordered))


if __name__ == "__main__":
    unittest.main()
