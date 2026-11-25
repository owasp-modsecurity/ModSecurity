import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'TestCase', b'TestCase', 0, 'Test case 2'),
    (b'Test\x00Case', b'Test\x00Case', 0, 'Test case 3'),
    (b'    TestCase', b'TestCase', 1, 'Test case 4'),
    (b'TestCase    ', b'TestCase', 1, 'Test case 5'),
    (b'    TestCase    ', b'TestCase', 1, 'Test case 6'),
    (b'    Test   Case    ', b'Test   Case', 1, 'Test case 7'),
    (b'    Test \x00 Case    ', b'Test \x00 Case', 1, 'Test case 8'),
    (b' \t   Test \x00 Case \t  \t   \r\n  ', b'Test \x00 Case', 1, 'Test case 9'),
])
def test_trim_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the trim transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="trim",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
