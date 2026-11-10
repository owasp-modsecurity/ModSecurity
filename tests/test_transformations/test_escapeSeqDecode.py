import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'TestCase', b'TestCase', 0, 'Test case 2'),
    (b'Test\x00Case', b'Test\x00Case', 0, 'Test case 3'),
    (b'\\a\\b\\f\\n\\r\\t\\v\\?\\\'\\"\\0\\12\\123\\x00\\xff', b'\x07\x08\x0c\n\r\t\x0b?\'"\x00\nS\x00\xff', 1, 'Test case 4'),
    (b'\\a\\b\\f\\n\\r\\t\\v\x00\\?\\\'\\"\\0\\12\\123\\x00\\xff', b'\x07\x08\x0c\n\r\t\x0b\x00?\'"\x00\nS\x00\xff', 1, 'Test case 5'),
    (b'\\8\\9\\666\\xag\\xga\\0123', b'89\xb6xagxga\n3', 1, 'Test case 6'),
    (b'\\x', b'x', 1, 'Test case 7'),
    (b'\\x\\x0', b'xx0', 1, 'Test case 8'),
    (b'\\x\\x0\x00', b'xx0\x00', 1, 'Test case 9'),
    (b'\\0', b'\x00', 1, 'Test case 10'),
    (b'\\01', b'\x01', 1, 'Test case 11'),
    (b'\\012', b'\n', 1, 'Test case 12'),
    (b'\\', b'\\', 0, 'Test case 13'),
    (b'\\\x00', b'\x00', 1, 'Test case 14'),
])
def test_escapeseqdecode_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the escapeSeqDecode transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="escapeSeqDecode",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
