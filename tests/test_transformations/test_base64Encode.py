import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'TestCase', b'VGVzdENhc2U=', 1, 'Test case 2'),
    (b'TestCase1', b'VGVzdENhc2Ux', 1, 'Test case 3'),
    (b'TestCase12', b'VGVzdENhc2UxMg==', 1, 'Test case 4'),
    (b'Test\x00Case', b'VGVzdABDYXNl', 1, 'Test case 5'),
])
def test_base64encode_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the base64Encode transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="base64Encode",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
