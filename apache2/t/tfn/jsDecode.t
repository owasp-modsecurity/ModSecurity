### Empty
{
	type => "tfn",
	name => "jsDecode",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfn",
	name => "jsDecode",
	input => "TestCase",
	output => "TestCase",
	ret => 0,
},
{
	type => "tfn",
	name => "jsDecode",
	input => "Test\0Case",
	output => "Test\0Case",
	ret => 0,
},

### Valid Sequences
{
	type => "tfn",
	name => "jsDecode",
	input => "\\a\\b\\f\\n\\r\\t\\v\\?\\'\\\"\\0\\12\\123\\x00\\xff\\u0021\\uff01",
	output => "\a\b\f\x0a\x0d\t\x0b?'\"\x00\x0a\x53\x00\xff\x21\x21",
	ret => 1,
},
{
	type => "tfn",
	name => "jsDecode",
	input => "\\a\\b\\f\\n\\r\\t\\v\0\\?\\'\\\"\\0\\12\\123\\x00\\xff\\u0021\\uff01",
	output => "\a\b\f\x0a\x0d\t\x0b\0?'\"\x00\x0a\x53\x00\xff\x21\x21",
	ret => 1,
},

### Invalid Sequences
# \8 and \9 are not octal
# \666 is \66 + '6' (JS does not allow the overflow as C does)
# \u00ag, \u00ga, \u0zaa, \uz0aa are not hex
# \xag and \xga are not hex,
# \0123 is \012 + '3'
{
	type => "tfn",
	name => "jsDecode",
	input => "\\8\\9\\666\\u00ag\\u00ga\\u0zaa\\uz0aa\\xag\\xga\\0123\\u00a",
	output => "89\x366u00agu00gau0zaauz0aaxagxga\x0a3u00a",
	ret => 1,
},

# \x, \x0 lack enough hex digits
{
	type => "tfn",
	name => "jsDecode",
	input => "\\x",
	output => "x",
	ret => 1,
},
{
	type => "tfn",
	name => "jsDecode",
	input => "\\x\\x0",
	output => "xx0",
	ret => 1,
},
{
	type => "tfn",
	name => "jsDecode",
	input => "\\x\\x0\0",
	output => "xx0\0",
	ret => 1,
},
# \u, \u0 \u01, \u012 lack enough hex digits
{
	type => "tfn",
	name => "jsDecode",
	input => "\\u",
	output => "u",
	ret => 1,
},
{
	type => "tfn",
	name => "jsDecode",
	input => "\\u\\u0",
	output => "uu0",
	ret => 1,
},
{
	type => "tfn",
	name => "jsDecode",
	input => "\\u\\u0\\u01",
	output => "uu0u01",
	ret => 1,
},
{
	type => "tfn",
	name => "jsDecode",
	input => "\\u\\u0\\u01\\u012",
	output => "uu0u01u012",
	ret => 1,
},
{
	type => "tfn",
	name => "jsDecode",
	input => "\\u\\u0\\u01\\u012\0",
	output => "uu0u01u012\0",
	ret => 1,
},
# A forward slash with nothing after
{
	type => "tfn",
	name => "jsDecode",
	input => "\\",
	output => "\\",
	ret => 0,
},
# A forward slash with NUL after
{
	type => "tfn",
	name => "jsDecode",
	input => "\\\0",
	output => "\0",
	ret => 1,
},
