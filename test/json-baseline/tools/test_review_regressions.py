#!/usr/bin/env python3
"""Synthetic regression controls for PR review findings; not engine results."""
import contextlib
import copy
import io
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

import build
import run_all
import test_verification as verification

BASELINE = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(BASELINE))
import run as request
import run_extended_audit as audit


def framed(variable):
    return b'0\n' + json.dumps({'match': [{'variable': variable}]}).encode() + b'\n'


class OffsetStructureTests(unittest.TestCase):
    def single(self):
        return {'highlight': [{'size': '3', 'startingAt': '1'}],
                'value': [{'value': 'BCD'}], 'operator': {'value': 'C'}}

    def transformed(self):
        return {'value': [{'value': 'ABCDEF'},
                          {'value': 'abcdef', 'transformation': 't:lowercase'}]}

    def test_highlight_members_can_be_reordered(self):
        audit.check_offset_response(framed(self.single()), 'single')

    def test_transformation_members_can_be_reordered(self):
        audit.check_offset_response(framed(self.transformed()), 'transform')

    def test_wrong_highlight_value_is_rejected(self):
        value = self.single()
        value['highlight'][0]['size'] = '4'
        with self.assertRaises(AssertionError):
            audit.check_offset_response(framed(value), 'single')

    def test_duplicate_object_member_is_rejected(self):
        raw = framed(self.single()).replace(b'"size": "3"',
                                             b'"size": "3", "size": "3"')
        with self.assertRaises(AssertionError):
            audit.check_offset_response(raw, 'single')

    def test_object_cannot_replace_array(self):
        value = self.single()
        value['highlight'] = {'0': value['highlight'][0]}
        with self.assertRaises(AssertionError):
            audit.check_offset_response(framed(value), 'single')

    def test_transformation_array_order_stays_significant(self):
        value = self.transformed()
        value['value'].reverse()
        with self.assertRaises(AssertionError):
            audit.check_offset_response(framed(value), 'transform')

    def test_transformation_value_stays_exact(self):
        value = self.transformed()
        value['value'][1]['value'] = 'abcdef abcdef'
        with self.assertRaises(AssertionError):
            audit.check_offset_response(framed(value), 'transform')

    def test_requirement_invokes_one_transformation_range(self):
        calls = []

        def probe(_executable, args):
            calls.append(args)
            if args[-1] == '':
                return b'0\n{}\n'
            if args[-1] == 'v1,3o1,1':
                return framed(self.single())
            self.assertEqual(args[-1], 'v0,6t:lowercase')
            return framed(self.transformed())

        with tempfile.TemporaryDirectory() as directory:
            report = Path(directory) / 'report.json'
            argv = ['run_extended_audit.py', 'synthetic-probe', '--report',
                    str(report), '--suite', 'requirements']
            with patch.object(sys, 'argv', argv), \
                 patch.object(audit, 'AUDIT_CASES', []), \
                 patch.object(audit, 'production_commit', return_value=None), \
                 patch.object(audit, 'run', side_effect=probe), \
                 contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(audit.main(), 0)
            self.assertEqual(len(calls), 3)
            result = json.loads(report.read_text())
            self.assertEqual(result['counts']['PASS'], 3)
            self.assertEqual(result['cases'][-1]['match_string'], 'v0,6t:lowercase')


class RepeatTransactionTests(unittest.TestCase):
    def setUp(self):
        self.case = next(c for c in request.cases() if c.id == 'K-repeat-error')
        tx = {'ARGS': self.case.args.copy(),
              'NAMES': [(key, key) for key, value in self.case.args],
              'GET': [], 'POST': [], 'ERROR': b'1', 'PROCESSOR_ERROR': b'1',
              'PHASE2': b'1', 'PROCESSOR': b'JSON',
              'ERROR_MSG': b'JSON parsing error: parse error: synthetic'}
        self.record = {**copy.deepcopy(tx), 'CONFIG': 1, 'YAJL_ACTIVE': 1,
                       'INBOUND': b'0', 'VALUE_SEEN': b'', 'STATUSES': [200, 200],
                       'TRANSACTIONS': [copy.deepcopy(tx), copy.deepcopy(tx)]}
        for key in ('ARGS', 'NAMES', 'GET', 'POST'):
            self.record[key] *= 2

    def test_both_correct_transactions_pass(self):
        self.assertEqual(request.check(self.case, self.record), [])

    def test_first_error_flag_cannot_be_hidden_by_last_transaction(self):
        self.record['TRANSACTIONS'][0]['ERROR'] = b'0'
        errors = request.check(self.case, self.record)
        self.assertTrue(any('transaction 0 ERROR:' in e for e in errors))

    def test_first_processor_error_cannot_be_hidden(self):
        self.record['TRANSACTIONS'][0]['PROCESSOR_ERROR'] = b'0'
        self.assertTrue(request.check(self.case, self.record))

    def test_arguments_cannot_shift_between_transactions(self):
        self.record['TRANSACTIONS'][0]['ARGS'] = []
        self.record['TRANSACTIONS'][1]['ARGS'] *= 2
        self.assertTrue(request.check(self.case, self.record))

    def test_first_error_message_is_checked(self):
        self.record['TRANSACTIONS'][0]['ERROR_MSG'] = b''
        self.assertTrue(request.check(self.case, self.record))

    def test_missing_or_extra_transaction_is_rejected(self):
        for count in (0, 1, 3):
            with self.subTest(count=count):
                record = copy.deepcopy(self.record)
                record['TRANSACTIONS'] = [copy.deepcopy(self.record['TRANSACTIONS'][0])
                                          for _ in range(count)]
                self.assertTrue(request.check(self.case, record))

    def test_each_transaction_collection_is_checked(self):
        for field in ('NAMES', 'GET', 'POST'):
            with self.subTest(field=field):
                record = copy.deepcopy(self.record)
                record['TRANSACTIONS'][0][field] = [(b'wrong', b'wrong')]
                self.assertTrue(request.check(self.case, record))


class UntrackedSourceTests(unittest.TestCase):
    def setUp(self):
        self.fixture = verification.SourceVerificationTests()
        self.fixture.setUp()
        self.addCleanup(self.fixture.doCleanups)
        self.root = self.fixture.root
        self.additions = {}
        self.nested = None

    def git(self, source, *args):
        if args == ('ls-files', '--others', '--exclude-standard', '-z', '--'):
            return self.additions.get(Path(source), '')
        result = self.fixture.git(source, *args)
        if args[:2] == ('submodule', 'status') and self.nested:
            result += '\n ' + verification.MODULE + ' ' + self.nested + ' (pinned)'
        return result

    def metadata(self):
        with patch.object(build, 'git', side_effect=self.git):
            return build.source_metadata(self.root, verification.CURRENT)

    def test_untracked_production_header_is_blocked(self):
        self.additions[self.root] = 'headers/shadow.h\0'
        with self.assertRaisesRegex(build.Blocked, 'Untracked production'):
            self.metadata()

    def test_untracked_ancillary_file_is_allowed(self):
        self.additions[self.root] = 'doc/notes.md\0test/new-case.py\0'
        self.assertTrue(self.metadata()['production_matches_original'])

    def test_untracked_direct_submodule_file_is_blocked(self):
        self.additions[self.root / build.SUBMODULES[0]] = 'shadow.h\0'
        with self.assertRaisesRegex(build.Blocked, 'Submodule has untracked'):
            self.metadata()

    def test_untracked_nested_submodule_file_is_blocked(self):
        self.nested = 'others/mbedtls/framework'
        (self.root / self.nested).mkdir()
        self.additions[self.root / self.nested] = 'shadow.h\0'
        with self.assertRaisesRegex(build.Blocked, 'Recursive submodule has untracked'):
            self.metadata()

    @unittest.skipUnless(shutil.which('git'), 'git is required')
    def test_real_git_preserves_unusual_names_and_ignores_build_products(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            subprocess.run(['git', 'init', '-q', str(root)], check=True, timeout=10)
            (root / '.gitignore').write_text('*.o\n')
            name = 'new header\nwith newline.h'
            (root / name).touch()
            (root / 'generated.o').touch()
            names = build.untracked_files(root)
            self.assertIn(name, names)
            self.assertNotIn('generated.o', names)


class CanaryExitTests(unittest.TestCase):
    def test_harness_errors_are_failures_even_without_canary_record(self):
        for code in (1, 2, 127, -11):
            with self.subTest(code=code):
                result = subprocess.CompletedProcess([], code, '', 'synthetic error')
                with patch.object(build.subprocess, 'run', return_value=result):
                    with self.assertRaises(RuntimeError) as caught:
                        build.active_canary(Path('probe'), {})
                self.assertNotIsInstance(caught.exception, build.Blocked)

    def test_exit_77_remains_blocked(self):
        result = subprocess.CompletedProcess([], 77, '', 'JSON unavailable')
        with patch.object(build.subprocess, 'run', return_value=result):
            with self.assertRaises(build.Blocked):
                build.active_canary(Path('probe'), {})


class NativeAccountingTests(unittest.TestCase):
    def test_disabled_fixture_is_blocked_and_accounted(self):
        counts = run_all.native_counts([{'enabled': 1}, {'enabled': 0}], 1, 0, 0, 1)
        self.assertEqual(counts, dict(PASS=1, FAIL=0, SKIP=0, BLOCKED=1, XFAIL=0))

    def test_skipped_fixture_is_accounted(self):
        counts = run_all.native_counts([{}, {}], 1, 0, 1, 0)
        self.assertEqual(counts['SKIP'], 1)

    def test_inconsistent_native_counts_are_rejected(self):
        for numbers in ((1, 0, 0, 0), (3, 0, 0, 0), (2, 3, 0, 0), (2, -1, 0, 0)):
            with self.subTest(numbers=numbers):
                with self.assertRaises(ValueError):
                    run_all.native_counts([{}, {}], *numbers)

    def test_disabled_native_case_does_not_become_infrastructure_failure(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            tests = root / 'test-repo/test/json-baseline'
            fixture = tests.parent / 'test-cases/regression/request-body-parser-json.json'
            fixture.parent.mkdir(parents=True)
            fixture.write_text(json.dumps([{'enabled': 1}, {'enabled': 0}]))
            probes, output, source = root / 'probes', root / 'output', root / 'source'
            probes.mkdir()
            metadata = {'source_root': str(source), 'original_commit': verification.CURRENT}
            manifest = {'library': {'path': str(source / 'src/.libs/libmodsecurity.so')}}

            def execute(command, **kwargs):
                if command[0] == str(source / 'test/regression_tests'):
                    kwargs['stdout'].write('Ran a total of: 1 regression tests - '
                                           'All tests passed.\n0 skipped test(s).\n'
                                           '1 disabled test(s).\n')
                else:
                    flag = '--report' if '--report' in command else '--output'
                    report = Path(command[command.index(flag) + 1])
                    counts = run_all.zeros()
                    counts['PASS'] = 1
                    report.write_text(json.dumps({'counts': counts,
                                                   'cases': [{'status': 'PASS'}]}))
                return subprocess.CompletedProcess(command, 0)

            argv = ['run_all.py', '--probes', str(probes), '--output', str(output),
                    '--profile', 'characterization']
            with patch.object(sys, 'argv', argv), patch.object(run_all, 'TESTS', tests), \
                 patch.object(run_all, 'verify_manifest', return_value=(manifest, metadata, {}, {})), \
                 patch.object(run_all, 'linked_library', return_value={}), \
                 patch.object(run_all.subprocess, 'run', side_effect=execute), \
                 contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(run_all.main(), 1)
            summary = json.loads((output / 'summary.json').read_text())
            native = next(s for s in summary['suites'] if s['name'] == 'native-json')
            self.assertEqual(native['status'], 'BLOCKED')
            self.assertEqual(native['counts']['BLOCKED'], 1)
            self.assertEqual(native['counts']['FAIL'], 0)
            self.assertNotIn('reason', native)
            self.assertEqual(summary['counts']['FAIL'], 0)
            report = json.loads((output / 'native-json.json').read_text())
            self.assertEqual(report['fixture_cases'], 2)
            self.assertEqual(report['enabled_cases'], 1)


class NoYajlProbeTests(unittest.TestCase):
    @unittest.skipUnless(shutil.which('c++'), 'C++ compiler is required')
    def test_no_yajl_probe_builds_without_json_only_code(self):
        with tempfile.TemporaryDirectory() as directory:
            executable = Path(directory) / 'probe'
            compiled = subprocess.run(['c++', '-std=c++17', '-Wall', '-Wextra', '-Werror',
                                       '-UWITH_YAJL', str(BASELINE / 'probe.cc'),
                                       '-o', str(executable)],
                                      capture_output=True, timeout=60, check=False)
            self.assertEqual(compiled.returncode, 0, compiled.stderr.decode(errors='replace'))
            result = subprocess.run([str(executable)], input=b'', capture_output=True,
                                    timeout=10, check=False)
            self.assertEqual(result.returncode, 77)
            self.assertEqual(result.stdout, b'')
            self.assertEqual(result.stderr, b'BLOCKED: JSON support was not enabled\n')


if __name__ == '__main__':
    unittest.main()
