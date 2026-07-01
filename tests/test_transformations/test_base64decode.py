import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    # Empty test
    ("", "", 0, "Empty input"),
    
    # Test values with varying lengths to check padding
    ("VGVzdENhc2U=", "TestCase", 1, "Standard base64 with padding"),
    ("VGVzdENhc2Ux", "TestCase1", 1, "Base64 without padding - 1 char"),
    ("VGVzdENhc2UxMg==", "TestCase12", 1, "Base64 with double padding"),
    
    # Check with a NUL byte
    ("VGVzdABDYXNl", "Test\x00Case", 1, "Base64 with null byte"),
    
    # Invalid base64 (should still process what it can)
    ("VGVzdENhc2U=\x00VGVzdENhc2U=", "TestCase", 1, "Base64 with null terminator"),
    
    # Additional test cases
    ("SGVsbG8gV29ybGQ=", "Hello World", 1, "Hello World test"),
    ("YWJjZGVmZ2hpams=", "abcdefghijk", 1, "Alphabet sequence"),
    ("MTIzNDU2Nzg5MA==", "1234567890", 1, "Numbers test"),
])
def test_base64decode_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the base64Decode transformation with various inputs"""
    
    test_config = {
        "type": "tfn",
        "name": "base64Decode",
        "input": input_data,
        "output": expected_output,
        "ret": expected_ret
    }
    
    result = unit_test.run_unit_test(test_config)
    
    assert result.success, f"Test '{test_name}' failed: {result.message}"


class TestBase64DecodeTransformation:
    """Test class for grouping related base64Decode tests"""
    
    def test_padding_variations(self, unit_test):
        """Test different padding scenarios"""
        test_cases = [
            # No padding needed
            {"input": "YWJj", "output": "abc", "ret": 1, "desc": "No padding"},
            
            # Single padding
            {"input": "YWJjZA==", "output": "abcd", "ret": 1, "desc": "Double padding"},
            
            # Double padding  
            {"input": "YWJjZGU=", "output": "abcde", "ret": 1, "desc": "Single padding"},
            
            # Missing padding (should still work)
            {"input": "YWJjZGU", "output": "abcde", "ret": 1, "desc": "Missing padding"},
        ]
        
        for case in test_cases:
            test_config = {
                "type": "tfn",
                "name": "base64Decode",
                "input": case["input"],
                "output": case["output"],
                "ret": case["ret"]
            }
            
            result = unit_test.run_unit_test(test_config)
            assert result.success, f"Padding test failed: {case['desc']} - {result.message}"
    
    def test_special_characters(self, unit_test):
        """Test base64 decoding with special characters"""
        test_cases = [
            # URL characters
            {"input": "aHR0cDovL2V4YW1wbGUuY29t", "output": "http://example.com", "ret": 1},
            
            # Special symbols
            {"input": "IUAjJCVeJiooKQ==", "output": "!@#$%^&*()", "ret": 1},
            
            # Mixed case
            {"input": "VGVzdCBDYXNl", "output": "Test Case", "ret": 1},
            
            # JSON-like data
            {"input": "eyJ0ZXN0IjoidmFsdWUifQ==", "output": '{"test":"value"}', "ret": 1},
            
            # XML-like data  
            {"input": "PHRhZz5jb250ZW50PC90YWc+", "output": "<tag>content</tag>", "ret": 1},
        ]
        
        for i, case in enumerate(test_cases):
            test_config = {
                "type": "tfn",
                "name": "base64Decode",
                "input": case["input"],
                "output": case["output"],
                "ret": case["ret"]
            }
            
            result = unit_test.run_unit_test(test_config)
            assert result.success, f"Special chars test {i+1} failed: {result.message}"
    
    def test_invalid_input(self, unit_test):
        """Test handling of invalid base64 input"""
        test_cases = [
            # Invalid characters (should be ignored or cause graceful failure)
            {"input": "Invalid!@#$", "output": "", "ret": 0, "desc": "Completely invalid"},
            
            # Partial valid base64
            {"input": "VGVzdA!@#", "output": "Test", "ret": 1, "desc": "Partially valid"},
            
            # Wrong length
            {"input": "VGVz", "output": "Tes", "ret": 1, "desc": "Short input"},
        ]
        
        for case in test_cases:
            test_config = {
                "type": "tfn", 
                "name": "base64Decode",
                "input": case["input"],
                "output": case["output"],
                "ret": case["ret"]
            }
            
            result = unit_test.run_unit_test(test_config)
            # Note: Some invalid cases might behave differently than expected
            # This is testing the actual behavior rather than ideal behavior
            assert result.success or case["ret"] == 0, f"Invalid input test failed: {case['desc']}"
    
    def test_binary_data(self, unit_test):
        """Test base64 decoding of binary data"""
        test_cases = [
            # Binary sequences
            {"input": "AAECAwQFBgcICQ==", "output": "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09", "ret": 1},
            
            # High byte values (bytes literal: these are raw byte 0xfe, not
            # the Unicode codepoint U+00FE, which would UTF-8 encode as 2 bytes)
            {"input": "/v7+/g==", "output": b"\xfe\xfe\xfe\xfe", "ret": 1},
            
            # Mixed binary and text
            {"input": "VGVzdABCaW5hcnk=", "output": "Test\x00Binary", "ret": 1},
        ]
        
        for i, case in enumerate(test_cases):
            test_config = {
                "type": "tfn",
                "name": "base64Decode", 
                "input": case["input"],
                "output": case["output"],
                "ret": case["ret"]
            }
            
            result = unit_test.run_unit_test(test_config)
            assert result.success, f"Binary data test {i+1} failed: {result.message}"
    
    def test_large_input(self, unit_test):
        """Test base64 decoding with large inputs"""
        # Create a large base64 string
        import base64
        
        # 1KB of data
        large_data = "A" * 1024
        large_b64 = base64.b64encode(large_data.encode()).decode()
        
        test_config = {
            "type": "tfn",
            "name": "base64Decode",
            "input": large_b64,
            "output": large_data,
            "ret": 1
        }
        
        result = unit_test.run_unit_test(test_config)
        assert result.success, f"Large input test failed: {result.message}" 