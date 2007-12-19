### Empty
{
	type => "tfns",
	name => "removeNulls",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfns",
	name => "removeNulls",
	input => "TestCase",
	output => "TestCase",
	ret => 0,
},
{
	type => "tfns",
	name => "removeNulls",
	input => "Test\x01Case",
	output => "Test\x01Case",
	ret => 0,
},


### Basics
{
	type => "tfns",
	name => "removeNulls",
	input => "\0TestCase",
	output => "TestCase",
	ret => 1,
},
{
	type => "tfns",
	name => "removeNulls",
	input => "Test\0Case",
	output => "TestCase",
	ret => 1,
},
{
	type => "tfns",
	name => "removeNulls",
	input => "Test\0\0Case",
	output => "TestCase",
	ret => 1,
},
{
	type => "tfns",
	name => "removeNulls",
	input => "TestCase\0",
	output => "TestCase",
	ret => 1,
},
{
	type => "tfns",
	name => "removeNulls",
	input => "\0Test\0Case\0",
	output => "TestCase",
	ret => 1,
},
