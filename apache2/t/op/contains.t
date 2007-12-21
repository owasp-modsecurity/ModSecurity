### Empty
{
	type => "op",
	name => "contains",
	param => "",
	input => "",
	ret => 0,
},
{
	type => "op",
	name => "contains",
	param => "TestCase",
	input => "",
	ret => 0,
},
{
	type => "op",
	name => "contains",
	param => "",
	input => "TestCase",
	ret => 0,
},

### General
{
	type => "op",
	name => "contains",
	param => "abc",
	input => "abcdefghi",
	ret => 1,
},
{
	type => "op",
	name => "contains",
	param => "def",
	input => "abcdefghi",
	ret => 1,
},
{
	type => "op",
	name => "contains",
	param => "ghi",
	input => "abcdefghi",
	ret => 1,
},
{
	type => "op",
	name => "contains",
	param => "ghij",
	input => "abcdefghi",
	ret => 0,
},
