import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'TestCase', b'TestCase', 0, 'Test case 2'),
    (b'Test\x00Case', b'Test\x00Case', 0, 'Test case 3'),
    (b'&#x0;&#X0;&#x20;&#X20;&#0;&#32;\x00&#100;&quot;&amp;&lt;&gt;&nbsp;', b'\x00\x00  \x00 \x00d"&<>\xa0', 1, 'Test case 4'),
    (b'&#x0&#X0&#x20&#X20&#0&#32\x00&#100&quot&amp&lt&gt&nbsp', b'\x00\x00  \x00 \x00d"&<>\xa0', 1, 'Test case 5'),
    (b'&#xg;&#Xg;&#xg0;&#X2g;&#a;\x00&#a2;&#3a&#a00;&#1a0;&#10a;&foo;', b'&#xg;&#Xg;&#xg0;\x02g;&#a;\x00&#a2;\x03a&#a00;\x01a0;\na;&foo;', 1, 'Test case 6'),
    (b'&#xg&#Xg&#xg0&#X2g&#a\x00&#a2&#3a&#a00&#1a0&#10a&foo', b'&#xg&#Xg&#xg0\x02g&#a\x00&#a2\x03a&#a00\x01a0\na&foo', 1, 'Test case 7'),
])
def test_htmlentitydecode_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the htmlEntityDecode transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="htmlEntityDecode",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
