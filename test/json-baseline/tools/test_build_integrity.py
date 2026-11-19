#!/usr/bin/env python3
"""Build-provenance and malformed-report regression tests; not engine results."""
import contextlib
import ctypes
import io
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

import build
import run_all

SHA = 'a' * 40


def command(arguments, cwd):
    result = subprocess.run(arguments, cwd=cwd, text=True, capture_output=True,
                            timeout=30, check=False)
    if result.returncode:
        raise AssertionError(result.stderr or result.stdout)
    return result.stdout.strip()


@unittest.skipUnless(shutil.which('git') and shutil.which('c++') and shutil.which('make'),
                     'real local Git, compiler and make are required')
class FreshBuildTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name).resolve()
        self.source = self.root / 'source with spaces'
        self.source.mkdir()
        self.output = self.root / 'new build'
        self.output.mkdir()
        for directory in ('src', 'headers', 'test/regression'):
            (self.source / directory).mkdir(parents=True)
        command(['git', 'init', '-q'], self.source)
        command(['git', 'config', 'user.name', 'Build fixture'], self.source)
        command(['git', 'config', 'user.email', 'fixture@example.invalid'], self.source)
        (self.source / '.gitignore').write_text('.libs/\ntest/regression_tests\nsrc/ignored.h\n')
        (self.source / 'configure.ac').touch()
        (self.source / 'headers/version.h').write_text('#define VERSION 1\n')
        (self.source / 'src/value.cc').write_text(
            '#include "version.h"\nextern "C" int version() { return VERSION; }\n')
        (self.source / 'test/regression/regression.cc').write_text(
            '#include <iostream>\n#include "version.h"\n'
            'int main() { std::cout << VERSION << "\\n"; }\n')
        for filename in ('build.sh', 'configure'):
            script = self.source / filename
            script.write_text('#!/bin/sh\nset -eu\nexit 0\n')
            script.chmod(0o755)
        (self.source / 'Makefile').write_text(
            'all:\n\tmkdir -p src/.libs test/.libs\n'
            '\tc++ -shared -fPIC -Iheaders src/value.cc -o src/.libs/libmodsecurity.so\n'
            '\tc++ -Iheaders test/regression/regression.cc -o test/.libs/regression_tests\n'
            '\tprintf \'#!/bin/sh\\nexec "$$(dirname "$$0")/.libs/regression_tests" "$$@"\\n\' '
            '> test/regression_tests\n\tchmod +x test/regression_tests\n')
        self.ref = self.commit()
        self.modules = patch.object(build, 'SUBMODULES', ())
        self.modules.start()
        self.addCleanup(self.modules.stop)

    def commit(self):
        command(['git', 'add', '.'], self.source)
        command(['git', 'commit', '-qm', 'Synthetic build fixture'], self.source)
        return command(['git', 'rev-parse', 'HEAD'], self.source)

    def clean_build(self):
        return build.clean_source_build(self.source, self.ref, self.output)

    def test_real_compiler_rebuilds_new_commit_instead_of_reusing_old_binaries(self):
        command(['make'], self.source)
        old_library = build.sha256(self.source / 'src/.libs/libmodsecurity.so')
        self.assertEqual(command(['./test/regression_tests'], self.source), '1')
        (self.source / 'headers/version.h').write_text('#define VERSION 2\n')
        self.ref = self.commit()  # Deliberately do not rebuild the input checkout.
        target, metadata, provenance = self.clean_build()
        self.assertEqual(command(['./test/regression_tests'], target), '2')
        library = ctypes.CDLL(str(target / 'src/.libs/libmodsecurity.so'))
        self.assertEqual(library.version(), 2)
        self.assertEqual(command(['./test/regression_tests'], self.source), '1')
        self.assertEqual(build.sha256(self.source / 'src/.libs/libmodsecurity.so'), old_library)
        self.assertNotEqual(provenance['artifacts']['library']['sha256'], old_library)
        self.assertEqual(metadata['source_head'], self.ref)
        self.assertEqual(provenance['source_commit'], self.ref)
        self.assertEqual(set(provenance['artifacts']), set(build.ENGINE_ARTIFACTS))

    def test_existing_engine_directory_is_not_reused_or_deleted(self):
        target = self.output / 'engine'
        target.mkdir()
        marker = target / 'keep'
        marker.write_text('keep')
        with self.assertRaises(build.Blocked):
            self.clean_build()
        self.assertEqual(marker.read_text(), 'keep')

    def test_dangling_engine_symlink_is_not_followed(self):
        (self.output / 'engine').symlink_to(self.root / 'absent')
        with self.assertRaises(build.Blocked):
            self.clean_build()
        self.assertFalse((self.root / 'absent').exists())

    def test_failed_build_cannot_produce_provenance(self):
        (self.source / 'build.sh').write_text('#!/bin/sh\nexit 12\n')
        self.ref = self.commit()
        with self.assertRaisesRegex(RuntimeError, 'Engine build step failed'):
            self.clean_build()
        self.assertTrue((self.output / 'engine-logs/bootstrap.log').exists())

    def test_missing_native_runner_prevents_verified_build(self):
        with (self.source / 'Makefile').open('a') as stream:
            stream.write('\trm test/.libs/regression_tests\n')
        self.ref = self.commit()
        with self.assertRaises(FileNotFoundError):
            self.clean_build()

    def test_untracked_production_source_is_rejected_before_cloning(self):
        (self.source / 'src/shadow.h').touch()
        with self.assertRaisesRegex(build.Blocked, 'Untracked production'):
            self.clean_build()
        self.assertFalse((self.output / 'engine').exists())

    def test_ignored_header_is_not_copied_from_input_checkout(self):
        (self.source / 'src/ignored.h').write_text('not in the selected commit')
        target, _, _ = self.clean_build()
        self.assertFalse((target / 'src/ignored.h').exists())
        self.assertTrue((self.source / 'src/ignored.h').exists())

    def test_shallow_detached_checkout_is_supported(self):
        shallow = self.root / 'shallow'
        command(['git', 'clone', '--no-local', '--depth=1', str(self.source), str(shallow)], self.root)
        command(['git', 'checkout', '--detach', self.ref], shallow)
        target, metadata, _ = build.clean_source_build(shallow, self.ref, self.output)
        self.assertEqual(metadata['source_head'], self.ref)
        self.assertEqual(command(['./test/regression_tests'], target), '1')

    def test_untracked_path_bytes_are_not_stripped(self):
        name = '\n leading header.h'
        (self.source / name).touch()
        self.assertIn(name, build.untracked_files(self.source))

    def test_invalid_parallelism_is_rejected(self):
        for jobs in (0, -1, True, '2'):
            with self.subTest(jobs=jobs), self.assertRaises(ValueError):
                build.clean_source_build(self.source, self.ref, self.output, jobs)


class EngineArtifactTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.source = Path(self.temporary.name).resolve() / 'engine'
        for name, relative in build.ENGINE_ARTIFACTS.items():
            path = self.source / relative
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(name.encode())
        artifacts = build.engine_artifacts(self.source)
        self.manifest = {
            'original_commit': SHA, 'source_head': SHA,
            'library': {**artifacts['library'], 'directory': str(self.source / 'src/.libs')},
            'engine_build': {'schema': 1, 'method': 'fresh-git-clone',
                             'source_commit': SHA, 'source_root': str(self.source),
                             'artifacts': artifacts},
        }

    def verify(self):
        return build.verify_engine_build(self.manifest, self.source)

    def test_unchanged_artifacts_pass(self):
        self.assertEqual(self.verify(), self.manifest['engine_build'])

    def test_library_native_executable_and_launcher_are_all_hash_checked(self):
        for relative in build.ENGINE_ARTIFACTS.values():
            with self.subTest(relative=relative):
                path = self.source / relative
                original = path.read_bytes()
                path.write_bytes(b'replaced artifact')
                with self.assertRaisesRegex(build.Blocked, 'changed after'):
                    self.verify()
                path.write_bytes(original)

    def test_absent_or_non_object_provenance_is_rejected(self):
        for value in (None, [], '', 42):
            with self.subTest(value=value):
                self.manifest['engine_build'] = value
                with self.assertRaises(build.Blocked):
                    self.verify()

    def test_source_ref_or_head_mismatch_is_rejected(self):
        for field in ('source_head', 'original_commit'):
            with self.subTest(field=field):
                self.manifest[field] = 'b' * 40
                with self.assertRaises(build.Blocked):
                    self.verify()
                self.manifest[field] = SHA

    def test_recorded_library_must_match_clean_build(self):
        self.manifest['library']['sha256'] = 'b' * 64
        with self.assertRaisesRegex(build.Blocked, 'Library metadata'):
            self.verify()

    def test_artifact_symlink_outside_build_is_rejected(self):
        path = self.source / build.ENGINE_ARTIFACTS['native_runner']
        outside = self.source.parent / 'unrelated'
        outside.write_bytes(path.read_bytes())
        path.unlink()
        path.symlink_to(outside)
        with self.assertRaisesRegex(build.Blocked, 'inside the fresh build'):
            self.verify()

    def test_artifact_objects_cannot_be_arrays(self):
        self.manifest['engine_build']['artifacts'] = []
        with self.assertRaises(build.Blocked):
            self.verify()

    def test_library_metadata_must_be_object(self):
        self.manifest['library'] = []
        with self.assertRaises(build.Blocked):
            self.verify()


class ManifestShapeTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        self.probes = self.root / 'probes'
        self.probes.mkdir()
        self.path = self.probes / 'build-manifest.json'

    def write(self, value):
        self.path.write_text(json.dumps(value))

    def run_all(self, output):
        argv = ['run_all.py', '--probes', str(self.probes), '--output', str(output)]
        with patch.object(sys, 'argv', argv), contextlib.redirect_stdout(io.StringIO()):
            return run_all.main()

    def assert_blocked_summary(self, value, index):
        self.write(value)
        output = self.root / ('result-' + str(index))
        with patch.object(run_all.subprocess, 'run') as process:
            self.assertEqual(self.run_all(output), 1)
            process.assert_not_called()
        result = json.loads((output / 'summary.json').read_text())
        self.assertEqual(result['counts'], dict(PASS=0, FAIL=0, SKIP=0, BLOCKED=7, XFAIL=0))
        self.assertIsNone(result['original_commit'])
        self.assertTrue(all(s['status'] == 'BLOCKED' for s in result['suites']))

    def test_non_object_json_creates_blocked_summary_for_every_suite(self):
        for index, value in enumerate((None, [], [1], 'text', 5, False)):
            with self.subTest(value=value):
                self.assert_blocked_summary(value, index)

    def test_both_manifest_entry_points_reject_non_objects(self):
        for value in (None, [], 'text', 5, False):
            self.write(value)
            for reader in (run_all.read_manifest, run_all.verify_manifest):
                with self.subTest(value=value, reader=reader.__name__):
                    with self.assertRaises(build.Blocked):
                        reader(self.probes)

    def test_invalid_source_path_shapes_are_blocked(self):
        for index, value in enumerate(([], {}, 5, True, '', 'relative/path', '/bad\0path')):
            with self.subTest(value=value):
                self.assert_blocked_summary({'status': 'PASS', 'source_root': value}, index)

    def test_missing_source_root_is_blocked(self):
        self.assert_blocked_summary({'schema': 2, 'status': 'PASS'}, 0)

    def test_probe_map_and_entries_must_be_objects(self):
        for index, value in enumerate((None, [], 5, {'probe': []}, {'probe': {}})):
            self.assert_blocked_summary({'schema': 2, 'status': 'PASS',
                                         'source_root': '/synthetic', 'probes': value}, index)

    def test_legacy_manifest_requires_fresh_build(self):
        self.assert_blocked_summary({'schema': 1, 'status': 'PASS',
                                     'source_root': '/synthetic'}, 0)

    def test_invalid_json_still_creates_summary(self):
        self.path.write_text('{invalid')
        output = self.root / 'result'
        self.assertEqual(self.run_all(output), 1)
        result = json.loads((output / 'summary.json').read_text())
        self.assertEqual(result['counts']['BLOCKED'], 7)

    def test_missing_manifest_still_creates_summary(self):
        output = self.root / 'result'
        self.assertEqual(self.run_all(output), 1)
        self.assertTrue((output / 'summary.json').exists())

    def test_output_cannot_be_created_inside_probe_or_build_tree(self):
        with self.assertRaises(SystemExit), contextlib.redirect_stderr(io.StringIO()):
            self.run_all(self.probes / 'engine/new-output')
        self.assertFalse((self.probes / 'engine').exists())

    def test_output_cannot_be_created_inside_recorded_source(self):
        source = self.root / 'source'
        source.mkdir()
        self.write({'source_root': str(source)})
        with self.assertRaises(SystemExit), contextlib.redirect_stderr(io.StringIO()):
            self.run_all(source / 'new-output')
        self.assertFalse((source / 'new-output').exists())


class CaseReportShapeTests(unittest.TestCase):
    def test_non_object_case_reports_are_rejected(self):
        for report in (None, [], '', 5):
            with self.subTest(report=report), self.assertRaises(ValueError):
                run_all.validated_counts(report)

    def test_non_array_case_records_are_rejected(self):
        for field in ('cases', 'sensitivity'):
            for value in (None, {}, 2):
                report = {'counts': run_all.zeros(), 'cases': [{'status': 'PASS'}], field: value}
                with self.subTest(field=field, value=value), self.assertRaises(ValueError):
                    run_all.validated_counts(report)

    def test_invalid_case_entries_or_status_are_rejected(self):
        for value in (None, [], '', {}, {'status': 'UNKNOWN'}):
            report = {'counts': run_all.zeros(), 'cases': [value]}
            with self.subTest(value=value), self.assertRaises(ValueError):
                run_all.validated_counts(report)

    def test_real_failure_remains_failure(self):
        counts = run_all.zeros()
        counts['FAIL'] = 1
        self.assertEqual(run_all.validated_counts({'counts': counts,
                          'cases': [{'status': 'FAIL'}]}), counts)


if __name__ == '__main__':
    unittest.main()
