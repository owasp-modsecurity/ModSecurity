import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'TestCase', b'TestCase', 0, 'Test case 2'),
    (b'Test\x00Case', b'Test\x00Case', 0, 'Test case 3'),
    (b'Test Case', b'Test Case', 0, 'Test case 4'),
    (b'  Test  \t   Case  ', b' Test Case ', 1, 'Test case 5'),
    (b'This is a   test case with a tab \t, vtab \x0b, newline \n, return \r, formfeed \x0c, and a NUL\x00 in it with a CRLF at the end.\r\n', b'This is a test case with a tab , vtab , newline , return , formfeed , and a NUL\x00 in it with a CRLF at the end. ', 1, 'Test case 6'),
])
def test_compresswhitespace_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the compressWhitespace transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="compressWhitespace",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
