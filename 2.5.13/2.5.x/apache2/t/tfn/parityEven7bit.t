### Empty
{
	type => "tfn",
	name => "parityEven7bit",
	input => "",
	output => "",
	ret => 0,
},

### Nothing
{
	type => "tfn",
	name => "parityEven7bit",
	input => "cefijloqrtwx03569ABDGHKMNPSUVYZ",
	output => "cefijloqrtwx03569ABDGHKMNPSUVYZ",
	ret => 0,
},

### Parity
{
	type => "tfn",
	name => "parityEven7bit",
	input => "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	output => "\xe1\xe2c\xe4ef\xe7\xe8ij\xebl\xed\xeeo\xf0qr\xf3t\xf5\xf6wx\xf9\xfa0\xb1\xb23\xb456\xb7\xb89AB\xc3D\xc5\xc6GH\xc9\xcaK\xccMN\xcfP\xd1\xd2S\xd4UV\xd7\xd8YZ",
	ret => 1,
},
{
	type => "tfn",
	name => "parityEven7bit",
	input => "abcdefghijklmnopqrstuvwxyz\x000123456789\x00ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	output => "\xe1\xe2c\xe4ef\xe7\xe8ij\xebl\xed\xeeo\xf0qr\xf3t\xf5\xf6wx\xf9\xfa\x000\xb1\xb23\xb456\xb7\xb89\x00AB\xc3D\xc5\xc6GH\xc9\xcaK\xccMN\xcfP\xd1\xd2S\xd4UV\xd7\xd8YZ",
	ret => 1,
},

