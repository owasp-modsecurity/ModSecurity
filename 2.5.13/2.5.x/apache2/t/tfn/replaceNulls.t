### Empty
{
	type => "tfn",
	name => "replaceNulls",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfn",
	name => "replaceNulls",
	input => "TestCase",
	output => "TestCase",
	ret => 0,
},


### Basics
{
	type => "tfn",
	name => "replaceNulls",
	input => "\0TestCase",
	output => " TestCase",
	ret => 1,
},
{
	type => "tfn",
	name => "replaceNulls",
	input => "Test\0Case",
	output => "Test Case",
	ret => 1,
},
{
	type => "tfn",
	name => "replaceNulls",
	input => "Test\0\0Case",
	output => "Test  Case",
	ret => 1,
},
{
	type => "tfn",
	name => "replaceNulls",
	input => "TestCase\0",
	output => "TestCase ",
	ret => 1,
},
{
	type => "tfn",
	name => "replaceNulls",
	input => "\0Test\0Case\0",
	output => " Test Case ",
	ret => 1,
},
