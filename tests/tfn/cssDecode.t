### Empty
{
	type => "tfn",
	name => "cssDecode",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfn",
	name => "cssDecode",
	input => "TestCase",
	output => "TestCase",
	ret => 0,
},
{
	type => "tfn",
	name => "cssDecode",
	input => "Test\0Case",
	output => "Test\0Case",
	ret => 0,
},

### Valid Sequences
{
	type => "tfn",
	name => "cssDecode",
	input => "test\\a\\b\\f\\n\\r\\t\\v\\?\\'\\\"\\0\\12\\123\\1234\\12345\\123456\\ff01\\ff5e\\\n\\0  string",
	output => qq(test\x0a\x0b\x0fnrtv?'"\x00\x12\x23\x34\x45\x56\x21\x7e\x00 string),
	ret => 1,
},

### Invalid Sequences
# Trailing escape == line continuation with no line following (ie nothing)
{
	type => "tfn",
	name => "cssDecode",
	input => "test\\",
	output => "test",
	ret => 1,
},

# Edge cases
#  "\1A" == "\x1A"
#  "\1 A" == "\x01A"
#  "\1234567" == "\x567"
#  "\123456 7" == "\x567"
#  "\1x" == "\x01x"
#  "\1 x" == "\x01 x"
{
	type => "tfn",
	name => "cssDecode",
	input => "\\1A\\1 A\\1234567\\123456 7\\1x\\1 x",
	output => "\x1A\x01A\x567\x567\x01x\x01x",
	ret => 1,
},
