### Empty
{
	type => "op",
	name => "rx",
	param => "",
	input => "",
	ret => 1,
},
{
	type => "op",
	name => "rx",
	param => "TestCase",
	input => "",
	ret => 0,
},
{
	type => "op",
	name => "rx",
	param => "",
	input => "TestCase",
	ret => 1,
},

### General
{
	type => "op",
	name => "rx",
	param => "abc",
	input => "abcdefghi",
	ret => 1,
},
{
	type => "op",
	name => "rx",
	param => "def",
	input => "abcdefghi",
	ret => 1,
},
{
	type => "op",
	name => "rx",
	param => "ghi",
	input => "abcdefghi",
	ret => 1,
},
{
	type => "op",
	name => "rx",
	param => "ghij",
	input => "abcdefghi",
	ret => 0,
},

### Complex regex
{
	type => "op",
	name => "rx",
	param => "(?i:(sleep\\((\\s*?)(\\d*?)(\\s*?)\\)|benchmark\\((.*?)\\,(.*?)\\)))",
	input => "SELECT pg_sleep(10);",
	ret => 1,
},

