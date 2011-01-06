### Empty
{
	type => "op",
	name => "streq",
	param => "",
	input => "",
	ret => 1,
},
{
	type => "op",
	name => "streq",
	param => "TestCase",
	input => "",
	ret => 0,
},
{
	type => "op",
	name => "streq",
	param => "",
	input => "TestCase",
	ret => 0,
},

### General
{
	type => "op",
	name => "streq",
	param => "abc",
	input => "abcdefghi",
	ret => 0,
},
{
	type => "op",
	name => "streq",
	param => "def",
	input => "abcdefghi",
	ret => 0,
},
{
	type => "op",
	name => "streq",
	param => "ghi",
	input => "abcdefghi",
	ret => 0,
},
{
	type => "op",
	name => "streq",
	param => "abcdefghi",
	input => "abcdefghi",
	ret => 1,
},
