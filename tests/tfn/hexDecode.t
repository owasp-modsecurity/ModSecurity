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
	output => "Test00Case",
	ret => 1,
},

### Invalid
# What should happen here?  Probably just fail and leave alone.
{
	type => "tfn",
	name => "hexDecode",
	input => "01234567890a0z01234567890a",
	output => "01#Eg890a0z01#Eg890a",
	ret => 1,
},
{
	type => "tfn",
	name => "hexDecode",
	input => "01234567890az",
	output => "01#Eg890az",
	ret => 1,
},
{
	type => "tfn",
	name => "hexDecode",
	input => "01234567890a0",
	output => "01#Eg890a0",
	ret => 1,
},
