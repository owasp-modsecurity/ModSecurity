### Empty
{
	type => "tfn",
	name => "compressWhitespace",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfn",
	name => "compressWhitespace",
	input => "TestCase",
	output => "TestCase",
	ret => 0,
},
{
	type => "tfn",
	name => "compressWhitespace",
	input => "Test\0Case",
	output => "Test\0Case",
	ret => 0,
},
{
	type => "tfn",
	name => "compressWhitespace",
	input => "Test Case",
	output => "Test Case",
	ret => 0,
},


### Compress space/tab
{
	type => "tfn",
	name => "compressWhitespace",
	input => "  Test  \t   Case  ",
	output => " Test Case ",
	ret => 1,
},

### Pretty much everything in one
{
	type => "tfn",
	name => "compressWhitespace",
	input => "This is a   test case with a tab \t, vtab \x0b, newline \x0a, return \x0d, formfeed \f, and a NUL\0 in it with a CRLF at the end.\x0d\x0a",
	output => "This is a test case with a tab , vtab , newline , return , formfeed , and a NUL\0 in it with a CRLF at the end. ",
	ret => 1,
},
