"""Tests for upstream runner catalog and lifecycle announcement formats."""

import json
import subprocess
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "ci"))

from runner_catalog import announcement_review_notes, parse_announcements, parse_catalog

FIXTURES = Path(__file__).with_name("fixtures")


def issue(number, fixture=None, **changes):
    """Build a canonical announcement from an optional captured upstream body."""
    value = {
        "number": number,
        "title": "Deprecation of operating system runner images",
        "html_url": f"https://github.com/actions/runner-images/issues/{number}",
        "labels": [{"name": "Announcement"}],
        "body": (FIXTURES / fixture).read_text(encoding="utf-8") if fixture else "",
    }
    value.update(changes)
    return value


class CatalogTests(unittest.TestCase):
    def setUp(self):
        """Load a representative catalog with aliases, architectures and statuses."""
        self.readme = (FIXTURES / "runner-catalog.md").read_text(encoding="utf-8")

    def test_architecture_status_and_aliases_are_separate(self):
        """Parsing preserves image metadata and every label in an alias group."""
        rows = parse_catalog(self.readme)
        self.assertEqual(rows["ubuntu-26.04"]["architecture"], "x64")
        self.assertEqual(rows["ubuntu-26.04-arm"]["architecture"], "arm64")
        self.assertEqual(rows["xcode-27"]["family"], "macos")
        self.assertTrue(rows["xcode-27"]["preview"])
        self.assertTrue(rows["macos-14-large"]["deprecated"])
        self.assertFalse(rows["ubuntu-22.04"]["deprecated"])
        self.assertEqual(rows["windows-2025"]["aliases"], ["windows-latest", "windows-2025", "windows-2025-vs2026"])
        self.assertEqual(rows["windows-2025"]["aliases"], rows["windows-2025-vs2026"]["aliases"])
        self.assertEqual(rows["xcode-27"]["aliases"], ["xcode-27", "xcode-27-xlarge"])
        self.assertTrue(rows["ubuntu-26.04"]["source"].startswith("https://github.com/actions/runner-images"))

    def test_changed_or_empty_catalog_fails_closed(self):
        """Malformed tables and unexpected labels cannot produce partial runner data."""
        for text in (
            "# Empty",
            self.readme.replace("YAML Label", "New unknown column"),
            self.readme.replace("| x64 |", "| unknown |", 1),
            "## Available Images\n\n| Image | Architecture | YAML Label | Included Software |\n| --- | --- | --- | --- |\n",
            self.readme.replace("`ubuntu-26.04-arm`", "`ubuntu-26.04`"),
            self.readme.replace("`ubuntu-26.04`", "`$(execute)`", 1),
            self.readme.replace("| Ubuntu 22.04 |", "Ubuntu 22.04 |", 1),
        ):
            with self.subTest(text=text[:100]), self.assertRaises(ValueError):
                parse_catalog(text)

    def test_other_sections_cannot_add_labels(self):
        """A later Markdown table cannot inject rows into Available Images."""
        rows = parse_catalog(self.readme + "\n| Ubuntu 99.04 | x64 | `ubuntu-99.04` | text |\n")
        self.assertNotIn("ubuntu-99.04", rows)

    def test_repeated_alias_in_same_row_is_deduplicated(self):
        """Repeated literal names in one image row remain one alias."""
        text = self.readme.replace("`ubuntu-26.04`", "`ubuntu-26.04` or `ubuntu-26.04`", 1)
        self.assertEqual(parse_catalog(text)["ubuntu-26.04"]["aliases"], ["ubuntu-26.04"])


class AnnouncementTests(unittest.TestCase):
    def test_comments_and_decorated_headings_preserve_announcement_scope(self):
        """Complete template comments disappear while unclosed comments remain prose."""
        body = """<!-- Retirement of `ubuntu-24.04` runner images.
<!-- nested opener ends at the first closer -->
<!-- adjacent comment --><!-- another comment -->
<!-- unfinished template comment
### Breaking changes ###
Retirement of `ubuntu-22.04` runner images.
### Target date ####
Final retirement date: APRIL 17TH, 2027
"""
        rows = parse_announcements([issue(8, body=body)])
        self.assertEqual(set(rows), {"ubuntu-22.04"})
        self.assertEqual(rows["ubuntu-22.04"]["retirement"], "2027-04-17")
        self.assertFalse(rows["ubuntu-22.04"]["needs_review"])

    def test_checked_entries_require_spaces_around_checkbox(self):
        """Tabs and uppercase checks work, but incomplete list syntax adds no labels."""
        body = """### Breaking changes
Retirement of runner images.
### Target date
Retirement: 2027-04-17
### Runner images affected
  -\t[X]\t`ubuntu-22.04`\t
- [x]\t\t
*[x] `ubuntu-24.04`
- [x]`ubuntu-26.04`
- [ ] `ubuntu-20.04`
"""
        rows = parse_announcements([issue(9, body=body)])
        self.assertEqual(set(rows), {"ubuntu-22.04"})
        self.assertEqual(rows["ubuntu-22.04"]["retirement"], "2027-04-17")

    def test_retirement_prefixes_accept_only_complete_template_names(self):
        """Whitespace and case vary without accepting additional prefix words."""
        for prefix, retirement in (
            ("Retirement", "2027-04-17"),
            ("retirement date", "2027-04-17"),
            ("FINAL\tRETIREMENT   DATE", "2027-04-17"),
            ("Retirement planned date", None),
            ("Finalretirement", None),
        ):
            body = f"### Breaking changes\nRetirement of `ubuntu-22.04` runner images.\n### Target date\n{prefix}: April 17th, 2027\n"
            with self.subTest(prefix=prefix):
                rows = parse_announcements([issue(10, body=body)])
                self.assertEqual(rows["ubuntu-22.04"]["retirement"], retirement)
                self.assertEqual(rows["ubuntu-22.04"]["needs_review"], retirement is None)

    def test_long_near_matches_finish_and_keep_the_confirmed_retirement(self):
        """Hostile comment, whitespace and template near-matches cannot stall parsing."""
        base = "### Breaking changes\nRetirement of `ubuntu-22.04` runner images.\n### Target date\nRetirement: 2027-04-17\n"
        padding = " " * 100_000
        cases = {
            "unclosed comments": "<!--" * 50_000 + "\n" + base,
            "heading decoration": "### title" + padding + "!\n" + base,
            "checked entry": base + "### Runner images affected\n- [x] title" + padding + "!\n",
            "unheaded section": "prose\n" + "\n" * 100_000 + "unknown\n" + base,
            "retirement prefix": base + "Retirement" + padding + "!\n",
        }
        program = """
import json
import sys
sys.path.insert(0, sys.argv[1])
from runner_catalog import parse_announcements
print(json.dumps(parse_announcements([json.load(sys.stdin)])))
"""
        for name, body in cases.items():
            with self.subTest(name=name):
                result = subprocess.run(
                    [sys.executable, "-c", program, str(ROOT / "tools" / "ci")],
                    input=json.dumps(issue(11, body=body)), text=True,
                    capture_output=True, check=True, timeout=10,
                )
                rows = json.loads(result.stdout)
                self.assertEqual(set(rows), {"ubuntu-22.04"})
                self.assertEqual(rows["ubuntu-22.04"]["retirement"], "2027-04-17")
                self.assertFalse(rows["ubuntu-22.04"]["needs_review"])

    def test_retirement_date_ignores_brownouts_and_replacements(self):
        """Only the final retirement date and affected images become lifecycle data."""
        rows = parse_announcements([issue(14254, "announcement-14254.md")])
        self.assertEqual(set(rows), {"ubuntu-22.04", "ubuntu-22.04-arm"})
        self.assertEqual(rows["ubuntu-22.04"]["retirement"], "2027-04-17")
        self.assertFalse(rows["ubuntu-22.04"]["needs_review"])
        self.assertEqual(rows["ubuntu-22.04"]["status"], "deprecated")

    def test_checked_images_resolve_exact_impact_labels(self):
        """Checked display names confirm literal impacts without retiring replacements."""
        rows = parse_announcements([issue(13518, "announcement-13518.md")])
        self.assertEqual(set(rows), {"macos-14", "macos-14-large", "macos-14-xlarge"})
        self.assertEqual(rows["macos-14"]["retirement"], "2026-11-02")
        self.assertNotIn("macos-15", rows)
        self.assertNotIn("macos-26", rows)

    def test_iso_final_retirement_date_on_next_line(self):
        """A complete date on the next template line remains a valid retirement."""
        rows = parse_announcements([issue(12045, "announcement-12045.md")])
        self.assertEqual(set(rows), {"windows-2019"})
        self.assertEqual(rows["windows-2019"]["retirement"], "2025-06-30")

    def test_ambiguous_missing_year_and_invalid_dates_need_review(self):
        """Incomplete, impossible or conflicting dates never become confirmed dates."""
        for date in ("April 17th", "2027-02-30", "April 17th, 2027\nRetirement: April 18th, 2027"):
            body = f"### Breaking changes\nDeprecation of `ubuntu-22.04` runner images.\n### Target date\nRetirement: {date}\n"
            rows = parse_announcements([issue(1, body=body)])
            self.assertIsNone(rows["ubuntu-22.04"]["retirement"])
            self.assertTrue(rows["ubuntu-22.04"]["needs_review"])

    def test_unchecked_labels_and_display_names_are_not_invented(self):
        """Unsupported display names and unchecked choices remain a review note."""
        body = "### Breaking changes\nRetirement of runner images.\n### Target date\nRetirement: 2027-04-17\n### Runner images affected\n- [x] Ubuntu 22.04\n- [ ] `ubuntu-24.04`\n### Mitigation ways\nUse `ubuntu-26.04`.\n"
        data = issue(2, body=body)
        self.assertEqual(parse_announcements([data]), {})
        self.assertEqual(announcement_review_notes([data]), [data["html_url"]])

    def test_tool_deprecation_is_not_an_os_retirement(self):
        """Removing a bundled tool does not retire the runner hosting that tool."""
        body = "### Breaking changes\nRemove Node.js from `ubuntu-22.04`.\n### Target date\nRetirement: 2027-04-17\n"
        self.assertEqual(parse_announcements([issue(3, title="Deprecation of Node.js tools", body=body)]), {})
        self.assertEqual(parse_announcements([issue(3, title="Deprecation of tools on runner images", body=body)]), {})

    def test_checked_literal_label_is_supported_without_impact_list(self):
        """An explicitly checked YAML label identifies its image without prose impact."""
        body = "### Breaking changes\nRetirement of runner images.\n### Target date\n**Retirement: April 17th, 2027**\n### Runner images affected\n- [x] `ubuntu-22.04`\n- [ ] `ubuntu-24.04`\n"
        rows = parse_announcements([issue(4, body=body)])
        self.assertEqual(set(rows), {"ubuntu-22.04"})
        self.assertEqual(rows["ubuntu-22.04"]["retirement"], "2027-04-17")

    def test_plain_mitigation_text_does_not_add_replacement_labels(self):
        """Unheaded mitigation prose cannot add a suggested replacement to impacts."""
        body = "Retirement of `ubuntu-22.04` runner images.\nMitigation ways\nUse `ubuntu-26.04`.\n"
        self.assertEqual(set(parse_announcements([issue(5, body=body)])), {"ubuntu-22.04"})

    def test_inline_migration_recommendations_do_not_deprecate_replacements(self):
        """Migration advice in the same sentence never retires the suggested target."""
        for text in (
            "The `ubuntu-22.04` runner images will be retired. Please migrate jobs to `ubuntu-24.04`.",
            "The `ubuntu-22.04` runner images will be retired; migrate jobs to `ubuntu-24.04`.",
            "Deprecation of `ubuntu-22.04` runner images, please use `ubuntu-24.04` instead.",
            "The `ubuntu-22.04` runner images will be retired in favor of `ubuntu-24.04`.",
        ):
            body = f"### Breaking changes\n{text}\n### Target date\nRetirement: April 17th, 2027\n"
            with self.subTest(text=text):
                rows = parse_announcements([issue(6, body=body)])
                self.assertEqual(set(rows), {"ubuntu-22.04"})
                self.assertEqual(rows["ubuntu-22.04"]["retirement"], "2027-04-17")
                self.assertFalse(rows["ubuntu-22.04"]["needs_review"])

    def test_multiple_generations_without_clear_direction_require_review(self):
        """Ambiguous prose about multiple generations cannot confirm retirement dates."""
        body = "### Breaking changes\nRetirement changes for `ubuntu-22.04` and `ubuntu-24.04` runner images.\n### Target date\nRetirement: April 17th, 2027\n"
        rows = parse_announcements([issue(7, body=body)])
        self.assertEqual(set(rows), {"ubuntu-22.04", "ubuntu-24.04"})
        self.assertTrue(all(row["needs_review"] for row in rows.values()))
        self.assertTrue(all(row["retirement"] is None for row in rows.values()))

    def test_noncanonical_sources_and_pull_requests_are_ignored(self):
        """Untrusted URLs, mismatched identities and non-announcement issues are ignored."""
        valid = issue(14254, "announcement-14254.md")
        for changes in (
            {"html_url": "https://evil.example/issues/14254"},
            {"html_url": "https://github.com/actions/runner-images/issues/14254?x=1"},
            {"pull_request": {}},
            {"number": 99},
            {"labels": [{"name": "Bug"}]},
            {"labels": 42},
        ):
            with self.subTest(changes=changes):
                self.assertEqual(parse_announcements([{**valid, **changes}]), {})

    def test_conflicting_retirement_announcements_need_review(self):
        """Conflicting official notices retain evidence but clear the disputed date."""
        first = issue(14254, "announcement-14254.md")
        second = issue(14255, body=first["body"].replace("April 17th, 2027", "April 18th, 2027"))
        rows = parse_announcements([second, first])
        self.assertIsNone(rows["ubuntu-22.04"]["retirement"])
        self.assertTrue(rows["ubuntu-22.04"]["needs_review"])


if __name__ == "__main__":
    unittest.main()
