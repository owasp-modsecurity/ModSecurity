### Empty
{
	type => "tfn",
	name => "normalisePath",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfn",
	name => "normalisePath",
	input => "/foo/bar/baz",
	output => "/foo/bar/baz",
	ret => 0,
},
{
	type => "tfn",
	name => "normalisePath",
	input => "/foo/bar\0/baz",
	output => "/foo/bar\0/baz",
	ret => 0,
},

### Basic
{
	type => "tfn",
	name => "normalisePath",
	input => "/foo/bar//baz",
	output => "/foo/bar/baz",
	ret => 1,
},
{
	type => "tfn",
	name => "normalisePath",
	input => "/foo/bar baz/././././boo//eek/././../whoa",
	output => "/foo/bar baz/boo/whoa",
	ret => 1,
},
{
	type => "tfn",
	name => "normalisePath",
	input => "./foo/bar baz/././././boo//eek/././../whoa",
	output => "./foo/bar baz/boo/whoa",
	ret => 1,
},
{
	type => "tfn",
	name => "normalisePath",
	input => "/./foo/bar baz/././././boo//eek/././../whoa",
	output => "/foo/bar baz/boo/whoa",
	ret => 1,
},
{
	type => "tfn",
	name => "normalisePath",
	input => "//foo/bar baz/././././boo//eek/././../whoa",
	output => "/foo/bar baz/boo/whoa",
	ret => 1,
},
{
	type => "tfn",
	name => "normalisePath",
	input => "//foo/bar baz/././././boo//eek/././../whoa/./",
	output => "/foo/bar baz/boo/whoa/",
	ret => 1,
},
{
	type => "tfn",
	name => "normalisePath",
	input => "/./foo/bar baz/././././boo//eek/././../whoa//",
	output => "/foo/bar baz/boo/whoa/",
	ret => 1,
},
{
	type => "tfn",
	name => "normalisePath",
	input => "/./../../../../../../../../etc/passwd",
	output => "/etc/passwd",
	ret => 1,
},
{
	type => "tfn",
	name => "normalisePath",
	input => "/./.././../../../../../../../etc/../etc/./passwd",
	output => "/etc/passwd",
	ret => 1,
},

### With NUL
{
	type => "tfn",
	name => "normalisePath",
	input => "/./.././../../../../../../../\0/../etc/./passwd",
	output => "/etc/passwd",
	ret => 1,
},
