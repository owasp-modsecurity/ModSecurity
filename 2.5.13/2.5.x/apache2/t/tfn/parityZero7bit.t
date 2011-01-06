### Empty
{
	type => "tfn",
	name => "parityZero7bit",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfn",
	name => "parityZero7bit",
	input => "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	output => "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	ret => 0,
},
{
	type => "tfn",
	name => "parityZero7bit",
	input => "abcdefghijklmnopqrstuvwxyz\x000123456789\x00ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	output => "abcdefghijklmnopqrstuvwxyz\x000123456789\x00ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	ret => 0,
},

### Basic
{
	type => "tfn",
	name => "parityZero7bit",
	input => "\x80\x00\x8f\xff",
	output => "\x00\x00\x0f\x7f",
	ret => 1,
},
