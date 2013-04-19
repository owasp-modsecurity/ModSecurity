{
   type => "op",
   name => "detectSQLi",
   input => "",
   ret => 0
},
{
   type => "op",
   name => "detectSQLi",
   input => "this is not isqli",
   ret => 0
},
{
   type => "op",
   name => "detectSQLi",
   input => "ascii(substring(version() from 1 for 1))",
   ret => 1
}
