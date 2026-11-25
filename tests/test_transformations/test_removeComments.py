import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'TestCase', b'TestCase', 0, 'Test case 2'),
    (b'Test\\u0000Case', b'Test\\u0000Case', 0, 'Test case 3'),
    (b'/* TestCase */', b'', 1, 'Test case 4'),
    (b'/*TestCase*/', b'', 1, 'Test case 5'),
    (b'/* TestCase*/', b'', 1, 'Test case 6'),
    (b'/*TestCase */', b'', 1, 'Test case 7'),
    (b'Before/* TestCase */After', b'BeforeAfter', 1, 'Test case 8'),
    (b'Before TestCase */ After', b'Before TestCase */ After', 0, 'Test case 9'),
    (b'/* Test\\nCase */', b'', 1, 'Test case 10'),
    (b'/* Test\r\nCase */', b'', 1, 'Test case 11'),
    (b'/* Test\nCase */', b'', 1, 'Test case 12'),
    (b'/* Test\rCase */', b'', 1, 'Test case 13'),
    (b'/*Before/* Test\r\nCase ', b' ', 1, 'Test case 14'),
    (b'Before /* Test\nCase ', b'Before  ', 1, 'Test case 15'),
    (b'Before/* T*/ /* e */ /* s */ /* t */\r\nCase ', b'Before   \r\nCase ', 1, 'Test case 16'),
    (b'Before /* */ ops */ Test\nCase ', b'Before  ops */ Test\nCase ', 1, 'Test case 17'),
    (b'/*Test\r\nCase */After', b'After', 1, 'Test case 18'),
    (b'Test\nCase /**/ After', b'Test\nCase  After', 1, 'Test case 19'),
    (b'Test\r\nCase */After', b'Test\r\nCase */After', 0, 'Test case 20'),
    (b'Test/*\nCase */ After', b'Test After', 1, 'Test case 21'),
    (b'SELECT/**//**/UNION', b'SELECTUNION', 1, 'Test case 22'),
    (b'foo<!-- comment 1 --><!-- comment 2 -->bar', b'foobar', 1, 'Test case 23'),
])
def test_removecomments_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the removeComments transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="removeComments",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
