### Empty
{
	type => "tfn",
	name => "lowercase",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfn",
	name => "lowercase",
	input => "testcase",
	output => "testcase",
	ret => 0,
},
{
	type => "tfn",
	name => "lowercase",
	input => "test\0case",
	output => "test\0case",
	ret => 0,
},

### Basic
{
	type => "tfn",
	name => "lowercase",
	input => "TestCase",
	output => "testcase",
	ret => 1,
},
{
	type => "tfn",
	name => "lowercase",
	input => "Test\0Case",
	output => "test\0case",
	ret => 1,
},
