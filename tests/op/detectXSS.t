{
   type => "op",
   name => "detectXSS",
   input => "",
   ret => 0
},
{
   type => "op",
   name => "detectXSS",
   input => "this is not an XSS",
   ret => 0
},
{
   type => "op",
   name => "detectXSS",
   input => "<a href=\"javascript:alert(1)\">)",
   ret => 1
}
