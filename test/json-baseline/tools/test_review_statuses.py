#!/usr/bin/env python3
"""Review regressions for empty suites and unavailable/failing test probes."""
import contextlib
import io
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

import build
import run_all
from result_validation import native_counts, validated_counts, zeros

BASELINE = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(BASELINE))
import run as request
import run_audit as audit


class SuiteAccountingTests(unittest.TestCase):
    def test_passing_controls_cannot_replace_behavior_cases(self):
        counts = zeros()
        counts['PASS'] = 1
        report = {'counts': counts, 'cases': [],
                  'sensitivity': [{'status': 'PASS'}]}
        with self.assertRaises(ValueError):
            validated_counts(report)

    def test_missing_cases_cannot_be_replaced_by_controls(self):
        counts = zeros()
        counts['PASS'] = 1
        report = {'counts': counts, 'sensitivity': [{'status': 'PASS'}]}
        with self.assertRaises(ValueError):
            validated_counts(report)

    def test_behavior_case_and_control_remain_separate_counts(self):
        counts = zeros()
        counts.update(PASS=1, FAIL=1)
        report = {'counts': counts, 'cases': [{'status': 'PASS'}],
                  'sensitivity': [{'status': 'FAIL'}]}
        self.assertEqual(validated_counts(report), counts)

    def test_real_behavior_failure_is_not_hidden(self):
        counts = zeros()
        counts['FAIL'] = 1
        report = {'counts': counts, 'cases': [{'status': 'FAIL'}]}
        self.assertEqual(validated_counts(report), counts)

    def test_empty_native_fixture_is_rejected(self):
        with self.assertRaises(ValueError):
            native_counts([], 0, 0, 0, 0)

    def test_native_fixture_must_be_an_array_of_objects(self):
        for fixture in (None, {}, 'x', [None], [5]):
            with self.subTest(fixture=fixture):
                with self.assertRaises(ValueError):
                    native_counts(fixture, 1, 0, 0, 0)

    def test_disabled_native_case_remains_blocked(self):
        self.assertEqual(native_counts([{'enabled': 0}], 0, 0, 0, 1),
                         dict(PASS=0, FAIL=0, SKIP=0, BLOCKED=1, XFAIL=0))

    def test_native_failure_and_skip_remain_visible(self):
        self.assertEqual(native_counts([{}, {}], 1, 1, 1, 0),
                         dict(PASS=0, FAIL=1, SKIP=1, BLOCKED=0, XFAIL=0))


class AuditDependencyTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.directory = Path(temporary.name)
        self.probe = self.directory / 'probe'
        self.report = self.directory / 'nested/report.json'

    def invoke(self):
        arguments = ['run_audit.py', str(self.probe), '--report', str(self.report)]
        with patch.object(sys, 'argv', arguments), contextlib.redirect_stdout(io.StringIO()):
            code = audit.main()
        return code, json.loads(self.report.read_text())

    def assert_all(self, status):
        code, report = self.invoke()
        self.assertEqual(code, 1)
        self.assertEqual(report['counts'][status], 12)
        self.assertEqual(len(report['cases']), 12)
        self.assertTrue(all(case['status'] == status for case in report['cases']))
        self.assertEqual(report['counts']['XFAIL'], 0)

    def test_real_missing_probe_and_dependent_controls_are_blocked(self):
        self.assert_all('BLOCKED')

    def test_real_non_executable_probe_is_blocked(self):
        self.probe.write_text('#!/bin/sh\nexit 0\n')
        self.probe.chmod(0o600)
        self.assert_all('BLOCKED')

    def test_real_json_unavailable_exit_is_blocked(self):
        self.probe.write_text('#!/bin/sh\nexit 77\n')
        self.probe.chmod(0o700)
        self.assert_all('BLOCKED')

    def test_loader_dependency_failure_is_blocked(self):
        result = subprocess.CompletedProcess([], 127, b'', b'error while loading shared libraries: missing')
        with patch.object(audit.subprocess, 'run', return_value=result):
            self.assert_all('BLOCKED')

    def test_other_exit_127_is_a_failure(self):
        result = subprocess.CompletedProcess([], 127, b'', b'unrelated error')
        with patch.object(audit.subprocess, 'run', return_value=result):
            self.assert_all('FAIL')

    def test_harness_exit_is_a_failure(self):
        result = subprocess.CompletedProcess([], 2, b'', b'synthetic harness error')
        with patch.object(audit.subprocess, 'run', return_value=result):
            self.assert_all('FAIL')

    def test_timeout_is_a_failure(self):
        error = subprocess.TimeoutExpired('probe', 10)
        with patch.object(audit.subprocess, 'run', side_effect=error):
            self.assert_all('FAIL')

    def test_malformed_output_is_a_failure(self):
        result = subprocess.CompletedProcess([], 0, b'{not json}\n', b'')
        with patch.object(audit.subprocess, 'run', return_value=result):
            self.assert_all('FAIL')

    def test_probe_signal_is_a_failure(self):
        result = subprocess.CompletedProcess([], -11, b'', b'')
        with patch.object(audit.subprocess, 'run', return_value=result):
            self.assert_all('FAIL')


class SensitivityReportTests(unittest.TestCase):
    def run_with_control_error(self, error):
        available = request.cases()
        observed_count = 0
        checked_count = 0

        def observe(probe, case):
            nonlocal observed_count
            observed_count += 1
            if observed_count == len(available) + 1:
                raise error
            return {}, 'synthetic observation'

        def check(case, record):
            nonlocal checked_count
            checked_count += 1
            return [] if checked_count <= len(available) else ['false expectation rejected']

        with tempfile.TemporaryDirectory() as directory:
            report_path = Path(directory) / 'report.json'
            arguments = ['run.py', '--probe', 'synthetic', '--sensitivity',
                         '--output', str(report_path)]
            with patch.object(sys, 'argv', arguments), \
                 patch.object(request, 'observe', side_effect=observe), \
                 patch.object(request, 'check', side_effect=check), \
                 contextlib.redirect_stdout(io.StringIO()):
                code = request.main()
            report = json.loads(report_path.read_text())
        self.assertEqual(code, 1)
        self.assertEqual(len(report['cases']), len(available))
        self.assertTrue(all(case['status'] == 'PASS' for case in report['cases']))
        self.assertEqual([item['status'] for item in report['sensitivity']],
                         ['FAIL', 'PASS', 'PASS', 'PASS'])
        self.assertIn('control did not reach an assertion',
                      report['sensitivity'][0]['errors'][0])
        self.assertEqual(report['counts']['PASS'], len(available) + 3)
        self.assertEqual(report['counts']['FAIL'], 1)
        self.assertEqual(validated_counts(report), report['counts'])

    def test_control_timeout_preserves_completed_report(self):
        self.run_with_control_error(subprocess.TimeoutExpired('probe', 10))

    def test_control_execution_error_preserves_completed_report(self):
        self.run_with_control_error(RuntimeError('probe exit=2'))

    def test_control_disappearance_is_not_a_successful_mutation(self):
        self.run_with_control_error(FileNotFoundError('probe disappeared'))

    def test_control_protocol_error_preserves_completed_report(self):
        self.run_with_control_error(ValueError('malformed protocol'))



class ProbeHeaderIntegrityTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        root = Path(temporary.name)
        self.probes = root / 'probes'
        self.tests = root / 'tests'
        self.source = root / 'source'
        for folder in (self.probes, self.tests, self.source):
            folder.mkdir()
        self.library = self.source / 'library.so'
        self.library.write_bytes(b'library')
        (self.probes / 'probe').write_bytes(b'probe')
        (self.tests / 'probe.cc').write_bytes(b'probe source')
        for header in build.PROBE_HEADERS:
            (self.tests / header).write_text(header)
        self.metadata = {'source_root': str(self.source), 'source_head': 'a' * 40,
                         'original_commit': 'a' * 40, 'submodules': {}}
        self.manifest = {
            **self.metadata, 'schema': 2, 'status': 'PASS',
            'library': {'path': str(self.library), 'directory': str(self.source),
                        'sha256': build.sha256(self.library)},
            'probes': {'probe': {
                'sha256': build.sha256(self.probes / 'probe'),
                'source_sha256': build.sha256(self.tests / 'probe.cc'),
                'header_sha256': {name: build.sha256(self.tests / name)
                                  for name in build.PROBE_HEADERS}}},
        }

    def verify(self):
        (self.probes / 'build-manifest.json').write_text(json.dumps(self.manifest))
        with patch.object(run_all, 'PROBES', {'probe': 'probe.cc'}), \
             patch.object(run_all, 'TESTS', self.tests), \
             patch.object(run_all, 'verify_engine_build', return_value={}), \
             patch.object(run_all, 'source_metadata', return_value=self.metadata), \
             patch.object(run_all, 'linked_library', return_value={
                 'sha256': build.sha256(self.library)}), \
             patch.object(run_all, 'active_canary', return_value=True):
            return run_all.verify_manifest(self.probes)

    def test_unchanged_common_headers_pass_preflight(self):
        self.assertEqual(self.verify()[1], self.metadata)

    def test_changed_common_header_cannot_reuse_old_probe(self):
        for name in build.PROBE_HEADERS:
            with self.subTest(header=name):
                path = self.tests / name
                content = path.read_text()
                path.write_text('changed')
                with self.assertRaisesRegex(build.Blocked, 'Probe headers'):
                    self.verify()
                path.write_text(content)

    def test_missing_header_evidence_requires_rebuild(self):
        del self.manifest['probes']['probe']['header_sha256']
        with self.assertRaisesRegex(build.Blocked, 'Probe headers'):
            self.verify()


if __name__ == '__main__':
    unittest.main()
