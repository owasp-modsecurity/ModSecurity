### Empty
{
	type => "tfn",
	name => "hexEncode",
	input => "",
	output => "",
	ret => 1,
},

### Basic
{
	type => "tfn",
	name => "hexEncode",
	input => "TestCase",
	output => "5465737443617365",
	ret => 1,
},

### Basic w/NULL
{
	type => "tfn",
	name => "hexEncode",
	input => "Test\0Case",
	output => "546573740043617365",
	ret => 1,
},
