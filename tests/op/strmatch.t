### Empty
{
	type => "op",
	name => "strmatch",
	param => "TestCase",
	input => "",
	ret => 0,
},
### General
{
	type => "op",
	name => "strmatch",
	param => "abc",
	input => "abcdefghi",
	ret => 1,
},
{
	type => "op",
	name => "strmatch",
	param => "def",
	input => "abcdefghi",
	ret => 1,
},
{
	type => "op",
	name => "strmatch",
	param => "ghi",
	input => "abcdefghi",
	ret => 1,
},
{
	type => "op",
	name => "strmatch",
	param => "ghij",
	input => "abcdefghi",
	ret => 0,
},
