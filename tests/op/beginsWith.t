### Empty
{
	type => "op",
	name => "beginsWith",
	param => "",
	input => "",
	ret => 1,
},
{
	type => "op",
	name => "beginsWith",
	param => "TestCase",
	input => "",
	ret => 0,
},
{
	type => "op",
	name => "beginsWith",
	param => "",
	input => "TestCase",
	ret => 1,
},

### General
{
	type => "op",
	name => "beginsWith",
	param => "abcdef",
	input => "abcdef",
	ret => 1,
},
{
	type => "op",
	name => "beginsWith",
	param => "abcdef",
	input => "abcdefghi",
	ret => 1,
},
{
	type => "op",
	name => "beginsWith",
	param => "abcdef",
	input => "abc",
	ret => 0,
},
