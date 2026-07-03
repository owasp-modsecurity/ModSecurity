import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'cefijloqrtwx03569ABDGHKMNPSUVYZ', b'cefijloqrtwx03569ABDGHKMNPSUVYZ', 0, 'Test case 2'),
    (b'abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ', b'\xe1\xe2c\xe4ef\xe7\xe8ij\xebl\xed\xeeo\xf0qr\xf3t\xf5\xf6wx\xf9\xfa0\xb1\xb23\xb456\xb7\xb89AB\xc3D\xc5\xc6GH\xc9\xcaK\xccMN\xcfP\xd1\xd2S\xd4UV\xd7\xd8YZ', 1, 'Test case 3'),
    (b'abcdefghijklmnopqrstuvwxyz\x000123456789\x00ABCDEFGHIJKLMNOPQRSTUVWXYZ', b'\xe1\xe2c\xe4ef\xe7\xe8ij\xebl\xed\xeeo\xf0qr\xf3t\xf5\xf6wx\xf9\xfa\x000\xb1\xb23\xb456\xb7\xb89\x00AB\xc3D\xc5\xc6GH\xc9\xcaK\xccMN\xcfP\xd1\xd2S\xd4UV\xd7\xd8YZ', 1, 'Test case 4'),
])
def test_parityeven7bit_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the parityEven7bit transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="parityEven7bit",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
