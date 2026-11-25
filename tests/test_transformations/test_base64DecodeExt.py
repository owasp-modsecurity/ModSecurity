import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'dzBzV==YWxlcnQoMSk=', b'', 0, 'Test case 1'),
    (b'eyJwIjoieHh-IiwiYWxnIjoibm9uZSIsInR5cCI6IkpXVCJ9', b'{"p":"xx~","alg":"none","typ":"JWT"}', 1, 'Test case 2'),
])
def test_base64decodeext_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the base64DecodeExt transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="base64DecodeExt",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
