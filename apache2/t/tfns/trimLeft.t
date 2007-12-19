### Empty
{
	type => "tfns",
	name => "trimLeft",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfns",
	name => "trimLeft",
	input => "TestCase",
	output => "TestCase",
	ret => 0,
},
{
	type => "tfns",
	name => "trimLeft",
	input => "Test\0Case",
	output => "Test\0Case",
	ret => 0,
},
{
	type => "tfns",
	name => "trimLeft",
	input => "TestCase    ",
	output => "TestCase    ",
	ret => 0,
},


### Basics
{
	type => "tfns",
	name => "trimLeft",
	input => "    TestCase",
	output => "TestCase",
	ret => 1,
},
{
	type => "tfns",
	name => "trimLeft",
	input => "    TestCase    ",
	output => "TestCase    ",
	ret => 1,
},
{
	type => "tfns",
	name => "trimLeft",
	input => "    Test   Case    ",
	output => "Test   Case    ",
	ret => 1,
},
{
	type => "tfns",
	name => "trimLeft",
	input => "    Test \0 Case    ",
	output => "Test \0 Case    ",
	ret => 1,
},
{
	type => "tfns",
	name => "trimLeft",
	input => " 	   Test \0 Case 	  	   \r\n  ",
	output => "Test \0 Case 	  	   \r\n  ",
	ret => 1,
},
