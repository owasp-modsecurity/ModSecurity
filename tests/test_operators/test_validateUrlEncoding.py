import pytest


@pytest.mark.unit
@pytest.mark.parametrize("param,input_data,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'', b'Hello%20World!', 0, 'Test case 2'),
    (b'', b'Hello+World!', 0, 'Test case 3'),
    (b'', b'HelloWorld!', 0, 'Test case 4'),
    (b'', b'%00Hello%20World!', 0, 'Test case 5'),
    (b'', b'Hello%20World!%00', 0, 'Test case 6'),
    (b'', b'%00', 0, 'Test case 7'),
    (b'', b'%ff', 0, 'Test case 8'),
    (b'', b'%0', 1, 'Test case 9'),
    (b'', b'%f', 1, 'Test case 10'),
    (b'', b'%', 1, 'Test case 11'),
    (b'', b'%0z', 1, 'Test case 12'),
    (b'', b'%z0', 1, 'Test case 13'),
    (b'', b'%0%', 1, 'Test case 14'),
])
def test_validateurlencoding_operator(unit_test, param, input_data, expected_ret, test_name):
    """Test the @validateUrlEncoding operator with various inputs"""

    result = unit_test.unit_runner.run_operator_test(
        name="validateUrlEncoding",
        param=param,
        input_data=input_data,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
