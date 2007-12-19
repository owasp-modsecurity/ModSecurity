### Empty
{
	type => "tfns",
	name => "lowercase",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfns",
	name => "lowercase",
	input => "testcase",
	output => "testcase",
	ret => 0,
},
{
	type => "tfns",
	name => "lowercase",
	input => "test\0case",
	output => "test\0case",
	ret => 0,
},

### Basic
{
	type => "tfns",
	name => "lowercase",
	input => "TestCase",
	output => "testcase",
	ret => 1,
},
{
	type => "tfns",
	name => "lowercase",
	input => "Test\0Case",
	output => "test\0case",
	ret => 1,
},
