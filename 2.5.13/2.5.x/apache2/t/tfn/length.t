### Empty
{
	type => "tfn",
	name => "length",
	input => "",
	output => "0",
	ret => 1,
},


### Basic normal and large
{
	type => "tfn",
	name => "length",
	input => "0123456789abcdef",
	output => "16",
	ret => 1,
},
# ENH: This sometimes fails w/4096 length
#{
#	type => "tfn",
#	name => "length",
#	input => ('x' x 8192),
#	output => "8192",
#	ret => 1,
#},

### With TAB
{
	type => "tfn",
	name => "length",
	input => "0123456789\tabcdef",
	output => "17",
	ret => 1,
},

### With NUL
{
	type => "tfn",
	name => "length",
	input => "Test\0Case",
	output => "9",
	ret => 1,
},
