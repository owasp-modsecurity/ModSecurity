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

### GHSA-cxqf-vgrr-xxrv: advisory bypass cases
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "javascript&colon;alert(1)",
	output => "javascript:alert(1)",
	ret => 1,
},
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "it&apos;s",
	output => "it's",
	ret => 1,
},

### GHSA-cxqf-vgrr-xxrv: full set of newly-supported ASCII entities
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "&num;&dollar;&percnt;&lpar;&rpar;&ast;&plus;&comma;&period;&sol;&semi;&equals;&quest;&commat;&excl;",
	output => "#\$%()*+,./;=?\@!",
	ret => 1,
},
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "&lbrack;&bsol;&rbrack;&lowbar;&grave;&lbrace;&verbar;&rbrace;&Hat;",
	output => "[\\]_`{|}^",
	ret => 1,
},
# Aliases for the same characters -- a hand-written list misses these
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "&lcub;&rcub;&lsqb;&rsqb;&vert;&VerticalLine;&midast;&UnderBar;&DiacriticalGrave;",
	output => "{}[]||*_`",
	ret => 1,
},
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "&NewLine;&Tab;&shy;&NonBreakingSpace;",
	output => "\n\t\xad\xa0",
	ret => 1,
},
# CRS 944150 matches "&l(?:brace|cub);?" because real Log4Shell payloads use
# both spellings; the decoder must handle both.
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "&dollar;&lcub;jndi&colon;ldap&rcub;",
	output => "\${jndi:ldap}",
	ret => 1,
},
# Entities the specification does NOT map into ASCII must pass through:
# &caret; is U+2041, &hyphen; is U+2010, &tilde; is U+02DC.
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "&caret;&hyphen;&tilde;",
	output => "&caret;&hyphen;&tilde;",
	ret => 0,
},
# Prefix collisions must not decode (v3 regression; v2 was already correct).
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "&ltest;&amped;&gtfoo;",
	output => "&ltest;&amped;&gtfoo;",
	ret => 0,
},

### Case-insensitive lookup (preserved from prior behavior)
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "&COLON;&Apos;",
	output => ":'",
	ret => 1,
},

### Unknown entities pass through unchanged
{
	type => "tfn",
	name => "htmlEntityDecode",
	input => "&unknown;",
	output => "&unknown;",
	ret => 0,
},
