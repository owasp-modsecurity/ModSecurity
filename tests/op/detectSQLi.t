{
   type => "op",
   name => "issqli",
   input => "",
   ret => 0
},
{
   type => "op",
   name => "issqli",
   input => "this is not isqli",
   ret => 0
},
{
   type => "op",
   name => "issqli",
   input => "ascii(substring(version() from 1 for 1))",
   ret => 1
}
