### Empty
{
	type => "tfns",
	name => "length",
	input => "",
	output => "0",
	ret => 1,
},


### Basic normal and large
{
	type => "tfns",
	name => "length",
	input => "0123456789abcdef",
	output => "16",
	ret => 1,
},
{
	type => "tfns",
	name => "length",
	input => ('x' x 8192),
	output => "8192",
	ret => 1,
},

### With TAB
{
	type => "tfns",
	name => "length",
	input => "0123456789\tabcdef",
	output => "17",
	ret => 1,
},

### With NUL
{
	type => "tfns",
	name => "length",
	input => "Test\0Case",
	output => "9",
	ret => 1,
},
