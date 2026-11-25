import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'abdghkmnpsuvyz12478CEFIJLOQRTW', b'abdghkmnpsuvyz12478CEFIJLOQRTW', 0, 'Test case 2'),
    (b'abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ', b'ab\xe3d\xe5\xe6gh\xe9\xeak\xecmn\xefp\xf1\xf2s\xf4uv\xf7\xf8yz\xb012\xb34\xb5\xb678\xb9\xc1\xc2C\xc4EF\xc7\xc8IJ\xcbL\xcd\xceO\xd0QR\xd3T\xd5\xd6WX\xd9\xda', 1, 'Test case 3'),
    (b'abcdefghijklmnopqrstuvwxyz\x000123456789\x00ABCDEFGHIJKLMNOPQRSTUVWXYZ', b'ab\xe3d\xe5\xe6gh\xe9\xeak\xecmn\xefp\xf1\xf2s\xf4uv\xf7\xf8yz\x80\xb012\xb34\xb5\xb678\xb9\x80\xc1\xc2C\xc4EF\xc7\xc8IJ\xcbL\xcd\xceO\xd0QR\xd3T\xd5\xd6WX\xd9\xda', 1, 'Test case 4'),
])
def test_parityodd7bit_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the parityOdd7bit transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="parityOdd7bit",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
