### Empty
{
	type => "op",
	name => "m",
	param => "",
	input => "",
	ret => 1,
},
{
	type => "op",
	name => "m",
	param => "TestCase",
	input => "",
	ret => 0,
},
{
	type => "op",
	name => "m",
	param => "",
	input => "TestCase",
	ret => 1,
},

### General
{
	type => "op",
	name => "m",
	param => "abc",
	input => "abcdefghi",
	ret => 1,
},
{
	type => "op",
	name => "m",
	param => "def",
	input => "abcdefghi",
	ret => 1,
},
{
	type => "op",
	name => "m",
	param => "ghi",
	input => "abcdefghi",
	ret => 1,
},
{
	type => "op",
	name => "m",
	param => "ghij",
	input => "abcdefghi",
	ret => 0,
},
