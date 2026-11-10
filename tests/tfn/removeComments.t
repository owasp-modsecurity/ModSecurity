## 1
{
      "output" => "",
      "input" => "",
      "type" => "tfn",
      "name" => "removeComments",
      "ret" => 0
},
## 2
{
      "input" => "TestCase",
      "type" => "tfn",
      "name" => "removeComments",
      "ret" => 0,
      "output" => "TestCase"
},
## 3
{
      "ret" => 0,
      "input" => "Test\\u0000Case",
      "name" => "removeComments",
      "type" => "tfn",
      "output" => "Test\\u0000Case"
},
## 4
{
      "ret" => 1,
      "input" => "/* TestCase */",
      "name" => "removeComments",
      "type" => "tfn",
      "output" => ""
},
## 5
{
      "output" => "",
      "ret" => 1,
      "type" => "tfn",
      "input" => "/*TestCase*/",
      "name" => "removeComments"
},
## 6
{
      "input" => "/* TestCase*/",
      "type" => "tfn",
      "name" => "removeComments",
      "ret" => 1,
      "output" => ""
},
## 7
{
      "output" => "",
      "ret" => 1,
      "name" => "removeComments",
      "input" => "/*TestCase */",
      "type" => "tfn"
},
## 8
{
      "output" => "BeforeAfter",
      "ret" => 1,
      "type" => "tfn",
      "input" => "Before/* TestCase */After",
      "name" => "removeComments"
},
## 9
{
      "output" => "Before TestCase */ After",
      "name" => "removeComments",
      "input" => "Before TestCase */ After",
      "type" => "tfn",
      "ret" => 0
},
## 10
{
      "type" => "tfn",
      "input" => "/* Test\\nCase */",
      "name" => "removeComments",
      "ret" => 1,
      "output" => ""
},
## 11
{
      "name" => "removeComments",
      "input" => "/* Test\r\nCase */",
      "type" => "tfn",
      "ret" => 1,
      "output" => ""
},
## 12
{
      "ret" => 1,
      "input" => "/* Test\nCase */",
      "type" => "tfn",
      "name" => "removeComments",
      "output" => ""
},
## 13
{
      "ret" => 1,
      "name" => "removeComments",
      "input" => "/* Test\rCase */",
      "type" => "tfn",
      "output" => ""
},
## 14
{
      "ret" => 1,
      "input" => "/*Before/* Test\r\nCase ",
      "type" => "tfn",
      "name" => "removeComments",
      "output" => " "
},
## 15
{
      "output" => "Before  ",
      "input" => "Before /* Test\nCase ",
      "type" => "tfn",
      "name" => "removeComments",
      "ret" => 1
},
## 16
{
      "output" => "Before   \r\nCase ",
      "ret" => 1,
      "input" => "Before/* T*/ /* e */ /* s */ /* t */\r\nCase ",
      "name" => "removeComments",
      "type" => "tfn"
},
## 17
{
      "input" => "Before /* */ ops */ Test\nCase ",
      "name" => "removeComments",
      "type" => "tfn",
      "ret" => 1,
      "output" => "Before  ops */ Test\nCase "
},
## 18
{
      "ret" => 1,
      "name" => "removeComments",
      "input" => "/*Test\r\nCase */After",
      "type" => "tfn",
      "output" => "After"
},
## 19
{
      "output" => "Test\nCase  After",
      "name" => "removeComments",
      "input" => "Test\nCase /**/ After",
      "type" => "tfn",
      "ret" => 1
},
## 20
{
      "output" => "Test\r\nCase */After",
      "ret" => 0,
      "name" => "removeComments",
      "input" => "Test\r\nCase */After",
      "type" => "tfn"
},
## 21
{
      "input" => "Test/*\nCase */ After",
      "name" => "removeComments",
      "type" => "tfn",
      "ret" => 1,
      "output" => "Test After"
},
## 22
{
     "input" => "SELECT/**//**/UNION",
     "name" => "removeComments",
     "type" => "tfn",
     "ret" => 1,
     "output" => "SELECTUNION"
},
## 23
{
     "input" => "foo<!-- comment 1 --><!-- comment 2 -->bar",
     "name" => "removeComments",
     "type" => "tfn",
     "ret" => 1,
     "output" => "foobar"
}
