import pytest


@pytest.mark.unit
@pytest.mark.parametrize("param,input_data,expected_ret,test_name", [
    (b'op/pmFromFile-01.dat', b'xxxyyyzzz', 0, 'Test case 1'),
    (b'op/pmFromFile-01.dat', b'defxxxyyy', 1, 'Test case 2'),
    (b'op/pmFromFile-01.dat', b'xxxdefyyy', 1, 'Test case 3'),
    (b'op/pmFromFile-01.dat', b'xxxyyydef', 1, 'Test case 4'),
    (b'op/pmFromFile-01.dat', b'xxx yyy zzz', 1, 'Test case 5'),
    (b'op/pmFromFile-01.dat', b'xxx yyy', 0, 'Test case 6'),
])
def test_pmfromfile_operator(unit_test, param, input_data, expected_ret, test_name):
    """Test the @pmFromFile operator with various inputs"""

    result = unit_test.unit_runner.run_operator_test(
        name="pmFromFile",
        param=param,
        input_data=input_data,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
