### Empty
{
	type => "op",
	name => "endsWith",
	param => "",
	input => "",
	ret => 0,
},
{
	type => "op",
	name => "endsWith",
	param => "TestCase",
	input => "",
	ret => 0,
},
{
	type => "op",
	name => "endsWith",
	param => "",
	input => "TestCase",
	ret => 0,
},

### General
{
	type => "op",
	name => "endsWith",
	param => "abc",
	input => "abcdefghi",
	ret => 0,
},
{
	type => "op",
	name => "endsWith",
	param => "def",
	input => "abcdefghi",
	ret => 0,
},
{
	type => "op",
	name => "endsWith",
	param => "ghi",
	input => "abcdefghi",
	ret => 1,
},
{
	type => "op",
	name => "endsWith",
	param => "ghi",
	input => "abcdef\0ghi",
	ret => 1,
},
