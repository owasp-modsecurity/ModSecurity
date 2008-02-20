### Empty
{
	type => "tfn",
	name => "hexDecode",
	input => "",
	output => "",
	ret => 1,
},

### Basic
{
	type => "tfn",
	name => "hexDecode",
	input => "5465737443617365",
	output => "TestCase",
	ret => 1,
},

### Basic w/NULL
{
	type => "tfn",
	name => "hexDecode",
	input => "546573740043617365",
	output => "Test\0Case",
	ret => 1,
},

### Invalid
# What should happen here?  Probably just fail and leave alone.
{
	type => "tfn",
	name => "hexDecode",
	input => "01234567890a0z01234567890a",
	output => "\x01#Eg\x89\x0a#\x01#Eg\x89\x0a",
	ret => 1,
},
{
	type => "tfn",
	name => "hexDecode",
	input => "01234567890az",
	output => "\x01#Eg\x89\x0a",
	ret => 1,
},
{
	type => "tfn",
	name => "hexDecode",
	input => "01234567890a0",
	output => "\x01#Eg\x89\x0a",
	ret => 1,
},
