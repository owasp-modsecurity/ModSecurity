#!/usr/bin/env python3
"""Compile the byte helpers and cleanup guard without building the engine."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

BASELINE = Path(__file__).resolve().parent.parent
ROOT = BASELINE.parents[1]

SOURCE = r'''
#include <cassert>
#include <cstring>
#include <string>
#include <type_traits>
#include "probe_support.h"
#include "intervention_guard.h"

int main() {
    std::string bytes;
    for (int value = 0; value < 256; ++value) {
        bytes.push_back(static_cast<char>(value));
    }
    assert(json_test::unhex(json_test::hex(bytes)) == bytes);
    assert(json_test::unhex("00aAfF") == std::string("\0\xaa\xff", 3));
    assert(json_test::unhex("").empty());
    assert(json_test::hex("").empty());
    for (const std::string text : {"0", "xyz", "zz", "+1", " 1", "1 ", "g0"}) {
        bool rejected = false;
        try {
            json_test::unhex(text);
        } catch (const json_test::ProbeError &) {
            rejected = true;
        }
        assert(rejected);
    }
    static_assert(!std::is_copy_constructible_v<json_test::InterventionGuard>);
    static_assert(!std::is_copy_assignable_v<json_test::InterventionGuard>);
    for (int attempt = 0; attempt < 10; ++attempt) {
        try {
            json_test::InterventionGuard guard;
            assert(guard.value.status == 200);
            assert(guard.value.url == nullptr && guard.value.log == nullptr);
            guard.value.url = ::strdup("synthetic URL");
            guard.value.log = ::strdup("synthetic log");
            assert(guard.value.url != nullptr && guard.value.log != nullptr);
            throw json_test::ProbeError("exercise exceptional cleanup");
        } catch (const json_test::ProbeError &) {
            // The guard's destructor must free both C-owned fields on unwinding.
        }
    }
}
'''


@unittest.skipUnless(shutil.which('c++'), 'C++17 compiler is required')
class ProbeSupportTests(unittest.TestCase):
    def test_byte_roundtrip_and_exception_cleanup_compile_and_execute(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / 'support.cc'
            executable = Path(directory) / 'support'
            source.write_text(SOURCE)
            compiled = subprocess.run(
                ['c++', '-std=c++17', '-Wall', '-Wextra', '-Werror',
                 '-I' + str(BASELINE), '-isystem', str(ROOT / 'headers'),
                 str(source), '-o', str(executable)],
                capture_output=True, text=True, timeout=60, check=False)
            self.assertEqual(compiled.returncode, 0, compiled.stderr)
            result = subprocess.run([str(executable)], capture_output=True,
                                    text=True, timeout=10, check=False)
            self.assertEqual(result.returncode, 0, result.stderr)

    def test_no_yajl_path_still_compiles_and_reports_blocked(self):
        with tempfile.TemporaryDirectory() as directory:
            executable = Path(directory) / 'probe'
            compiled = subprocess.run(
                ['c++', '-std=c++17', '-Wall', '-Wextra', '-Werror', '-UWITH_YAJL',
                 str(BASELINE / 'probe.cc'), '-o', str(executable)],
                capture_output=True, text=True, timeout=60, check=False)
            self.assertEqual(compiled.returncode, 0, compiled.stderr)
            result = subprocess.run([str(executable)], input='', capture_output=True,
                                    text=True, timeout=10, check=False)
            self.assertEqual(result.returncode, 77)
            self.assertEqual(result.stdout, '')
            self.assertEqual(result.stderr, 'BLOCKED: JSON support was not enabled\n')


if __name__ == '__main__':
    unittest.main()
