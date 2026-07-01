import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'', 1, 'Test case 1'),
    (b'TestCase', b'5465737443617365', 1, 'Test case 2'),
    (b'Test\x00Case', b'546573740043617365', 1, 'Test case 3'),
])
def test_hexencode_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the hexEncode transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="hexEncode",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
