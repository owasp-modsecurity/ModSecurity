#!/usr/bin/env python3
"""
Script to convert Perl .t test files to pytest format.

Usage:
    python convert_perl_tests.py input.t output.py
    python convert_perl_tests.py --all  # Convert all .t files
"""

import argparse
import base64
import json
import re
import os
import subprocess
import sys
from pathlib import Path
from typing import List, Dict, Any


class PerlTestConverter:
    """Converts Perl test format to pytest format"""

    def __init__(self):
        self.test_data = []

    def parse_unit_test_file(self, file_path: Path) -> List[Dict[str, Any]]:
        """Parse an op/*.t or tfn/*.t unit-test data file via dump_unit_fixtures.pl.

        Unlike parse_perl_file(), this lets Perl itself evaluate the file (same
        as run-unit-tests.pl does), so Perl string-escape sequences (\\xHH, \\n,
        "\\\\", etc.) are resolved correctly instead of being re-escaped by a
        naive regex parser. param/input/output come back base64-encoded (they
        are byte strings, not guaranteed to be valid UTF-8) and are decoded to
        `bytes` here.
        """
        script = Path(__file__).parent / "dump_unit_fixtures.pl"
        proc = subprocess.run(
            ["perl", str(script), str(file_path)],
            capture_output=True, text=True, check=True,
        )
        tests = json.loads(proc.stdout)
        for test in tests:
            for key in ("param", "input", "output"):
                if key in test:
                    test[key] = base64.b64decode(test[key])
        return tests

    def parse_perl_file(self, file_path: Path) -> List[Dict[str, Any]]:
        """Parse a Perl .t file and extract test data"""
        with open(file_path, 'r') as f:
            content = f.read()
        
        # Remove comments and clean up
        content = re.sub(r'#.*$', '', content, flags=re.MULTILINE)
        
        # Extract test hashes - this is a simplified parser
        # In practice, you might want to use a proper Perl parser
        pattern = r'\{([^{}]*(?:\{[^{}]*\}[^{}]*)*)\}'
        matches = re.findall(pattern, content, re.DOTALL)
        
        tests = []
        for match in matches:
            test_dict = self._parse_test_hash(match)
            if test_dict:
                tests.append(test_dict)
        
        return tests
    
    def _parse_test_hash(self, hash_content: str) -> Dict[str, Any]:
        """Parse individual test hash content"""
        test_dict = {}
        
        # Simple key-value extraction
        patterns = {
            'type': r'type\s*=>\s*["\']([^"\']+)["\']',
            'name': r'name\s*=>\s*["\']([^"\']+)["\']',
            'param': r'param\s*=>\s*["\']([^"\']*)["\']',
            'input': r'input\s*=>\s*["\']([^"\']*)["\']',
            'output': r'output\s*=>\s*["\']([^"\']*)["\']',
            'ret': r'ret\s*=>\s*(\d+)',
            'comment': r'comment\s*=>\s*["\']([^"\']+)["\']',
        }
        
        for key, pattern in patterns.items():
            match = re.search(pattern, hash_content)
            if match:
                if key == 'ret':
                    test_dict[key] = int(match.group(1))
                else:
                    test_dict[key] = match.group(1)
        
        # Handle special cases like conf (multiline)
        conf_match = re.search(r'conf\s*=>\s*qq\((.*?)\)', hash_content, re.DOTALL)
        if conf_match:
            test_dict['conf'] = conf_match.group(1).strip()
        
        return test_dict
    
    def generate_pytest_file(self, tests: List[Dict[str, Any]], output_path: Path, test_type: str):
        """Generate pytest file from test data"""
        
        # Determine the test category
        if tests and 'type' in tests[0]:
            category = tests[0]['type']
        else:
            category = 'unknown'
        
        # Generate appropriate pytest code
        if category == 'op':
            content = self._generate_operator_tests(tests, output_path.stem)
        elif category == 'tfn':
            content = self._generate_transformation_tests(tests, output_path.stem)
        elif category == 'action':
            content = self._generate_regression_tests(tests, output_path.stem)
        else:
            content = self._generate_generic_tests(tests, output_path.stem)
        
        # Write to file
        with open(output_path, 'w') as f:
            f.write(content)
    
    def _generate_operator_tests(self, tests: List[Dict[str, Any]], test_name: str) -> str:
        """Generate operator tests. `param`/`input` in `tests` are real bytes
        (decoded by parse_unit_test_file), so repr() emits a correct, unambiguous
        Python bytes literal for every test case, including NUL bytes, invalid
        UTF-8 sequences and literal backslashes."""

        param_data = []
        for i, test in enumerate(tests):
            param = test.get('param', b'')
            input_data = test.get('input', b'')
            ret = test.get('ret', 1)
            comment = test.get('comment', f"Test case {i + 1}")

            param_data.append(f'    ({param!r}, {input_data!r}, {ret}, {comment!r}),')

        operator_name = tests[0].get('name', 'unknown') if tests else 'unknown'

        content = f'''import pytest


@pytest.mark.unit
@pytest.mark.parametrize("param,input_data,expected_ret,test_name", [
{chr(10).join(param_data)}
])
def test_{operator_name.lower()}_operator(unit_test, param, input_data, expected_ret, test_name):
    """Test the @{operator_name} operator with various inputs"""

    result = unit_test.unit_runner.run_operator_test(
        name="{operator_name}",
        param=param,
        input_data=input_data,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{{test_name}}' failed: {{result.message}}"
'''

        return content

    def _generate_transformation_tests(self, tests: List[Dict[str, Any]], test_name: str) -> str:
        """Generate transformation tests. See _generate_operator_tests for why
        input/output are emitted as bytes literals."""

        param_data = []
        for i, test in enumerate(tests):
            input_data = test.get('input', b'')
            output = test.get('output', b'')
            ret = test.get('ret', 1)
            comment = test.get('comment', f"Test case {i + 1}")

            param_data.append(f'    ({input_data!r}, {output!r}, {ret}, {comment!r}),')

        tfn_name = tests[0].get('name', 'unknown') if tests else 'unknown'

        content = f'''import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
{chr(10).join(param_data)}
])
def test_{tfn_name.lower()}_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the {tfn_name} transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="{tfn_name}",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{{test_name}}' failed: {{result.message}}"
'''

        return content
    
    def _generate_regression_tests(self, tests: List[Dict[str, Any]], test_name: str) -> str:
        """Generate regression tests"""
        
        test_methods = []
        for i, test in enumerate(tests):
            method_name = f"test_{test_name}_{i+1}"
            comment = test.get('comment', f'Test case {i+1}')
            conf = test.get('conf', '')
            
            method_content = f'''    def {method_name}(self, modsec_test):
        """Test: {comment}"""
        config = """
{self._indent_text(conf, 12)}
        """
        
        test_config = {{
            "type": "action",
            "comment": "{comment}",
            "conf": config,
            "match_log": {{
                # Add log matching patterns here
            }},
            "match_response": {{
                # Add response matching patterns here  
            }},
            "request": {{
                "method": "GET",
                "path": "/test.txt"
            }}
        }}
        
        result = modsec_test.run_regression_test(test_config)
        assert result.success, f"Test '{comment}' failed: {{result.message}}"
'''
            test_methods.append(method_content)
        
        content = f'''import pytest
import re
import requests
from ..modsec_test import TestResult


@pytest.mark.regression
@pytest.mark.apache
class Test{test_name.title().replace('_', '')}:
    """Test class for {test_name} tests"""
    
{chr(10).join(test_methods)}
'''
        
        return content
    
    def _generate_generic_tests(self, tests: List[Dict[str, Any]], test_name: str) -> str:
        """Generate generic tests"""
        content = f'''import pytest
from ..modsec_test import TestResult


@pytest.mark.unit
class Test{test_name.title().replace('_', '')}:
    """Test class for {test_name} tests"""
    
    def test_placeholder(self, modsec_test):
        """Placeholder test - update with actual test logic"""
        # TODO: Implement test logic based on original Perl tests
        # Original test data:
        # {tests}
        pass
'''
        return content
    
    def _escape_string(self, s: str) -> str:
        """Escape string for Python code"""
        return s.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n').replace('\t', '\\t')
    
    def _indent_text(self, text: str, spaces: int) -> str:
        """Indent text by specified number of spaces"""
        indent = ' ' * spaces
        return '\n'.join(indent + line for line in text.split('\n'))


def main():
    """Main conversion function"""
    parser = argparse.ArgumentParser(description='Convert Perl .t files to pytest format')
    parser.add_argument('input_file', nargs='?', help='Input .t file to convert')
    parser.add_argument('output_file', nargs='?', help='Output .py file')
    parser.add_argument('--all', action='store_true', help='Convert all .t files in current directory')
    parser.add_argument('--output-dir', default='converted_tests', help='Output directory for --all mode')
    parser.add_argument('--unit-only', action='store_true',
                         help='Convert op/*.t -> test_operators/ and tfn/*.t -> test_transformations/ '
                              'via Perl-assisted parsing (parse_unit_test_file). Regression-type .t '
                              'files (under regression/) are not touched by this mode.')

    args = parser.parse_args()

    converter = PerlTestConverter()

    if args.unit_only:
        # These have hand-written pytest coverage beyond the mechanical
        # parametrize table (extra edge-case classes) - regenerating them
        # would blindly discard that. Left untouched; update by hand.
        hand_written = {"beginswith", "base64decode"}
        destinations = {
            Path('op'): Path('test_operators'),
            Path('tfn'): Path('test_transformations'),
        }
        for src_dir, dest_dir in destinations.items():
            dest_dir.mkdir(exist_ok=True)
            for t_file in sorted(src_dir.glob('*.t')):
                if t_file.stem.lower() in hand_written:
                    print(f"Skipping {t_file} (hand-written pytest file exists)")
                    continue
                print(f"Converting {t_file}...")
                try:
                    tests = converter.parse_unit_test_file(t_file)
                    if not tests:
                        print(f"  No tests found in {t_file}")
                        continue
                    output_file = dest_dir / f"test_{t_file.stem}.py"
                    converter.generate_pytest_file(tests, output_file, t_file.stem)
                    print(f"  -> {output_file}")
                except subprocess.CalledProcessError as e:
                    print(f"  Error converting {t_file}: {e.stderr}")
        return

    if args.all:
        # Convert all .t files
        output_dir = Path(args.output_dir)
        output_dir.mkdir(exist_ok=True)
        
        t_files = list(Path('.').glob('**/*.t'))
        if not t_files:
            print("No .t files found in current directory")
            return
        
        for t_file in t_files:
            print(f"Converting {t_file}...")
            try:
                tests = converter.parse_perl_file(t_file)
                if tests:
                    output_file = output_dir / f"test_{t_file.stem}.py"
                    converter.generate_pytest_file(tests, output_file, t_file.stem)
                    print(f"  -> {output_file}")
                else:
                    print(f"  No tests found in {t_file}")
            except Exception as e:
                print(f"  Error converting {t_file}: {e}")
    
    elif args.input_file and args.output_file:
        # Convert single file
        input_path = Path(args.input_file)
        output_path = Path(args.output_file)
        
        if not input_path.exists():
            print(f"Input file {input_path} does not exist")
            return
        
        print(f"Converting {input_path} -> {output_path}")
        
        try:
            tests = converter.parse_perl_file(input_path)
            if tests:
                converter.generate_pytest_file(tests, output_path, input_path.stem)
                print("Conversion completed successfully")
            else:
                print("No tests found in input file")
        except Exception as e:
            print(f"Error during conversion: {e}")
    
    else:
        parser.print_help()


if __name__ == '__main__':
    main() 