### Empty
{
	type => "op",
	name => "gt",
	param => "0",
	input => "",
	ret => 0,
},
{
	type => "op",
	name => "gt",
	param => "5",
	input => "",
	ret => 0,
},

### Invalid
# xxx interpreted as 0
{
	type => "op",
	name => "gt",
	param => "xxx",
	input => "5",
	ret => 1,
},
# xxx interpreted as 0
{
	type => "op",
	name => "gt",
	param => "xxx",
	input => "-1",
	ret => 0,
},
# xxx interpreted as 0
{
	type => "op",
	name => "gt",
	param => "-1",
	input => "xxx",
	ret => 1,
},
# xxx interpreted as 0
{
	type => "op",
	name => "gt",
	param => "5",
	input => "xxx",
	ret => 0,
},

### General
{
	type => "op",
	name => "gt",
	param => "0",
	input => "-5",
	ret => 0,
},
{
	type => "op",
	name => "gt",
	param => "0",
	input => "0",
	ret => 0,
},
{
	type => "op",
	name => "gt",
	param => "0",
	input => "5",
	ret => 1,
},
{
	type => "op",
	name => "gt",
	param => "5",
	input => "0",
	ret => 0,
},
{
	type => "op",
	name => "gt",
	param => "5",
	input => "5",
	ret => 0,
},
{
	type => "op",
	name => "gt",
	param => "5",
	input => "10",
	ret => 1,
},
