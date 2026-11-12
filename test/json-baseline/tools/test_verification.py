#!/usr/bin/env python3
"""Unit checks for source selection and attribution; no engine build required."""
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

import build
import run_all

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
from report_metadata import production_commit

CURRENT = 'a' * 40
MODULE = 'b' * 40


class SourceVerificationTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name).resolve()
        for relative in ('src', 'headers', 'test/regression', *build.SUBMODULES):
            (self.root / relative).mkdir(parents=True, exist_ok=True)
        for relative in ('configure.ac', 'test/regression/regression.cc'):
            (self.root / relative).touch()
        self.dirty = ''
        self.module = MODULE

    def git(self, source, *args):
        if args == ('rev-parse', '--show-toplevel'):
            return str(self.root)
        if args[:2] == ('rev-parse', '--verify'):
            return args[2].removesuffix('^{commit}')
        if args == ('rev-parse', 'HEAD'):
            return CURRENT if source == self.root else self.module
        if args[:2] == ('diff', '--name-only'):
            if source != self.root:
                return ''
            return 'src/transaction.cc' if args[2] == build.ORIGINAL else self.dirty
        if args[0] == 'ls-tree':
            return f'160000 commit {MODULE}\t{args[-1]}'
        if args[:2] == ('submodule', 'status'):
            return '\n'.join(f' {MODULE} {name} (pinned)' for name in build.SUBMODULES)
        self.fail(f'Unexpected git invocation: {source}: {args}')

    def test_explicit_current_source_accepts_committed_engine_change(self):
        with patch.object(build, 'git', side_effect=self.git):
            metadata = build.source_metadata(self.root, CURRENT)
        self.assertEqual(metadata['original_commit'], CURRENT)
        self.assertEqual(metadata['source_head'], CURRENT)

    def test_default_pinned_replay_rejects_changed_engine(self):
        with patch.object(build, 'git', side_effect=self.git):
            with self.assertRaises(build.Blocked):
                build.source_metadata(self.root)

    def test_current_source_rejects_uncommitted_engine_change(self):
        self.dirty = 'src/transaction.cc'
        with patch.object(build, 'git', side_effect=self.git):
            with self.assertRaises(build.Blocked):
                build.source_metadata(self.root, CURRENT)

    def test_current_source_rejects_wrong_submodule(self):
        self.module = 'c' * 40
        with patch.object(build, 'git', side_effect=self.git):
            with self.assertRaises(build.Blocked):
                build.source_metadata(self.root, CURRENT)

    def test_missing_or_symbolic_ref_is_not_accepted(self):
        for reference in (None, '', 'HEAD', 'abc123'):
            with self.subTest(reference=reference):
                with self.assertRaises(build.Blocked):
                    build.source_metadata(self.root, reference)


class ReportMetadataTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.directory = Path(self.temporary.name)
        self.probe = self.directory / 'probe'
        self.probe.write_bytes(b'test executable identity')
        self.manifest_path = self.directory / 'build-manifest.json'
        self.manifest = {'status': 'PASS', 'original_commit': CURRENT,
                         'probes': {'probe': {'sha256': hashlib.sha256(
                             self.probe.read_bytes()).hexdigest()}}}
        self.write_manifest()
        self.environment = patch.dict(os.environ, {
            'MODSECURITY_JSON_BUILD_MANIFEST': str(self.manifest_path)})
        self.environment.start()
        self.addCleanup(self.environment.stop)

    def write_manifest(self):
        self.manifest_path.write_text(json.dumps(self.manifest))

    def test_report_uses_verified_commit_not_historical_literal(self):
        self.assertEqual(production_commit(self.probe), CURRENT)

    def test_standalone_source_is_unknown(self):
        with patch.dict(os.environ, {}, clear=True):
            self.assertIsNone(production_commit(self.probe))

    def test_changed_probe_is_rejected(self):
        self.probe.write_bytes(b'changed executable')
        with self.assertRaises(ValueError):
            production_commit(self.probe)

    def test_failed_build_is_not_attributed(self):
        self.manifest['status'] = 'FAIL'
        self.write_manifest()
        with self.assertRaises(ValueError):
            production_commit(self.probe)

    def test_bad_commit_is_rejected(self):
        self.manifest['original_commit'] = 'HEAD'
        self.write_manifest()
        with self.assertRaises(ValueError):
            production_commit(self.probe)


class CanaryAndCountsTests(unittest.TestCase):
    def test_exact_active_canary_passes(self):
        result = subprocess.CompletedProcess([], 0, 'YAJL_ACTIVE\t1\n', '')
        with patch.object(build.subprocess, 'run', return_value=result):
            self.assertTrue(build.active_canary(Path('probe'), {}))

    def test_ambiguous_canary_is_blocked(self):
        for text in ('YAJL_ACTIVE\t10\n', 'YAJL_ACTIVE\t1\textra\n',
                     'YAJL_ACTIVE\t1\nYAJL_ACTIVE\t1\n', ''):
            with self.subTest(text=text):
                result = subprocess.CompletedProcess([], 0, text, '')
                with patch.object(build.subprocess, 'run', return_value=result):
                    with self.assertRaises(build.Blocked):
                        build.active_canary(Path('probe'), {})

    def test_real_requirement_failure_is_counted(self):
        counts = run_all.zeros()
        counts['FAIL'] = 1
        self.assertEqual(run_all.validated_counts(
            {'counts': counts, 'cases': [{'status': 'FAIL'}]}), counts)

    def test_requirement_failure_cannot_be_masked_as_xfail(self):
        counts = run_all.zeros()
        counts['XFAIL'] = 1
        with self.assertRaises(ValueError):
            run_all.validated_counts({'counts': counts,
                                      'cases': [{'status': 'XFAIL'}]})

    def test_inaccurate_summary_is_rejected(self):
        counts = run_all.zeros()
        counts['PASS'] = 1
        with self.assertRaises(ValueError):
            run_all.validated_counts({'counts': counts,
                                      'cases': [{'status': 'FAIL'}]})


if __name__ == '__main__':
    unittest.main()
