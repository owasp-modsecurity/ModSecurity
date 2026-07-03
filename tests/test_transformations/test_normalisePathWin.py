import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'\\foo\\bar\\baz', b'/foo/bar/baz', 1, 'Test case 2'),
    (b'\\foo\\bar\x00\\baz', b'/foo/bar\x00/baz', 1, 'Test case 3'),
    (b'x', b'x', 0, 'Test case 4'),
    (b'.', b'', 1, 'Test case 5'),
    (b'.\\', b'', 1, 'Test case 6'),
    (b'.\\..', b'..', 1, 'Test case 7'),
    (b'.\\..\\', b'../', 1, 'Test case 8'),
    (b'..', b'..', 0, 'Test case 9'),
    (b'..\\', b'../', 1, 'Test case 10'),
    (b'..\\.', b'..', 1, 'Test case 11'),
    (b'..\\.\\', b'../', 1, 'Test case 12'),
    (b'..\\..', b'../..', 1, 'Test case 13'),
    (b'..\\..\\', b'../../', 1, 'Test case 14'),
    (b'\\dir\\foo\\\\bar', b'/dir/foo/bar', 1, 'Test case 15'),
    (b'dir\\foo\\\\bar\\', b'dir/foo/bar/', 1, 'Test case 16'),
    (b'dir\\..\\foo', b'foo', 1, 'Test case 17'),
    (b'dir\\..\\..\\foo', b'../foo', 1, 'Test case 18'),
    (b'dir\\.\\..\\.\\..\\..\\foo\\bar', b'../../foo/bar', 1, 'Test case 19'),
    (b'dir\\.\\..\\.\\..\\..\\foo\\bar\\.', b'../../foo/bar', 1, 'Test case 20'),
    (b'dir\\.\\..\\.\\..\\..\\foo\\bar\\.\\', b'../../foo/bar/', 1, 'Test case 21'),
    (b'dir\\.\\..\\.\\..\\..\\foo\\bar\\..', b'../../foo', 1, 'Test case 22'),
    (b'dir\\.\\..\\.\\..\\..\\foo\\bar\\..\\', b'../../foo/', 1, 'Test case 23'),
    (b'dir\\.\\..\\.\\..\\..\\foo\\bar\\', b'../../foo/bar/', 1, 'Test case 24'),
    (b'dir\\\\.\\\\..\\\\.\\\\..\\\\..\\\\foo\\\\bar', b'../../foo/bar', 1, 'Test case 25'),
    (b'dir\\\\.\\\\..\\\\.\\\\..\\\\..\\\\foo\\\\bar\\\\', b'../../foo/bar/', 1, 'Test case 26'),
    (b'dir\\subdir\\subsubdir\\subsubsubdir\\..\\..\\..', b'dir', 1, 'Test case 27'),
    (b'dir\\.\\subdir\\.\\subsubdir\\.\\subsubsubdir\\..\\..\\..', b'dir', 1, 'Test case 28'),
    (b'dir\\.\\subdir\\..\\subsubdir\\..\\subsubsubdir\\..', b'dir', 1, 'Test case 29'),
    (b'\\dir\\.\\subdir\\..\\subsubdir\\..\\subsubsubdir\\..\\', b'/dir/', 1, 'Test case 30'),
    (b'\\.\\..\\.\\..\\..\\..\\..\\..\\..\\..\\\x00\\..\\etc\\.\\passwd', b'/etc/passwd', 1, 'Test case 31'),
])
def test_normalisepathwin_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the normalisePathWin transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="normalisePathWin",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
