"""Policy tests for runner additions and lifecycle reporting, without network access."""

from copy import deepcopy
from datetime import date
from pathlib import Path
import json
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "ci"))

import runner_matrix


CATALOG_SOURCE = "https://raw.githubusercontent.com/actions/runner-images/main/README.md"
RETIREMENT_SOURCE = "https://github.com/actions/runner-images/issues/14254"
TODAY = date(2026, 10, 27)


def policy_manifest():
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
    return {
        "ubuntu-24.04": image("ubuntu-24.04"),
        "macos-15": image("macos-15", family="macos"),
        "windows-2022": image("windows-2022", family="windows"),
    }


class ManifestTests(unittest.TestCase):
    def load_data(self, data):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "runner-matrix.json"
            path.write_text(json.dumps(data), encoding="utf-8")
            return runner_matrix.load_manifest(path)

    def test_valid_manifest_loads_from_a_file(self):
        expected = policy_manifest()
        self.assertEqual(self.load_data(expected), expected)

    def test_invalid_policies_are_rejected(self):
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


class AnnouncementTrustTests(unittest.TestCase):
    @staticmethod
    def issue(association="CONTRIBUTOR", login="outside-contributor", body=None):
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
        issue = self.issue()
        confirmed, notes = runner_matrix.announcement_data(
            [issue], [], {"ubuntu-24.04"}
        )
        self.assertEqual(confirmed, {})
        self.assertEqual(notes, [issue["html_url"]])

    def test_repository_members_and_allowlisted_publishers_can_confirm_dates(self):
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


class ReconciliationTests(unittest.TestCase):
    def test_new_generation_is_added_without_losing_legacy_coverage(self):
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
        manifest = policy_manifest()
        manifest["coverage"]["macos"]["labels"] = ["macos-26-intel"]
        catalog = current_catalog()
        catalog["macos-26"] = image(
            "macos-26", family="macos", aliases=["macos-26-intel", "macos-26"]
        )
        updated = runner_matrix.reconcile(manifest, catalog, {}, TODAY)
        self.assertEqual(updated["coverage"]["macos"]["labels"], ["macos-26-intel"])

    def test_an_explicit_exclusion_is_preserved(self):
        manifest = policy_manifest()
        reason = "The Windows build migration must be reviewed first."
        manifest["coverage"]["windows"]["ignored"]["windows-2025"] = reason
        catalog = current_catalog()
        catalog["windows-2025"] = image("windows-2025", family="windows")
        updated = runner_matrix.reconcile(manifest, catalog, {}, TODAY)
        self.assertEqual(updated["coverage"]["windows"]["labels"], ["windows-2022"])
        self.assertEqual(updated["coverage"]["windows"]["ignored"], {"windows-2025": reason})

    def test_a_missing_catalog_entry_is_unknown_and_is_not_removed(self):
        catalog = current_catalog()
        del catalog["ubuntu-24.04"]
        updated = runner_matrix.reconcile(policy_manifest(), catalog, {}, TODAY)
        self.assertEqual(updated["coverage"]["linux"]["labels"], ["ubuntu-24.04"])
        self.assertEqual(updated["lifecycle"]["ubuntu-24.04"]["status"], "unknown")
        self.assertIsNone(updated["lifecycle"]["ubuntu-24.04"]["retirement"])
        self.assertIn("ubuntu-22.04", updated["pinned"])

    def test_empty_catalog_does_not_produce_a_mass_change(self):
        with self.assertRaises(ValueError):
            runner_matrix.reconcile(policy_manifest(), {}, {}, TODAY)

    def test_a_configured_retirement_is_marked_before_and_after_its_date(self):
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
        catalog = current_catalog()
        catalog["ubuntu-26.04"] = image("ubuntu-26.04")
        first = runner_matrix.reconcile(policy_manifest(), catalog, {}, TODAY)
        second = runner_matrix.reconcile(first, catalog, {}, TODAY)
        self.assertEqual(first, second)
        self.assertEqual(runner_matrix.render_report(first), runner_matrix.render_report(second))

    def test_report_lists_the_retirement_and_legacy_reason_deterministically(self):
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
