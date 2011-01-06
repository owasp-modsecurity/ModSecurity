### Empty
{
	type => "tfn",
	name => "base64Encode",
	input => "",
	output => "",
	ret => 0,
},

### Test values with varying lengths to check padding
{
	type => "tfn",
	name => "base64Encode",
	input => "TestCase",
	output => "VGVzdENhc2U=",
	ret => 1,
},
{
	type => "tfn",
	name => "base64Encode",
	input => "TestCase1",
	output => "VGVzdENhc2Ux",
	ret => 1,
},
{
	type => "tfn",
	name => "base64Encode",
	input => "TestCase12",
	output => "VGVzdENhc2UxMg==",
	ret => 1,
},

### Check with a NUL
{
	type => "tfn",
	name => "base64Encode",
	input => "Test\0Case",
	output => "VGVzdABDYXNl",
	ret => 1,
},
