### Empty
# NOTE: All will return 0 because of lacking DB
{
	type => "op",
	name => "geoLookup",
	param => "",
	input => "",
	ret => 0,
},
{
	type => "op",
	name => "geoLookup",
	param => "TestCase",
	input => "",
	ret => 0,
},

# Failed Lookup
{
	type => "op",
	name => "geoLookup",
	param => "",
	input => "127.0.0.1",
	ret => 0,
},

# Good
{
	type => "op",
	name => "geoLookup",
	param => "",
	input => "216.75.21.122",
	#ret => 1,
	ret => 0,
},
{
	type => "op",
	name => "geoLookup",
	param => "",
	input => "www.modsecurity.org",
	#ret => 1,
	ret => 0,
},

