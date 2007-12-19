### Empty
{
	type => "op",
	name => "verifyCC",
	param => "\\d{15,16}",
	input => "",
	rc => 0,
},

### Non-matching
{
	type => "op",
	name => "verifyCC",
	param => "\\d{15,16}",
	input => "TestCase",
	rc => 0,
},

### Bad CC#
{
	type => "op",
	name => "verifyCC",
	param => "\\d{15,16}",
	input => "01234567890123456",
	rc => 0,
},

### Good CC#
{
	type => "op",
	name => "verifyCC",
	param => "\\d{15,16}",
	input => "372802490654059",
	rc => 0,
},
