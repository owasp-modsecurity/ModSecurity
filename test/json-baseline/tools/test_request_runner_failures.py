#!/usr/bin/env python3
"""Request runner launch/status regressions; no ModSecurity build required."""
import contextlib
import io
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

BASELINE = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(BASELINE))
import run as baseline
import run_extended as extended

RUNNERS = ((baseline, 'A-object'), (extended, 'PR-N-token-kinds'))


class RequestRunnerFailures(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.probe = self.root / 'probe'
        self.report = self.root / 'report.json'

    def invoke(self, runner, case_filter, *extra):
        args = [runner.__name__, '--probe', str(self.probe), '--output',
                str(self.report), '--filter', case_filter, '--sensitivity', *extra]
        with patch.object(sys, 'argv', args), \
             patch.object(extended, 'production_commit', return_value=None), \
             contextlib.redirect_stdout(io.StringIO()):
            code = runner.main()
        return code, json.loads(self.report.read_text())

    def assert_status(self, runner, case_filter, status):
        code, report = self.invoke(runner, case_filter)
        self.assertEqual(code, 1)
        self.assertEqual(report['counts'], {
            'PASS': 0, 'FAIL': int(status == 'FAIL'), 'SKIP': 0,
            'BLOCKED': int(status == 'BLOCKED'), 'XFAIL': 0})
        self.assertEqual(len(report['cases']), 1)
        self.assertEqual(report['cases'][0]['status'], status)
        self.assertEqual(report['sensitivity'], [])
        return report

    def executable(self, body):
        self.probe.write_text('#!/bin/sh\n' + body + '\n')
        self.probe.chmod(0o700)

    def test_real_missing_probe_writes_blocked_reports(self):
        for runner, case_filter in RUNNERS:
            with self.subTest(runner=runner.__name__):
                self.assert_status(runner, case_filter, 'BLOCKED')

    def test_real_permission_error_writes_blocked_reports(self):
        self.probe.write_text('#!/bin/sh\nexit 0\n')
        self.probe.chmod(0o600)
        for runner, case_filter in RUNNERS:
            with self.subTest(runner=runner.__name__):
                report = self.assert_status(runner, case_filter, 'BLOCKED')
                if runner is extended:
                    self.assertEqual(report['cases'][0]['failure_kind'], 'probe-unavailable')

    def test_real_invalid_executable_is_blocked(self):
        self.probe.write_text('Not an executable format\n')
        self.probe.chmod(0o700)
        for runner, case_filter in RUNNERS:
            with self.subTest(runner=runner.__name__):
                self.assert_status(runner, case_filter, 'BLOCKED')

    def test_exit_77_does_not_run_sensitivity_controls(self):
        self.executable('exit 77')
        for runner, case_filter in RUNNERS:
            with self.subTest(runner=runner.__name__), \
                 patch.object(runner.subprocess, 'run', wraps=subprocess.run) as launched:
                self.assert_status(runner, case_filter, 'BLOCKED')
                self.assertEqual(launched.call_count, 1)

    def test_real_nonzero_exit_stays_fail(self):
        self.executable('exit 2')
        for runner, case_filter in RUNNERS:
            with self.subTest(runner=runner.__name__), \
                 patch.object(runner.subprocess, 'run', wraps=subprocess.run) as launched:
                self.assert_status(runner, case_filter, 'FAIL')
                self.assertEqual(launched.call_count, 1)

    def test_timeout_stays_fail_without_controls(self):
        for runner, case_filter in RUNNERS:
            error = subprocess.TimeoutExpired('probe', 10)
            with self.subTest(runner=runner.__name__), \
                 patch.object(runner.subprocess, 'run', side_effect=error) as launched:
                self.assert_status(runner, case_filter, 'FAIL')
                self.assertEqual(launched.call_count, 1)

    def test_signal_stays_fail_without_controls(self):
        result = subprocess.CompletedProcess([], -11, '', '')
        for runner, case_filter in RUNNERS:
            with self.subTest(runner=runner.__name__), \
                 patch.object(runner.subprocess, 'run', return_value=result) as launched:
                self.assert_status(runner, case_filter, 'FAIL')
                self.assertEqual(launched.call_count, 1)

    def test_selected_cases_are_all_reported_when_launch_is_blocked(self):
        self.probe.write_text('not executable')
        self.probe.chmod(0o600)
        for runner, case_filter in ((baseline, 'A-'), (extended, 'PR-N-')):
            expected = sum(case_filter in case.id for case in runner.cases())
            with self.subTest(runner=runner.__name__):
                code, report = self.invoke(runner, case_filter)
                self.assertEqual(code, 1)
                self.assertEqual(len(report['cases']), expected)
                self.assertEqual(report['counts']['BLOCKED'], expected)
                self.assertEqual(report['counts']['FAIL'], 0)
                self.assertEqual(report['sensitivity'], [])

    def test_requirements_sensitivity_usage_error_precedes_probe_launch(self):
        args = ['run_extended.py', '--probe', str(self.probe),
                '--suite', 'requirements', '--sensitivity']
        with patch.object(sys, 'argv', args), \
             patch.object(extended, 'observe') as observed, \
             patch.object(extended, 'production_commit') as attributed, \
             contextlib.redirect_stderr(io.StringIO()) as stderr:
            self.assertEqual(extended.main(), 2)
        observed.assert_not_called()
        attributed.assert_not_called()
        self.assertIn('only supported for characterization', stderr.getvalue())

    def test_expanded_assertion_failure_prevents_controls(self):
        with patch.object(extended, 'observe', return_value=({}, [], '')) as observed, \
             patch.object(extended, 'check_extended', return_value=['wrong value']):
            report = self.assert_status(extended, 'PR-N-token-kinds', 'FAIL')
        self.assertEqual(observed.call_count, 1)
        self.assertEqual(report['cases'][0]['failure_kind'], 'assertion')

    def test_expanded_controls_still_run_after_passing_cases(self):
        checks = [[], ['wrong expectation'], ['wrong expectation'],
                  ['wrong expectation'], ['wrong expectation']]
        with patch.object(extended, 'observe', return_value=({}, [], '')) as observed, \
             patch.object(extended, 'check_extended', side_effect=checks):
            code, report = self.invoke(extended, 'PR-N-token-kinds')
        self.assertEqual(code, 0)
        self.assertEqual(observed.call_count, 5)
        self.assertEqual(report['counts']['PASS'], 5)
        self.assertEqual(len(report['sensitivity']), 4)

    def test_control_timeout_after_success_preserves_case_report(self):
        observations = [({}, [], ''), subprocess.TimeoutExpired('probe', 10),
                        ({}, [], ''), ({}, [], ''), ({}, [], '')]
        checks = [[], ['wrong expectation'], ['wrong expectation'], ['wrong expectation']]
        with patch.object(extended, 'observe', side_effect=observations), \
             patch.object(extended, 'check_extended', side_effect=checks):
            code, report = self.invoke(extended, 'PR-N-token-kinds')
        self.assertEqual(code, 1)
        self.assertEqual(report['cases'][0]['status'], 'PASS')
        self.assertEqual(report['counts']['FAIL'], 1)
        self.assertEqual(report['counts']['PASS'], 4)
        self.assertEqual(report['sensitivity'][0]['status'], 'FAIL')


if __name__ == '__main__':
    unittest.main()
