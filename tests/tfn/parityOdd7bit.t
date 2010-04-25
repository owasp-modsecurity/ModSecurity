### Empty
{
	type => "tfn",
	name => "parityOdd7bit",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfn",
	name => "parityOdd7bit",
	input => "abdghkmnpsuvyz12478CEFIJLOQRTW",
	output => "abdghkmnpsuvyz12478CEFIJLOQRTW",
	ret => 0,
},

### Parity
{
	type => "tfn",
	name => "parityOdd7bit",
	input => "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	output => "ab\xe3d\xe5\xe6gh\xe9\xeak\xecmn\xefp\xf1\xf2s\xf4uv\xf7\xf8yz\xb012\xb34\xb5\xb678\xb9\xc1\xc2C\xc4EF\xc7\xc8IJ\xcbL\xcd\xceO\xd0QR\xd3T\xd5\xd6WX\xd9\xda",
	ret => 1,
},
{
	type => "tfn",
	name => "parityOdd7bit",
	input => "abcdefghijklmnopqrstuvwxyz\x000123456789\x00ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	output => "ab\xe3d\xe5\xe6gh\xe9\xeak\xecmn\xefp\xf1\xf2s\xf4uv\xf7\xf8yz\x80\xb012\xb34\xb5\xb678\xb9\x80\xc1\xc2C\xc4EF\xc7\xc8IJ\xcbL\xcd\xceO\xd0QR\xd3T\xd5\xd6WX\xd9\xda",
	ret => 1,
},

