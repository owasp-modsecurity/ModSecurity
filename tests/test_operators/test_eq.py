import pytest


@pytest.mark.unit
@pytest.mark.parametrize("param,input_data,expected_ret,test_name", [
    (b'0', b'', 1, 'Test case 1'),
    (b'5', b'', 0, 'Test case 2'),
    (b'xxx', b'0', 1, 'Test case 3'),
    (b'xxx', b'5', 0, 'Test case 4'),
    (b'xxx', b'-1', 0, 'Test case 5'),
    (b'0', b'xxx', 1, 'Test case 6'),
    (b'5', b'xxx', 0, 'Test case 7'),
    (b'0', b'-5', 0, 'Test case 8'),
    (b'0', b'0', 1, 'Test case 9'),
    (b'0', b'5', 0, 'Test case 10'),
    (b'5', b'0', 0, 'Test case 11'),
    (b'5', b'5', 1, 'Test case 12'),
    (b'5', b'10', 0, 'Test case 13'),
])
def test_eq_operator(unit_test, param, input_data, expected_ret, test_name):
    """Test the @eq operator with various inputs"""

    result = unit_test.unit_runner.run_operator_test(
        name="eq",
        param=param,
        input_data=input_data,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
