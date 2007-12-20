### Empty
{
	type => "tfn",
	name => "escapeSeqDecode",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfn",
	name => "escapeSeqDecode",
	input => "TestCase",
	output => "TestCase",
	ret => 0,
},
{
	type => "tfn",
	name => "escapeSeqDecode",
	input => "Test\0Case",
	output => "Test\0Case",
	ret => 0,
},

### Valid Sequences
{
	type => "tfn",
	name => "escapeSeqDecode",
	input => "\\a\\b\\f\\n\\r\\t\\v\\?\\'\\\"\\0\\12\\123\\x00\\xff",
	output => "\a\b\f\x0a\x0d\t\x0b?'\"\x00\x0a\x53\x00\xff",
	ret => 1,
},
{
	type => "tfn",
	name => "escapeSeqDecode",
	input => "\\a\\b\\f\\n\\r\\t\\v\0\\?\\'\\\"\\0\\12\\123\\x00\\xff",
	output => "\a\b\f\x0a\x0d\t\x0b\0?'\"\x00\x0a\x53\x00\xff",
	ret => 1,
},

### Invalid Sequences
# \8 and \9 are not octal
# \666 is a byte overflow (0x1b6) and should be truncated to a byte as 0xb6
# \xag and \xga are not hex,
# \0123 is \012 + '3'
{
	type => "tfn",
	name => "escapeSeqDecode",
	input => "\\8\\9\\666\\xag\\xga\\0123",
	output => "89\xb6xagxga\x0a3",
	ret => 1,
},

# \x, \x0 lack enough hex digits
{
	type => "tfn",
	name => "escapeSeqDecode",
	input => "\\x",
	output => "x",
	ret => 1,
},
{
	type => "tfn",
	name => "escapeSeqDecode",
	input => "\\x\\x0",
	output => "xx0",
	ret => 1,
},
{
	type => "tfn",
	name => "escapeSeqDecode",
	input => "\\x\\x0\0",
	output => "xx0\0",
	ret => 1,
},
# Octal at end
{
	type => "tfn",
	name => "escapeSeqDecode",
	input => "\\0",
	output => "\x00",
	ret => 1,
},
{
	type => "tfn",
	name => "escapeSeqDecode",
	input => "\\01",
	output => "\x01",
	ret => 1,
},
{
	type => "tfn",
	name => "escapeSeqDecode",
	input => "\\012",
	output => "\x0a",
	ret => 1,
},
# A forward slash with nothing after
{
	type => "tfn",
	name => "escapeSeqDecode",
	input => "\\",
	output => "\\",
	ret => 0,
},
# A forward slash with NUL after
{
	type => "tfn",
	name => "escapeSeqDecode",
	input => "\\\0",
	output => "\0",
	ret => 1,
},
