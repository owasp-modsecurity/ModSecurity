import pytest


@pytest.mark.unit
@pytest.mark.parametrize("param,input_data,expected_ret,test_name", [
    # Empty tests
    ("", "", 1, "Empty param and input"),
    ("TestCase", "", 0, "Empty input with param"),
    ("", "TestCase", 1, "Empty param with input"),
    
    # General tests
    ("abcdef", "abcdef", 1, "Exact match"),
    ("abcdef", "abcdefghi", 1, "Input starts with param"),
    ("abcdef", "abc", 0, "Input shorter than param"),
    
    # Case sensitivity tests
    ("ABCDEF", "abcdef", 0, "Case mismatch - uppercase param"),
    ("abcdef", "ABCDEF", 0, "Case mismatch - uppercase input"),
    
    # Special characters
    ("test@example.com", "test@example.com/path", 1, "Email prefix match"),
    ("http://", "http://example.com", 1, "URL prefix match"),
    ("hello world", "hello world test", 1, "Space in param"),
    
    # Unicode tests
    ("café", "café au lait", 1, "Unicode prefix match"),
    ("测试", "测试数据", 1, "Chinese characters"),
])
def test_beginswith_operator(unit_test, param, input_data, expected_ret, test_name):
    """Test the @beginsWith operator with various inputs"""
    
    test_config = {
        "type": "op",
        "name": "beginsWith", 
        "param": param,
        "input": input_data,
        "ret": expected_ret
    }
    
    result = unit_test.run_unit_test(test_config)
    
    assert result.success, f"Test '{test_name}' failed: {result.message}"


class TestBeginsWithOperator:
    """Test class for grouping related beginsWith tests"""
    
    def test_empty_scenarios(self, unit_test):
        """Test edge cases with empty strings"""
        test_cases = [
            {"param": "", "input": "", "ret": 1, "desc": "Both empty"},
            {"param": "test", "input": "", "ret": 0, "desc": "Empty input"},
            {"param": "", "input": "test", "ret": 1, "desc": "Empty param"},
        ]
        
        for case in test_cases:
            test_config = {
                "type": "op",
                "name": "beginsWith",
                "param": case["param"],
                "input": case["input"],
                "ret": case["ret"]
            }
            
            result = unit_test.run_unit_test(test_config)
            assert result.success, f"Failed: {case['desc']} - {result.message}"
    
    def test_length_variations(self, unit_test):
        """Test with various string lengths"""
        base_string = "abcdefghijklmnopqrstuvwxyz"
        
        test_cases = [
            # Single character
            {"param": "a", "input": "abcdef", "ret": 1},
            {"param": "a", "input": "bcdef", "ret": 0},
            
            # Equal length
            {"param": base_string, "input": base_string, "ret": 1},
            
            # Param longer than input
            {"param": base_string, "input": "abc", "ret": 0},
            
            # Very long strings
            {"param": "x" * 1000, "input": "x" * 1500, "ret": 1},
            {"param": "x" * 1000, "input": "y" * 1000, "ret": 0},
        ]
        
        for i, case in enumerate(test_cases):
            test_config = {
                "type": "op",
                "name": "beginsWith",
                "param": case["param"],
                "input": case["input"], 
                "ret": case["ret"]
            }
            
            result = unit_test.run_unit_test(test_config)
            assert result.success, f"Length test {i+1} failed: {result.message}"
    
    def test_special_characters(self, unit_test):
        """Test with special characters and encoding"""
        test_cases = [
            # Newlines and whitespace. Note: a *leading* space in param is not
            # testable through msc_test's op harness - it builds the operator
            # invocation as a single "@beginsWith <param>" config line, and
            # leading whitespace in the param is trimmed by the config-line
            # parser before beginsWith ever sees it (trailing whitespace is
            # unaffected).
            {"param": "test\n", "input": "test\nmore", "ret": 1},
            {"param": "test ", "input": "test data", "ret": 1},
            {"param": "\t", "input": "\tindented", "ret": 1},
            
            # Null bytes
            {"param": "test\x00", "input": "test\x00more", "ret": 1},
            
            # HTML/XML characters  
            {"param": "<tag>", "input": "<tag>content</tag>", "ret": 1},
            {"param": "&amp;", "input": "&amp;entity", "ret": 1},
            
            # SQL injection patterns
            {"param": "' OR '1'='1", "input": "' OR '1'='1' --", "ret": 1},
            
            # Path traversal
            {"param": "../", "input": "../../../etc/passwd", "ret": 1},
        ]
        
        for i, case in enumerate(test_cases):
            test_config = {
                "type": "op",
                "name": "beginsWith",
                "param": case["param"],
                "input": case["input"],
                "ret": case["ret"]
            }
            
            result = unit_test.run_unit_test(test_config)
            assert result.success, f"Special char test {i+1} failed: {result.message}" 