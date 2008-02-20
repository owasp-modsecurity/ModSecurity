### Empty
{
	type => "op",
	name => "within",
	param => "",
	input => "",
	ret => 1,
},
{
	type => "op",
	name => "within",
	param => "TestCase",
	input => "",
	ret => 1,
},
{
	type => "op",
	name => "within",
	param => "",
	input => "TestCase",
	ret => 0,
},

### General
{
	type => "op",
	name => "within",
	param => "abcdefghi",
	input => "abc",
	ret => 1,
},
{
	type => "op",
	name => "within",
	param => "abcdefghi",
	input => "def",
	ret => 1,
},
{
	type => "op",
	name => "within",
	param => "abcdefghi",
	input => "ghi",
	ret => 1,
},
{
	type => "op",
	name => "within",
	param => "abcdefghi",
	input => "ghij",
	ret => 0,
},
