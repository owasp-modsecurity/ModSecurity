### Empty
{
	type => "tfns",
	name => "hexDecode",
	input => "",
	output => "",
	ret => 1,
},

### Basic
{
	type => "tfns",
	name => "hexDecode",
	input => "5465737443617365",
	output => "TestCase",
	ret => 1,
},

### Basic w/NULL
{
	type => "tfns",
	name => "hexDecode",
	input => "546573740043617365",
	output => "Test\0Case",
	ret => 1,
},

### Invalid
# What should happen here?  Probably just fail and leave alone.
{
	type => "tfns",
	name => "hexDecode",
	input => "01234567890a0z01234567890a",
	output => "01234567890a0z01234567890a",
	ret => 0,
},
{
	type => "tfns",
	name => "hexDecode",
	input => "01234567890az",
	output => "01234567890az",
	ret => 0,
},
{
	type => "tfns",
	name => "hexDecode",
	input => "01234567890a0",
	output => "01234567890a0",
	ret => 0,
},
