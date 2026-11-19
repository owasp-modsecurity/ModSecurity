#!/usr/bin/env python3
"""Reproduce post-build submodule additions with real Git and Libtool."""
import contextlib
import io
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest
from unittest.mock import patch

import build
import run_all


@unittest.skipUnless(all(shutil.which(tool) for tool in ('git', 'libtoolize', 'make', 'c++')),
                     'Git, Libtool, Make and C++ are required')
class GeneratedBuildFilesTests(unittest.TestCase):
    def command(self, arguments, directory):
        result = subprocess.run(arguments, cwd=directory, text=True, capture_output=True,
                                timeout=30, check=False)
        self.assertEqual(result.returncode, 0, result.stderr)
        return result.stdout.strip()

    def commit(self, directory):
        self.command(['git', 'add', '.'], directory)
        self.command(['git', 'commit', '-qm', 'Synthetic fixture'], directory)
        return self.command(['git', 'rev-parse', 'HEAD'], directory)

    def repository(self, name):
        directory = self.root / name
        directory.mkdir()
        self.command(['git', 'init', '-q'], directory)
        self.command(['git', 'config', 'user.name', 'Test fixture'], directory)
        self.command(['git', 'config', 'user.email', 'fixture@example.invalid'], directory)
        return directory

    def libtool_repository(self, name):
        directory = self.repository(name)
        (directory / 'build-aux/m4').mkdir(parents=True)
        (directory / 'build-aux/m4/.keep').touch()
        (directory / '.gitignore').write_text('build-aux/ltmain.sh\n')
        (directory / 'configure.ac').write_text(
            'AC_INIT([fixture], [1.0])\nAC_CONFIG_AUX_DIR([build-aux])\n'
            'AC_CONFIG_MACRO_DIR([build-aux/m4])\nLT_INIT\nAC_OUTPUT\n')
        (directory / 'source.c').write_text('int fixture(void) { return 0; }\n')
        self.commit(directory)
        return directory

    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name).resolve()
        # The file transport is allowed only in these local synthetic fixtures.
        environment = patch.dict(os.environ, {'GIT_ALLOW_PROTOCOL': 'file'})
        environment.start()
        self.addCleanup(environment.stop)
        nested = self.libtool_repository('nested')
        module = self.libtool_repository('module')
        self.command(['git', 'submodule', 'add', '-q', str(nested), 'framework'], module)
        self.commit(module)
        self.source = self.repository('source')
        for directory in ('src', 'headers', 'test/regression'):
            (self.source / directory).mkdir(parents=True)
        (self.source / 'headers/test.h').touch()
        (self.source / 'configure.ac').touch()
        (self.source / 'src/library.cc').write_text('extern "C" int version() { return 2; }\n')
        (self.source / 'test/regression/regression.cc').write_text('int main() { return 0; }\n')
        (self.source / '.gitignore').write_text('.libs/\ntest/regression_tests\n')
        self.command(['git', 'submodule', 'add', '-q', str(module), 'others/libinjection'], self.source)
        self.command(['git', 'submodule', 'update', '--init', '--recursive'], self.source)
        (self.source / 'build.sh').write_text(
            '#!/bin/sh\nset -eu\n'
            'for module in others/libinjection others/libinjection/framework; do\n'
            '  (cd "$module" && libtoolize --force --copy)\ndone\n')
        configure = self.source / 'configure'
        configure.write_text('#!/bin/sh\nexit 0\n')
        configure.chmod(0o755)
        (self.source / 'Makefile').write_text(
            'all:\n\tmkdir -p src/.libs test/.libs\n'
            '\tc++ -shared -fPIC src/library.cc -o src/.libs/libmodsecurity.so\n'
            '\tc++ test/regression/regression.cc -o test/.libs/regression_tests\n'
            '\tprintf \'#!/bin/sh\\nexit 0\\n\' > test/regression_tests\n'
            '\tchmod +x test/regression_tests\n')
        self.reference = self.commit(self.source)
        modules = patch.object(build, 'SUBMODULES', ('others/libinjection',))
        modules.start()
        self.addCleanup(modules.stop)
        self.output = self.root / 'output'
        self.output.mkdir()

    def execute_build(self):
        with contextlib.redirect_stdout(io.StringIO()):
            self.target, self.metadata, self.provenance = build.clean_source_build(
                self.source, self.reference, self.output)
        self.generated = self.provenance['generated_files']
        self.assertIn('others/libinjection/build-aux/m4/libtool.m4', self.generated)
        self.assertIn('others/libinjection/framework/build-aux/m4/libtool.m4', self.generated)

    def verify_source(self):
        return build.source_metadata(self.target, self.reference,
                                     generated_files=self.generated)

    def test_real_libtool_outputs_are_recorded_and_not_source_changes(self):
        self.execute_build()
        self.assertEqual(self.verify_source(), self.metadata)
        self.assertEqual(json.loads((self.output / 'engine-logs/generated-files.json').read_text()),
                         self.generated)
        self.assertFalse((self.source / 'others/libinjection/build-aux/m4/libtool.m4').exists())
        # Without the build record this reproduces the CI's strict-check failure.
        with self.assertRaisesRegex(build.Blocked, 'Submodule has untracked'):
            build.source_metadata(self.target, self.reference)

    def test_profile_preflight_uses_the_generated_file_evidence(self):
        self.execute_build()
        manifest = {**self.metadata, 'schema': 2, 'status': 'PASS', 'probes': {},
                    'engine_build': self.provenance,
                    'library': {**self.provenance['artifacts']['library'],
                                'directory': str(self.target / 'src/.libs')}}
        (self.output / 'build-manifest.json').write_text(json.dumps(manifest))
        with patch.object(run_all, 'PROBES', {}), \
             patch.object(run_all, 'active_canary', return_value=True):
            _, metadata, _, _ = run_all.verify_manifest(self.output)
        self.assertEqual(metadata, self.metadata)

    def test_input_submodule_additions_remain_blocked(self):
        (self.source / 'others/libinjection/unexpected.h').write_text('not committed')
        with self.assertRaisesRegex(build.Blocked, 'unexpected.h'):
            build.clean_source_build(self.source, self.reference, self.output)
        self.assertFalse((self.output / 'engine').exists())

    def test_generated_file_modification_is_blocked(self):
        self.execute_build()
        path = self.target / next(iter(self.generated))
        path.write_text('changed after build')
        with self.assertRaisesRegex(build.Blocked, 'Generated build file changed'):
            self.verify_source()

    def test_deleted_generated_file_is_blocked(self):
        self.execute_build()
        (self.target / next(iter(self.generated))).unlink()
        with self.assertRaisesRegex(build.Blocked, 'Generated build file is missing'):
            self.verify_source()

    def test_unrecorded_file_after_build_is_blocked(self):
        self.execute_build()
        (self.target / 'others/libinjection/unexpected.h').write_text('not a build output')
        with self.assertRaisesRegex(build.Blocked, 'unexpected.h'):
            self.verify_source()

    def test_nested_unrecorded_file_is_blocked(self):
        self.execute_build()
        (self.target / 'others/libinjection/framework/unexpected.h').write_text('not recorded')
        with self.assertRaisesRegex(build.Blocked, 'unexpected.h'):
            self.verify_source()

    def test_tracked_submodule_source_change_is_still_blocked(self):
        self.execute_build()
        (self.target / 'others/libinjection/source.c').write_text('changed source')
        with self.assertRaises(build.Blocked):
            self.verify_source()

    def test_invalid_record_shapes_and_hashes_are_blocked(self):
        for value in ([], None, {'source.c': 5}, {'source.c': 'invalid'}):
            with self.subTest(value=value), self.assertRaises(build.Blocked):
                build.verify_generated_files(self.source, value)

    def test_path_traversal_and_git_metadata_are_blocked(self):
        for name in ('../outside', '/absolute', './configure', '.git/config', 'a/../configure'):
            with self.subTest(name=name), self.assertRaises(build.Blocked):
                build.generated_file_path(self.source, name)

    def test_symlink_to_an_in_tree_file_is_not_allowed(self):
        (self.source / 'alias').symlink_to(self.source / 'configure')
        with self.assertRaisesRegex(build.Blocked, 'regular in-tree'):
            build.generated_file_path(self.source, 'alias')

    def test_generated_snapshot_cannot_follow_external_symlinks(self):
        outside = self.root / 'outside'
        outside.write_text('outside')
        (self.source / 'alias').symlink_to(outside)
        with self.assertRaisesRegex(build.Blocked, 'regular in-tree'):
            build.generated_file_snapshot(self.source, {})


if __name__ == '__main__':
    unittest.main()
