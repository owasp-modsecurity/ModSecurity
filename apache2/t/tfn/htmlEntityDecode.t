### Empty
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "TestCase",
	output => "TestCase",
	ret => 0,
},
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "Test\0Case",
	output => "Test\0Case",
	ret => 0,
},

### Valid
# With ;
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "&#x0;&#X0;&#x20;&#X20;&#0;&#32;\0&#100;&quot;&amp;&lt;&gt;&nbsp;",
	output => "\0\0\x20\x20\0\x20\0\x64\"&<>\xa0",
	ret => 1,
},
# Without ;
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "&#x0&#X0&#x20&#X20&#0&#32\0&#100&quot&amp&lt&gt&nbsp",
	output => "\0\0\x20\x20\0\x20\0\x64\"&<>\xa0",
	ret => 1,
},

### Invalid
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "&#xg;&#Xg;&#xg0;&#X2g;&#a;\0&#a2;&#3a&#a00;&#1a0;&#10a;&foo;",
	output => "&#xg;&#Xg;&#xg0;\x02g;&#a;\0&#a2;\x03a&#a00;\x01a0;\x0aa;&foo;",
	ret => 1,
},
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "&#xg&#Xg&#xg0&#X2g&#a\0&#a2&#3a&#a00&#1a0&#10a&foo",
	output => "&#xg&#Xg&#xg0\x02g&#a\0&#a2\x03a&#a00\x01a0\x0aa&foo",
	ret => 1,
},
