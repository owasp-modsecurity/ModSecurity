### Empty
{
	type => "tfns",
	name => "trim",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfns",
	name => "trim",
	input => "TestCase",
	output => "TestCase",
	ret => 0,
},
{
	type => "tfns",
	name => "trim",
	input => "Test\0Case",
	output => "Test\0Case",
	ret => 0,
},

### Basics
{
	type => "tfns",
	name => "trim",
	input => "    TestCase",
	output => "TestCase",
	ret => 1,
},
{
	type => "tfns",
	name => "trim",
	input => "TestCase    ",
	output => "TestCase",
	ret => 1,
},
{
	type => "tfns",
	name => "trim",
	input => "    TestCase    ",
	output => "TestCase",
	ret => 1,
},
{
	type => "tfns",
	name => "trim",
	input => "    Test   Case    ",
	output => "Test   Case",
	ret => 1,
},
{
	type => "tfns",
	name => "trim",
	input => "    Test \0 Case    ",
	output => "Test \0 Case",
	ret => 1,
},
{
	type => "tfns",
	name => "trim",
	input => " 	   Test \0 Case 	  	   \r\n  ",
	output => "Test \0 Case",
	ret => 1,
},
