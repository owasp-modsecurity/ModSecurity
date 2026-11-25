import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'TestCase', b'TestCase', 0, 'Test case 2'),
    (b'Test\x00Case', b'Test\x00Case', 0, 'Test case 3'),
    (b'test\\a\\b\\f\\n\\r\\t\\v\\?\\\'\\"\\0\\12\\123\\1234\\12345\\123456\\ff01\\ff5e\\\n\\0  string', b'test\n\x0b\x0fnrtv?\'"\x00\x12#4EV!~\x00 string', 1, 'Test case 4'),
    (b'test\\', b'test', 1, 'Test case 5'),
    (b'\\1A\\1 A\\1234567\\123456 7\\1x\\1 x', b'\x1a\x01AV7V7\x01x\x01x', 1, 'Test case 6'),
])
def test_cssdecode_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the cssDecode transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="cssDecode",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
