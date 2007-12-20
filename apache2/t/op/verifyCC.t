### Empty
{
	type => "op",
	name => "verifyCC",
	param => "\\d{15,16}",
	input => "",
	ret => 0,
},

### Non-matching
{
	type => "op",
	name => "verifyCC",
	param => "\\d{15,16}",
	input => "TestCase",
	ret => 0,
},

### Bad CC#
{
	type => "op",
	name => "verifyCC",
	param => "\\d{15,16}",
	input => "01234567890123456",
	ret => 0,
},

### Good CC#
{
	type => "op",
	name => "verifyCC",
	param => "\\d{15,16}",
	input => "372802490654059",
	ret => 1,
},
