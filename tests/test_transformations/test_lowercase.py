import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'testcase', b'testcase', 0, 'Test case 2'),
    (b'test\x00case', b'test\x00case', 0, 'Test case 3'),
    (b'TestCase', b'testcase', 1, 'Test case 4'),
    (b'Test\x00Case', b'test\x00case', 1, 'Test case 5'),
])
def test_lowercase_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the lowercase transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="lowercase",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
