### Empty
{
	type => "tfn",
	name => "removeWhitespace",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfn",
	name => "removeWhitespace",
	input => "TestCase",
	output => "TestCase",
	ret => 0,
},
{
	type => "tfn",
	name => "removeWhitespace",
	input => "Test\0Case",
	output => "Test\0Case",
	ret => 0,
},


### Remove space/tab
{
	type => "tfn",
	name => "removeWhitespace",
	input => "  Test  \t   Case  ",
	output => "TestCase",
	ret => 1,
},

### Pretty much everything in one
{
	type => "tfn",
	name => "removeWhitespace",
	input => "This is a   test case with a tab \t, vtab \x0b, newline \x0a, return \x0d, formfeed \f, and a NUL\0 in it with a CRLF at the end.\x0d\x0a",
	output => "Thisisatestcasewithatab,vtab,newline,return,formfeed,andaNUL\0initwithaCRLFattheend.",
	ret => 1,
},
