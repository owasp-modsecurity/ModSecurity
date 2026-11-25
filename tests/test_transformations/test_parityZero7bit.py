import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ', b'abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ', 0, 'Test case 2'),
    (b'abcdefghijklmnopqrstuvwxyz\x000123456789\x00ABCDEFGHIJKLMNOPQRSTUVWXYZ', b'abcdefghijklmnopqrstuvwxyz\x000123456789\x00ABCDEFGHIJKLMNOPQRSTUVWXYZ', 0, 'Test case 3'),
    (b'\x80\x00\x8f\xff', b'\x00\x00\x0f\x7f', 1, 'Test case 4'),
])
def test_parityzero7bit_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the parityZero7bit transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="parityZero7bit",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
