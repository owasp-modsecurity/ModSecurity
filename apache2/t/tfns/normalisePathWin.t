### Empty
{
	type => "tfns",
	name => "normalisePathWin",
	input => "",
	output => "",
	ret => 0,
},

### Nothing but switch slashes
{
	type => "tfns",
	name => "normalisePathWin",
	input => "\\foo\\bar\\baz",
	output => "/foo/bar/baz",
	ret => 1,
},
{
	type => "tfns",
	name => "normalisePathWin",
	input => "\\foo\\bar\0\\baz",
	output => "/foo/bar\0/baz",
	ret => 1,
},

### Basics
{
	type => "tfns",
	name => "normalisePathWin",
	input => "\\foo\\bar\\\\baz",
	output => "/foo/bar/baz",
	ret => 1,
},
{
	type => "tfns",
	name => "normalisePathWin",
	input => "\\foo\\bar baz\\.\\.\\.\\.\\boo\\\\eek\\.\\.\\..\\whoa",
	output => "/foo/bar baz/boo/whoa",
	ret => 1,
},
{
	type => "tfns",
	name => "normalisePathWin",
	input => ".\\foo\\bar baz\\.\\.\\.\\.\\boo\\\\eek\\.\\.\\..\\whoa",
	output => "./foo/bar baz/boo/whoa",
	ret => 1,
},
{
	type => "tfns",
	name => "normalisePathWin",
	input => "\\.\\foo\\bar baz\\.\\.\\.\\.\\boo\\\\eek\\.\\.\\..\\whoa",
	output => "/foo/bar baz/boo/whoa",
	ret => 1,
},
{
	type => "tfns",
	name => "normalisePathWin",
	input => "\\\\foo\\bar baz\\.\\.\\.\\.\\boo\\\\eek\\.\\.\\..\\whoa",
	output => "/foo/bar baz/boo/whoa",
	ret => 1,
},
{
	type => "tfns",
	name => "normalisePathWin",
	input => "\\\\foo\\bar baz\\.\\.\\.\\.\\boo\\\\eek\\.\\.\\..\\whoa\\.\\",
	output => "/foo/bar baz/boo/whoa/",
	ret => 1,
},
{
	type => "tfns",
	name => "normalisePathWin",
	input => "\\.\\foo\\bar baz\\.\\.\\.\\.\\boo\\\\eek\\.\\.\\..\\whoa\\\\",
	output => "/foo/bar baz/boo/whoa/",
	ret => 1,
},
{
	type => "tfns",
	name => "normalisePathWin",
	input => "\\.\\..\\..\\..\\..\\..\\..\\..\\..\\etc\\passwd",
	output => "/etc/passwd",
	ret => 1,
},
{
	type => "tfns",
	name => "normalisePathWin",
	input => "\\.\\..\\.\\..\\..\\..\\..\\..\\..\\..\\etc\\..\\etc\\.\\passwd",
	output => "/etc/passwd",
	ret => 1,
},

### With NUL
{
	type => "tfns",
	name => "normalisePathWin",
	input => "\\.\\..\\.\\..\\..\\..\\..\\..\\..\\..\\\0\\..\\etc\\.\\passwd",
	output => "/etc/passwd",
	ret => 1,
},
