### Empty
{
	type => "tfn",
	name => "trim",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfn",
	name => "trim",
	input => "TestCase",
	output => "TestCase",
	ret => 0,
},
{
	type => "tfn",
	name => "trim",
	input => "Test\0Case",
	output => "Test\0Case",
	ret => 0,
},

### Basics
{
	type => "tfn",
	name => "trim",
	input => "    TestCase",
	output => "TestCase",
	ret => 1,
},
{
	type => "tfn",
	name => "trim",
	input => "TestCase    ",
	output => "TestCase",
	ret => 1,
},
{
	type => "tfn",
	name => "trim",
	input => "    TestCase    ",
	output => "TestCase",
	ret => 1,
},
{
	type => "tfn",
	name => "trim",
	input => "    Test   Case    ",
	output => "Test   Case",
	ret => 1,
},
{
	type => "tfn",
	name => "trim",
	input => "    Test \0 Case    ",
	output => "Test \0 Case",
	ret => 1,
},
{
	type => "tfn",
	name => "trim",
	input => " 	   Test \0 Case 	  	   \r\n  ",
	output => "Test \0 Case",
	ret => 1,
},
