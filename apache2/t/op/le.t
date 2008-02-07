### Empty
{
	type => "op",
	name => "le",
	param => "0",
	input => "",
	ret => 1,
},
{
	type => "op",
	name => "le",
	param => "5",
	input => "",
	ret => 1,
},

### Invalid
# xxx interpreted as 0
{
	type => "op",
	name => "le",
	param => "xxx",
	input => "5",
	ret => 0,
},
# xxx interpreted as 0
{
	type => "op",
	name => "le",
	param => "xxx",
	input => "-1",
	ret => 1,
},
# xxx interpreted as 0
{
	type => "op",
	name => "le",
	param => "0",
	input => "xxx",
	ret => 1,
},
# xxx interpreted as 0
{
	type => "op",
	name => "le",
	param => "5",
	input => "xxx",
	ret => 1,
},

### General
{
	type => "op",
	name => "le",
	param => "0",
	input => "-5",
	ret => 1,
},
{
	type => "op",
	name => "le",
	param => "0",
	input => "0",
	ret => 1,
},
{
	type => "op",
	name => "le",
	param => "0",
	input => "5",
	ret => 0,
},
{
	type => "op",
	name => "le",
	param => "5",
	input => "0",
	ret => 1,
},
{
	type => "op",
	name => "le",
	param => "5",
	input => "5",
	ret => 1,
},
{
	type => "op",
	name => "le",
	param => "5",
	input => "10",
	ret => 0,
},
