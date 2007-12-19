### Empty
{
	type => "tfns",
	name => "replaceNulls",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfns",
	name => "replaceNulls",
	input => "TestCase",
	output => "TestCase",
	ret => 0,
},


### Basics
{
	type => "tfns",
	name => "replaceNulls",
	input => "\0TestCase",
	output => " TestCase",
	ret => 1,
},
{
	type => "tfns",
	name => "replaceNulls",
	input => "Test\0Case",
	output => "Test Case",
	ret => 1,
},
{
	type => "tfns",
	name => "replaceNulls",
	input => "Test\0\0Case",
	output => "Test  Case",
	ret => 1,
},
{
	type => "tfns",
	name => "replaceNulls",
	input => "TestCase\0",
	output => "TestCase ",
	ret => 1,
},
{
	type => "tfns",
	name => "replaceNulls",
	input => "\0Test\0Case\0",
	output => " Test Case ",
	ret => 1,
},
