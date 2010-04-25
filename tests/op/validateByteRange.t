### Empty
{
	type => "op",
	name => "validateByteRange",
	param => "0-255",
	input => "",
	ret => 0,
},
{
	type => "op",
	name => "validateByteRange",
	param => "",
	input => "TestCase",
	ret => 1,
},

### Invalid
{
	type => "op",
	name => "validateByteRange",
	param => "xxx",
	input => "TestCase",
	ret => 1,
},
{
	type => "op",
	name => "validateByteRange",
	param => "xxx",
	input => "\x00",
	ret => 0,
},

### General
{
	type => "op",
	name => "validateByteRange",
	param => "0-255",
	input => "abcdefghi",
	ret => 0,
},
{
	type => "op",
	name => "validateByteRange",
	param => ord("a")."-".ord("i"),
	input => "abcdefghi",
	ret => 0,
},
{
	type => "op",
	name => "validateByteRange",
	param => ord("a")."-".ord("i"),
	input => "abcdefghij",
	ret => 1,
},
