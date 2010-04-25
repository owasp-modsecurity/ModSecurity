### No Match
{
	type => "op",
	name => "pmFromFile",
	param => "op/pmFromFile-01.dat",
	input => "xxxyyyzzz",
	ret => 0,
},

### Multiple
{
	type => "op",
	name => "pmFromFile",
	param => "op/pmFromFile-01.dat",
	input => "defxxxyyy",
	ret => 1,
},
{
	type => "op",
	name => "pmFromFile",
	param => "op/pmFromFile-01.dat",
	input => "xxxdefyyy",
	ret => 1,
},
{
	type => "op",
	name => "pmFromFile",
	param => "op/pmFromFile-01.dat",
	input => "xxxyyydef",
	ret => 1,
},
{
	type => "op",
	name => "pmFromFile",
	param => "op/pmFromFile-01.dat",
	input => "xxx yyy zzz",
	ret => 1,
},
{
	type => "op",
	name => "pmFromFile",
	param => "op/pmFromFile-01.dat",
	input => "xxx yyy",
	ret => 0,
},
