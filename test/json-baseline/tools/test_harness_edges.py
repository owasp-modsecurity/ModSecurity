#!/usr/bin/env python3
"""Regression tests for output paths, audit reports and offset contracts."""
import contextlib
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

from output_directory import create_output_directory

BASELINE = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(BASELINE))
import run_audit
import run_extended_audit as extended


class OutputDirectoryTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.source = self.root / 'source'
        self.build = self.root / 'engine-build'
        self.source.mkdir()
        self.build.mkdir()

    def create(self, target):
        return create_output_directory(target, [self.source, self.build])

    def test_new_external_directory(self):
        target = self.root / 'new parent' / 'output'
        self.assertEqual(self.create(target), target.resolve())
        self.assertTrue(target.is_dir())

    def test_existing_file_is_not_overwritten(self):
        target = self.root / 'existing'
        target.write_text('keep this')
        with self.assertRaises(ValueError):
            self.create(target)
        self.assertEqual(target.read_text(), 'keep this')

    def test_existing_directory_is_not_reused(self):
        target = self.root / 'existing'
        target.mkdir()
        with self.assertRaises(ValueError):
            self.create(target)

    def test_source_and_build_descendants_rejected_before_creation(self):
        for root in (self.source, self.build):
            with self.subTest(root=root):
                target = root / 'new' / 'output'
                with self.assertRaises(ValueError):
                    self.create(target)
                self.assertFalse((root / 'new').exists())

    def test_source_root_itself_rejected(self):
        with self.assertRaises(ValueError):
            self.create(self.source)

    def test_similar_name_outside_source_allowed(self):
        self.assertTrue(self.create(self.root / 'source-other').is_dir())

    def test_existing_and_dangling_symlinks_rejected(self):
        for destination in (self.source, self.root / 'missing'):
            target = self.root / ('link-' + destination.name)
            target.symlink_to(destination, target_is_directory=True)
            with self.subTest(destination=destination):
                with self.assertRaises(ValueError):
                    self.create(target)
                self.assertTrue(target.is_symlink())

    def test_symlinked_ancestor_into_source_rejected(self):
        link = self.root / 'link'
        link.symlink_to(self.source, target_is_directory=True)
        with self.assertRaises(ValueError):
            self.create(link / 'new-output')
        self.assertFalse((self.source / 'new-output').exists())

    def test_parent_traversal_into_source_rejected(self):
        outside = self.root / 'outside'
        outside.mkdir()
        with self.assertRaises(ValueError):
            self.create(outside / '..' / 'source' / 'output')
        self.assertFalse((self.source / 'output').exists())


class ShellEntryPointTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.repo = self.root / 'repository with spaces'
        self.scripts = self.repo / 'test/json-baseline'
        (self.scripts / 'tools').mkdir(parents=True)
        for name in ('build-probes.sh', 'build-extended-audit.sh'):
            shutil.copy2(BASELINE / name, self.scripts / name)
        shutil.copy2(BASELINE / 'tools/output_directory.py',
                     self.scripts / 'tools/output_directory.py')
        self.log = self.root / 'compiler-called'
        commands = self.root / 'commands'
        commands.mkdir()
        compiler = commands / 'fake-cxx'
        compiler.write_text('#!/bin/sh\nprintf "called\\n" >> "$COMPILER_LOG"\n')
        compiler.chmod(0o755)
        pkg = commands / 'pkg-config'
        pkg.write_text('#!/bin/sh\ncase "$1" in\n'
                       '  --variable=includedir) printf "/usr/include/yajl\\n" ;;\n'
                       'esac\n')
        pkg.chmod(0o755)
        self.env = dict(os.environ, CXX=str(compiler), COMPILER_LOG=str(self.log),
                        PATH=str(commands) + os.pathsep + os.environ.get('PATH', ''))

    def run_script(self, name, output, *extra):
        return subprocess.run(['sh', str(self.scripts / name), str(output), *map(str, extra)],
                              env=self.env, text=True, capture_output=True, timeout=10)

    def test_both_scripts_reject_in_source_output_before_compilation(self):
        for name in ('build-probes.sh', 'build-extended-audit.sh'):
            with self.subTest(name=name):
                output = self.repo / ('forbidden-' + name)
                result = self.run_script(name, output)
                self.assertNotEqual(result.returncode, 0)
                self.assertIn('outside', result.stderr)
                self.assertFalse(output.exists())
                self.assertFalse(self.log.exists())

    def test_extended_script_protects_separate_build_root(self):
        build = self.root / 'separate-build'
        build.mkdir()
        result = self.run_script('build-extended-audit.sh', build / 'output', build)
        self.assertNotEqual(result.returncode, 0)
        self.assertFalse((build / 'output').exists())
        self.assertFalse(self.log.exists())

    def test_both_scripts_accept_new_external_directory(self):
        for name, calls in (('build-probes.sh', 2), ('build-extended-audit.sh', 1)):
            with self.subTest(name=name):
                self.log.unlink(missing_ok=True)
                output = self.root / ('external output ' + name)
                result = self.run_script(name, output)
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertTrue(output.is_dir())
                self.assertEqual(len(self.log.read_text().splitlines()), calls)


class AuditReportTests(unittest.TestCase):
    def test_missing_report_parent_is_created_and_failures_retained(self):
        with tempfile.TemporaryDirectory() as directory:
            report = Path(directory) / 'nested' / 'new' / 'report.json'
            failed = subprocess.CompletedProcess([], 2, b'', b'synthetic failure')
            with patch.object(sys, 'argv', ['run_audit.py', '/unused-probe', '--report', str(report)]), \
                    patch.object(run_audit.subprocess, 'run', return_value=failed), \
                    contextlib.redirect_stdout(io.StringIO()):
                result = run_audit.main()
            content = json.loads(report.read_text())
            self.assertEqual(result, 1)
            self.assertEqual(len(content['cases']), 12)
            self.assertEqual(content['counts']['FAIL'], 12)
            self.assertEqual(content['counts']['XFAIL'], 0)


class OffsetContractTests(unittest.TestCase):
    MALFORMED = b'0\n{"match":[{"variable":{"highlight":[],"value":[{"value":""}],"operator":{}}]}]\n'

    def corrected(self):
        return {'match': [{'variable': {
            'highlight': [{'startingAt': '1', 'size': '3'}],
            'value': [{'value': 'BCD'}], 'operator': {'value': 'C'}}}]}

    @staticmethod
    def response(document):
        return b'0\n' + json.dumps(document).encode() + b'\n'

    def test_malformed_bytes_remain_observable_but_fail_requirement(self):
        self.assertEqual(extended.check_offset_response(self.MALFORMED, 'observation'),
                         self.MALFORMED.partition(b'\n')[2])
        with self.assertRaises(ValueError):
            extended.check_offset_response(self.MALFORMED, 'single')

    def test_correct_output_can_pass_both_contracts(self):
        raw = self.response(self.corrected())
        extended.check_offset_response(raw, 'observation')
        extended.check_offset_response(raw, 'single')

    def test_wrong_value_still_fails_requirement(self):
        document = self.corrected()
        document['match'][0]['variable']['value'][0]['value'] = 'WRONG'
        with self.assertRaises(AssertionError):
            extended.check_offset_response(self.response(document), 'single')

    def test_wrong_range_still_fails_requirement(self):
        document = self.corrected()
        document['match'][0]['variable']['highlight'][0]['size'] = '2'
        with self.assertRaises(AssertionError):
            extended.check_offset_response(self.response(document), 'single')

    def test_wrong_status_missing_framing_and_empty_output_fail_observation(self):
        for raw in (b'0', b'0\n', b'-1\nerror\n'):
            with self.subTest(raw=raw), self.assertRaises(AssertionError):
                extended.check_offset_response(raw, 'observation')

    def test_fixed_out_of_bounds_error_still_checked(self):
        expected = b'Offset is out of the content limits.\n'
        extended.check_offset_response(b'-1\n' + expected, 'error', expected)
        with self.assertRaises(AssertionError):
            extended.check_offset_response(b'-1\nwrong\n', 'error', expected)

    def test_unknown_contract_is_not_silently_accepted(self):
        with self.assertRaises(ValueError):
            extended.check_offset_response(self.response(self.corrected()), 'typo')

    def test_report_preserves_raw_offset_observation(self):
        def run_probe(probe, args):
            if args[2] == 'v1,3o1,1':
                return self.MALFORMED
            return b'-1\nOffset is out of the content limits.\n'
        with tempfile.TemporaryDirectory() as directory:
            report = Path(directory) / 'report.json'
            with patch.object(sys, 'argv', ['run_extended_audit.py', '/unused-probe',
                                           '--report', str(report), '--suite', 'characterization']), \
                    patch.object(extended, 'production_commit', return_value=None), \
                    patch.object(extended, 'AUDIT_CASES', []), \
                    patch.object(extended, 'run', side_effect=run_probe), \
                    contextlib.redirect_stdout(io.StringIO()):
                extended.main()
            result = next(case for case in json.loads(report.read_text())['cases']
                          if case['id'] == 'OFFSET-C01')
            self.assertEqual(result['status'], 'PASS')
            self.assertEqual(result['output_hex'], self.MALFORMED.hex())


if __name__ == '__main__':
    unittest.main()
