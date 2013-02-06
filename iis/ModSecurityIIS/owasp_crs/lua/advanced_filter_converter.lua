#!/opt/local/bin/lua
local rex = require "rex_pcre"
local B = require "bit"

function main()

function dec2hex(nValue)
    if type(nValue) == "string" then
        nValue = String.ToNumber(nValue);
    end
    nHexVal = string.format("%X", nValue);
    sHexVal = nHexVal.."";
    return sHexVal;
end

function hex2dec (arg)
    local dec = {}
    for str in string.gfind(arg, "%w%w") do
        local str = '0X'..str
        table.insert(dec, tonumber(str))
    end

    return unpack(dec)
end

function explode ( seperator, str )
    local pos, arr = 0, {}
    for st, sp in function() return string.find( str, seperator, pos, true ) end
    do
        table.insert( arr, string.sub( str, pos, st-1 ) );
        pos = sp + 1;
    end
    table.insert( arr, string.sub( str, pos ) );
    return arr
end


function oct2dec(octstr)
    local i, len, num;

    num = 0;
    i = 0;
    octstr = string.reverse(octstr);
    len = string.len(octstr);

    if (len > 11) then
        return 1;
    end

    for str in string.gfind(octstr, "%w") do
        number = tonumber(str);
        if((number < 0) or (number > 7)) then
            num = 0;
            return 0;
        end
 
        i = tonumber(i);
        num_shr = B.lshift(number ,(i*3));
        num = B.bor(num,num_shr);
        i = i + 1;
    end
    return num;
end


function str_split_unique(data)
  a = {}
  b = {}
  -- use table to eliminate duplicates
  for i=1,string.len(data) do
    v = string.sub(data,i,i)
    a[v] = v
  end
  -- insert into ordered array and sort
  for k,v in pairs(a) do
    table.insert(b,k)
  end
  table.sort(b)
  return b
end

function str_split(data)
 a = {}
 for i=1,string.len(data) do
   a[i] = string.sub(data,i,i)
 end
 return a
end

-- character table string
local b='ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'

-- base64 decoding
function base64decode(data)
    data = string.gsub(data, '[^'..b..'=]', '')
    return (data:gsub('.', function(x)
        if (x == '=') then return '' end
        local r,f='',(b:find(x)-1)
        for i=6,1,-1 do r=r..(f%2^i-f%2^(i-1)>0 and '1' or '0') end
        return r;
    end):gsub('%d%d%d?%d?%d?%d?%d?%d?', function(x)
        if (#x ~= 8) then return '' end
        local c=0
        for i=1,8 do c=c+(x:sub(i,i)=='1' and 2^(8-i) or 0) end
        return string.char(c)
    end))
end

function urldecode(s)
  return (string.gsub (string.gsub (s, "+", " "), 
          "%%(%x%x)", 
         function (str)
          return string.char (tonumber (str, 16))
         end ))
end 

function urlencode(s)
 return (string.gsub (s, "%W", 
        function (str)
          return string.format ("%%%02X", string.byte (str))
        end  ))
end

function strip_tags(h) 
	local newstr = rex.gsub(h, "<(\/?)(\\w+)[^\>]*>", "%2", nil, 0, 0);
	return newstr
end

function hexdecode(s)
        s = string.gsub(s, "%%(%x%x)", function (h)
            return string.char(tonumber(h, 16))
          end)
      return s
end

function sql_hexdecode(s)
        s = string.gsub(s, "(%x%x)", function (h)
            return string.char(tonumber(h, 16))
          end)
      return s
end


--[[ Retrieve all ARGS parameters from ModSec

	urlDecodeUni, htmlEntityDecode and jsDecode can be used here with the initial
	extraction of data since they are able to decode any inline value vs.
	other transformation functions which will attempt to decode the entire
	string value.  For those situations, we must create our own Lua functions
]]

local args = {};
args = m.getvars("ARGS", {"none"});

-- Only run checks if ARGS are present
if (#args == "0") then
	m.log(4, "# of ARGS: " ..#args.. ".");
	return nil;
end


-- Place ARGS data into key/value pairs for inspection 
for k,v in pairs(args) do
  name = v["name"];
  value = v["value"];
  original_value = value;
  m.log(4, "Arg Name: " ..name.. " and Arg Value: " ..value.. ".");

--[[ Start Converter code ]]

--[[ Make sure the value to normalize and monitor doesn't contain 
	possibilities for a regex DoS.]]
	-- remove obvious repetition patterns
	value = rex.gsub(value, "(?:(.{2,})\\1{32,})|(?:[\-+=|@\\s]{128,})", "x", nil, 0, 0);
               	m.log(4, "Remove repetition patterns: " .. value .. "");

--[[ Check for comments and erases them if available ]]
	-- check for existing comments
       	if rex.match(value, "(?ms:(?:\\<!\-|\-\->|\\/\\*|\\*\\/|\\/\\/\\W*\\w+\\s*$)|(?:\-\-[^\\-]*\-))", 1) then
					converted = rex.gsub(value, "(?ms:(?:(?:<!)(?:(?:\-\-(?:[^\\-]*(?:\-[^\\-]+)*)\-\-\\s*)*)(?:>))|(?:(?:\\/\\*\\/*[^\\/\\*]*)+\\*\\/)|(?:\-\-[^\\-]*\-))", ";", nil, 0, 0);
					value = (value .. "\n" .. converted);
					m.log(4, "Check for Existing Comments: " .. value .. "");
       	end 

        -- make sure inline comments are detected and converted correctly
        value = rex.gsub(value, "(?m:(<\\w+)\\/+(\\w+=?))", "%1/%2", nil, 0, 0);
					m.log(4, "Remove Inline Comments1: " .. value .. "");
        value = rex.gsub(value, "(?m:[^\\\\:]\\/\\/(.*)$)", "/**/%1", nil, 0, 0);
					m.log(4, "Remove Inline Comments2: " .. value .. "");

--[[ Strip newlines ]] 
	-- check for inline linebreaks
	value = rex.gsub(value, "\\\\(r|n|f|t|v)", ";", nil, 0, 0);
		m.log(4, "Check for inline linebreaks: " .. value .. "");
	-- replace replacement characters regular spaces
	value = string.gsub(value, "�", ' ', nil, 0, 0);
		m.log(4, "Replace replacement chars: " .. value .. "");
	-- convert real linebreaks
	value = rex.gsub(value, "(?m:[\\r\\n\\f\\t\\v])", " ", nil, 0, 0);
		m.log(4, "Convert real linebreaks: " .. value .. "");

--[[ Checks for common charcode pattern and decodes them ]]
function convertFromJSCharcode(value)

    local matches, matches2, matches3;
    local changed = 0;
    local sum = 0;
    local chr = 0;
    local converted = "";
		local tmp_value = value;

        -- check if value matches typical charCode pattern

       for line in rex.gmatch(tmp_value, "(?ms:(?:[\\d+-=\/\* ]+(?:\\s?,\\s?[\\d+-=\/\* ]+)){4,})", 0, 0)
       do
            if(matches ~= nil) then
                matches = matches .. "," .. line;
            else
                matches = line;
            end
       end

       if(matches ~= nil) then

            matches = rex.gsub(matches,"(\\s)", "");
            matches = rex.gsub(matches,"(\\w+=)", "");

            str = explode(",",matches);

            for i=1, table.getn(str) do

            chr = str[i];

                if(string.len(str[i]) > 0) then

                chr = rex.gsub(chr,"(?s:\\W0)", "");

                    if(chr ~= nil) then

                        for line2 in rex.gmatch(chr, "(\\d*[+-\/\* ]\\d+)",0, 0)
                        do
                            if(matches2 ~= nil) then
                                matches2 = matches2 .. "" .. line2;
                            else
                                matches2 = line2;
                            end
                        end

                        if( matches2 ~= nil )then
                            for line3 in rex.split(matches2, "((\\W?\\d+))",0, 0)
                            do
                                if(line ~= nil) then
                                    changed = 1;
                                    sum = sum + tonumber(line3);
                                end

                                if(matches3 ~= nil) then
                                    matches3 = matches3 .. line3;
                                else
                                    matches3 = line3;
                                end
                            end
                        end

                        if(changed == 1) then
                            if(sum >= 20) then
                                if(sum <= 127) then
                                    converted = converted .. string.char(sum);
                                end
                            end
                        end

                        if(changed == 0) then
                            local num = 0;
                            if(string.len(chr) > 0) then
                                num = tonumber(chr);
                            end
                            converted = converted .. string.char(num);
                        end
                    end

            value = tmp_value .. "\n" .. converted;
            end
         end
        end
end

function convertFromJSCharcode_hex(value)
    -- check for hexadecimal charcode pattern
        local matches_hex = "";
        local converted = "";
				local tmp_value = value;

       for line in rex.gmatch(tmp_value, "(?ims:(?:(?:[\\\\]+\\w+\\s*){8,}))", 0, 0)
       do
            if(matches_hex ~= nil) then
                matches_hex = matches_hex .. "," .. line;
            else
                matches_hex = line;
            end
       end

       if(matches_hex ~= nil) then

            matches_hex = rex.gsub(matches_hex,"([ux])", "");

            converted = "";

            str = explode(",",matches_hex);

            for i=1, table.getn(str) do

            chr = str[i];

                if(tonumber(chr) ~= 0) then
                        converted = converted .. string.char(hex2dec(chr));
                end
            end

            value = tmp_value .. "\n" .. converted;
       end

        print(value);
        return value;
end

function convertFromJSCharcode_oct(value)

        local matches_oct = "";
        local converted_oct = "";
				local tmp_value = value;

    -- check for octal charcode pattern

       for line in rex.gmatch(tmp_value, "(?ims:(?:(?:[\\\\]+\\d+){8,}))", 0, 0)
       do
            if(matches_oct ~= nil) then
                matches_oct = matches_oct .. "," .. line;
            else
                matches_oct = line;
            end
       end

       if(matches_oct ~= nil) then

            matches_oct = rex.gsub(matches_oct,"(\\s)", "");

            str = explode(",",matches_oct);

            print(str);

            for i=1, table.getn(str) do

                chr = str[i];

                if (tonumber(str[i]) ~= 0) then

                        n = oct2dec(chr);

                        n = dec2hex(n);

                        if(n ~= 0)then
                            str2 = string.char(hex2dec(n));

                            if(converted_oct ~= nil) then
                                converted_oct = converted_oct .. str2;
                            else
                                converted_oct = str2;
                            end
                        end
                end
            end

            if(converted_oct ~= nil) then
                value = tmp_value .. "\n" .. converted_oct;
            else
                value = tmp_value;
            end
       end

       print(value);

       return value;
end

convertFromJSCharcode(value);
 m.log(4, "convertFromJSCharcode: " .. value .. "");
convertFromJSCharcode_hex(value);
m.log(4, "convertFromJSCharcode_hex: " .. value .. "");
convertFromJSCharcode_oct(value);
m.log(4, "convertFromJSCharcode_oct: " .. value .. "");

--[[ Eliminate JS regex modifiers ]]
        value = rex.gsub(value, "\/[gim]+", "\/", nil, 0, 0);
		m.log(4, "Eliminate JS regex modifiers: " .. value .. ".");

--[[ Converts from hex/dec entities ]]

	-- deal with double encoded payload
function htmlEntityDecode(value)
			   value = rex.gsub(value, "&amp;", "&", nil, 0, 0);
				   local result;
				   local tmp_value = value;

						  for line in rex.gmatch(tmp_value, "(?ms:&#x?([\\w]{2}\\d?);?)", 0, 0)
							 do
							       if(line ~= nil) then
	 					            if(result ~= nil) then
											     result = result .. line;
											  else
											     result = line;
											  end
										 end
							 end


						 if(result ~= nil) then
						    result = sql_hexdecode(result);
						    value = tmp_value .. "\n" .. result;
								result = rex.gsub(result, ";;", ";", nil, 0, 0);
					   else
					      value = tmp_value;
						 end


					 print(result);

        	 return result;

end

htmlEntityDecode(value);
	m.log(4, "Converts from hex/dex entities: " .. value .. ".");

				-- normalize obfuscated protocol handlers
	value = rex.gsub(value, "(?ms:(?:j\\s*a\\s*v\\s*a\\s*s\\s*c\\s*r\\s*i\\s*p\\s*t\\s*)|(d\\s*a\\s*t\\s*a\\s*))", "javascript", nil, 0, 0);

--[[ Normalize Quotes ]] 
	-- normalize different quotes to "
        value = rex.gsub(value, "[\'\`\´\’\‘]", "\"", nil, 0, 0);
                m.log(4, "Normalize Quotes: " .. value .. ".");

        -- make sure harmless quoted strings don't generate false alerts
        value = rex.gsub(value, "^\"([^\"=\\!><~]+)\"$", "%1", nil, 0, 0);
                m.log(4, "Harmless Quotes: " .. value .. ".");


--[[ Converts SQLHEX to plain text ]]
	local tmp_value = value;
	while true do
		sql_hex_value = rex.match(tmp_value, "(?im:0x([a-fA-F\\d]{2,}[a-fA-F\\d]*)+)");
		if (sql_hex_value == nil) then break end
			m.log(4, "SQL Hex Data: " .. sql_hex_value .. ".");
		local sql_hex_decoded = sql_hexdecode(sql_hex_value);
			m.log(4, "SQL Hex Data Decoded: " .. sql_hex_decoded .. ".");
		tmp_value = rex.gsub(tmp_value, "(?im:0x([a-fA-F\\d]{2,}[a-fA-F\\d]*)+)", sql_hex_decoded, 1, 0, 0);
			m.log(4, "SQL Hex Data Normalized: " .. tmp_value .. ".");
	end
	value = rex.gsub(tmp_value, "(?m:0x\\d+)", "1", nil, 0, 0);

--[[ Converts basic SQL keywords and obfuscations ]]
	value = rex.gsub(value, "(?ims:(?:IS\\s+null)|(LIKE\\s+null)|(?:(?:^|\\W)IN[\+\\s]*\([\\s\\d\"]+[^\(\)]*\)))", "\"=0", nil, 0, 0);
        value = rex.gsub(value, "(?ims:\\W+\\s*like\\s*\\W+)", "1\" OR \"1\"", nil, 0, 0);
        value = rex.gsub(value, "(?ims:null[,\"\\s])", ",0", nil, 0, 0);
        value = rex.gsub(value, "(?ims:\\d+\\.)", " 1", nil, 0, 0);
        value = rex.gsub(value, "(?ims:,null)", ",0", nil, 0, 0);
        value = rex.gsub(value, "(?ims:between|mod)", "or", nil, 0, 0);
        value = rex.gsub(value, "(?ims:and\\s+\\d+\.?\\d*)", "", nil, 0, 0);
        value = rex.gsub(value, "(?ims:\\s+and\\s+)", " or ", nil, 0, 0);
        value = rex.gsub(value, "(?ims:[^\\w,\(]NULL|\\\\N|TRUE|FALSE|UTC_TIME|LOCALTIME(?:STAMP)?|CURRENT_\\w+|BINARY|(?:(?:ASCII|SOUNDEX|FIND_IN_SET|MD5|R?LIKE)[\+\\s]*\\([^\(\)]+\\))|(?:\-+\\d))", "0", nil, 0, 0);
        value = rex.gsub(value, "(?ims:(?:NOT\\s+BETWEEN)|(?:IS\\s+NOT)|(?:NOT\\s+IN)|(?:XOR|\\WDIV\\W|\\WNOT\\W|<>|RLIKE(?:\\s+BINARY)?)|(?:REGEXP\\s+BINARY)|(?:SOUNDS\\s+LIKE))", "!", nil, 0, 0);
        value = rex.gsub(value, "\"\\s+\\d", "\"", nil, 0, 0);
        value = rex.gsub(value, "\\/(?i:\\d+|null)", "", nil, 0, 0);

	m.log(4, "Convert SQL Keywords and Obfuscations: " .. value .. ".");

--[[ Detects nullbytes and controls chars via ord() ]]
        -- critical ctrl values
        value = rex.gsub(value, "(?i:cha?r\\((0|1|2|3|4|5|6|7|8|11|12|14|15|16|17|18|19|24|25|192|193|238|255)\\))", "%%00", nil, 0, 0);
		m.log(4, "Convert nullbytes and control chars via ord(): " .. value .. ".");
        
	-- take care for malicious unicode characters
	value = urldecode(rex.gsub(urlencode(value), "(?i:(?:\%E(?:2|3)\%8(?:0|1)\%(?:A|8|9)\\w|\%EF\%BB\%BF|\%EF\%BF\%BD)|(?:&#(?:65|8)\\d{3};?))", "", nil, 0, 0));

	value = urldecode(rex.gsub(urlencode(value), "(?i:(?:\%F0\%80\%BE))", ">", nil, 0, 0));
	value = urldecode(rex.gsub(urlencode(value), "(?i:(?:\%F0\%80\%BC))", "<", nil, 0, 0));
	value = urldecode(rex.gsub(urlencode(value), "(?i:(?:\%F0\%80\%A2))", "\"", nil, 0, 0));
	value = urldecode(rex.gsub(urlencode(value), "(?i:(?:\%F0\%80\%A7))", "\'", nil, 0, 0));
	value = urldecode(rex.gsub(urlencode(value), "(?i:(?:\%ff1c))", "<", nil, 0, 0));
	
	value = rex.gsub(value, "(?i:(?:&[#x]*(200|820|200|820|zwn?j|lrm|rlm)\\w?;?))", "", nil, 0, 0);
	value = rex.gsub(value, "(?i:(?:&#(?:65|8)\\d{3};?)|(?:&#(?:56|7)3\\d{2};?)|(?:&#x(?:fe|20)\\w{2};?)|(?:&#x(?:d[c-f])\\w{2};?))", "", nil, 0, 0);
	value = rex.gsub(value, "(«|〈|＜|‹|〈|⟨)", "<", nil, 0, 0); 
	value = rex.gsub(value, "(»|〉|＞|›|〉|⟩)", ">", nil, 0, 0);
		m.log(4, "Malicious unicode characters: " .. value .. "");
	

--[[ This method matches and translates base64 strings and fragments
	used in data URIs ]]

	tmp_value = value;
	while true do
        	base64_value = rex.match(tmp_value, "([a-zA-Z0-9\+\/]{32,}={0,2})", 1, 0, 0);
		if (base64_value == nil) then break end
			m.log(4, "Base64 Data is: " .. base64_value .. ".");
		base64_value_decoded = base64decode(base64_value);
			m.log(4, "Base64 Data Decoded is: " .. base64_value_decoded .. ".");
		tmp_value = rex.gsub(tmp_value, "([a-zA-Z0-9\+\/]{32,}={0,2})", base64_value_decoded, 1, 0, 0);
			m.log(4, "Base64 Data Normalized: " .. tmp_value .. ".");
	end
	value = tmp_value;


--[[ Detects nullbytes and controls chars via ord() ]]
	local mytable = {};
	mytable = str_split(value);
	j = 1
        while mytable[j] do
          if (string.byte(mytable[j]) >= 127) then
		mytable[j] = rex.gsub(mytable[j], ".*", " ", nil, 0, 0);
	  end
          j = j + 1
        end
	
        value = table.concat(mytable);
                m.log(4, "Detect nullbytes and control chars via ord(): " .. value .. ".");


--[[ Strip XML patterns ]]
	converted = strip_tags(value);
		if (converted ~= value) then
			value = (value .. "\n" .. converted); 
			m.log(4, "Strip XML patterns: " .. value .. ".");
		end

--[[ This method converts JS unicode code points to regular characters ]]

function convertFromJSUnicode(args)
   local new_value = "";

   for line in rex.gmatch(args, "(?ims:\\\\u[0-9a-f]{4})", 0, 0)
   do
       hex = print(string.sub(line,3,6))
       chr = string.char(hex2dec(string.sub(line,5,7)));
       if ( new_value == nil ) then
           new_value = chr;
       else
           new_value = new_value .. chr;
       end
   end

   if ( string.len(new_value) > 0 ) then
        value = new_value .. "\n\\u0001";
   end
end

	convertFromJSUnicode(value);
	m.log(4, "Convert JS unicode code points to regular chars: " .. value .. "");


--[[ Converts relevant UTF-7 tags to UTF-8 ]]
   value = rex.gsub(value,"\\+ACI\\-","\"");
   value = rex.gsub(value,"\\+ADw\\-","<");
   value = rex.gsub(value,"\\+AD4\\-",">");
   value = rex.gsub(value,"\\+AFs\\-","%[");
   value = rex.gsub(value,"\\+AF0\\-","]");
   value = rex.gsub(value,"\\+AHs\\-","{");
   value = rex.gsub(value,"\\+AH0\\-","}");
   value = rex.gsub(value,"\\+AFw\\-","\\");
   value = rex.gsub(value,"\\+ADs\\-",";");
   value = rex.gsub(value,"\\+ACM\\-","#");
   value = rex.gsub(value,"\\+ACY\\-","&");
   value = rex.gsub(value,"\\+ACU\\-","%%");
   value = rex.gsub(value,"\\+ACQ\\-","$");
   value = rex.gsub(value,"\\+AD0\\-","=");
   value = rex.gsub(value,"\\+AGA\\-","`");
   value = rex.gsub(value,"\\+ALQ\\-","\"");
   value = rex.gsub(value,"\\+IBg\\-","\"");
   value = rex.gsub(value,"\\+IBk\\-","\"");
   value = rex.gsub(value,"\\+AHw\\-","|");
   value = rex.gsub(value,"\\+ACo\\-","*");
   value = rex.gsub(value,"\\+AF4\\-","%^");
   value = rex.gsub(value,"\\+ACIAPg\\-","\">");
   value = rex.gsub(value,"\\+ACIAPgA8\\-","\">");
	m.log(4, "Convert relevant UTF-7 tags to UTF-8: " .. value .. "");


--[[ Converts basic concatenations ]]
function stripslashes(args)

   local value = rex.gsub(args,"(\\\\(.?))","");

   return value;
end

function convertFromConcatenated(value)

       --normalize remaining backslashes
       if (value ~= rex.gsub(value,"((\\w)\\\\)", "%1")) then
           value = value .. rex.gsub(value,"((\\w)\\\\)", "%1");
       end

       local compare = stripslashes(value);

       pattern = { "(?s:(?:<\/\\w+>\+<\\w+>))",
           "(?s:(?:\":\\d+[^\"\[]+\"))",
           "(?s:(?:\"?\"\+\\w+\+\"))",
           "(?s:(?:\"\\s*;[^\"]+\")|(?:\";[^\"]+:\\s*\"))",
           "(?s:(?:\"\\s*(?:\;|\\+).{8,18}:\\s*\"))",
           "(?s:(?:\";\\w+=)|(?:!\"\"&&\")|(?:~))",
           "(?s:(?:\"?\"\\+\"\"?\\+?\"?)|(?:;\\w+=\")|(?:\"[|&]{2,}))",
           "(?s:(?:\"\\s*\\W+\"))",
           "(?s:(?:\";\\w\\s*\\+=\\s*\\w?\\s*\"))",
           "(?s:(?:\"[|&;]+\\s*[^\|\&\\n]*[\|\&]+\\s*\"?))", 
           "(?s:(?:\";\\s*\\w+\\W+\\w*\\s*[\|\&]*\"))",
           "(?s:(?:\"\\s*\"\\s*\.))",
           "((?:\\s*new\\s+\\w+\\s*[\\+\\\"\,]))",
           "((?:(?:^|\\s+)(?:do|else)\\s+))",
           "((?:(?:^|\\s+)(?:do|else)\\s+))",
           "((?:[{(]\\s*new\\s+\\w+\\s*[\)\}]))",
           "((?:(this|self)\.))",
           "((?:undefined))",
           "((?:in\\s+))" };

       for i=1, table.getn(pattern) do
           -- strip out concatenations
           converted = rex.gsub(compare,pattern[i],"");
       end

       -- strip object traversal
       converted = rex.gsub(converted,"(\\w(\.\\w\()))", "%1");

       -- normalize obfuscated method calls
       converted = rex.gsub(converted,"(\\)\\s*\+)", ")");

       --convert JS special numbers
       converted = rex.gsub(converted,"(?ims:(?:\\(*[.\\d]e[\+\-]*[^a-z\\W]+\\)*)|(?:NaN|Infinity)\\W)", "1");

       if (converted ~= nil) then
           if (compare ~= converted) then
           value = value .. "\n" .. converted;
           end
       end

       -- return value;
end

	convertFromConcatenated(value);
	m.log(4, "Convert basic concatenations: " .. value .. "");


--[[ This method collects and decodes proprietary encoding types ]]

function convertFromProprietaryEncodings(args)

       local value = args;

       --Xajax error reportings
       value = rex.gsub(value,"(?im:<!\[CDATA\[(.*)\]\]>)","%1", nil, 0, 0);

       --strip false alert triggering apostrophes
       value = rex.gsub(value,"(?m:(\\w)\"(s))", "%1%2");

       --strip quotes within typical search patterns
       value = rex.gsub(value,"(^\"([^\"=\\!><~]+)\"/$)", "%1");

       --OpenID login tokens
       value = rex.gsub(value,"({[\\w-]{8,9}\}(?:\{[\w=]{8}\}){2})", "");

       --convert Content and \sdo\s to null
       value = rex.gsub(value,"(?s:Content|\\Wdo)", "");

       --strip emoticons
       value = rex.gsub(value,
           "(?m:(?:\\s[:;]-[)\/PD]+)|(?:\\s;[)PD]+)|(?:\\s:[)PD]+)|-\.-|\^\^)",
           "");

       --normalize separation char repetion
       value = rex.gsub(value,"(?m:([.+~=*_;\-])\1{2,})", "%1");

       --normalize multiple single quotes
       value = rex.gsub(value,"(?m:/\"{2,})", "\"");
 
       --normalize quoted numerical values and asterisks
       value = rex.gsub(value,"(?m:\"(\\d+)\")", "%1");

       --normalize pipe separated request parameters
       value = rex.gsub(value,"(?m:\|(\\w+=\\w+))", "&%1");

       --normalize ampersand listings
       value = rex.gsub(value,"((\\w\\s)&\\s(\\w))", "%1%2");

       --normalize escaped RegExp modifiers
       value = rex.gsub(value,"(\/\\\\(\\w))", "/%1");

end

	convertFromProprietaryEncodings(value);
	m.log(4, "convertFromProprietaryEncodings: " .. value .. "");





	normalized_name = rex.gsub(name, "^(.*)$", "tx.%1_normalized");
        m.setvar(normalized_name, value);


--[[ This method is the centrifuge prototype ]]
	m.log(4, "Starting Centrifuge.. Arg Name = " ..name.. " and Arg Value = " ..value.. ".");

	threshold = 3.49;

	-- Examine each value
	if string.len(value) > 25 then
	local name = name;	
	-- strip padding
	tmp_value = rex.gsub(value, "\\s{4}|==$", "", nil, 0, 0);
		m.log(4, "Strip Padding1 - name is: " .. name .. " and value is: " .. tmp_value ..".");
	tmp_value = rex.gsub(tmp_value, "\\s{4}|[\\p{L}\\d\+\-\=\,\.\%\(\)]{8,}", "aaa", nil, 0, 0);
		m.log(4, "Strip Padding2 - name is: " .. name .. " and value is: " .. tmp_value ..".");
	
	-- Check for the attack char ratio
	tmp_value = rex.gsub(tmp_value, "([\*\.\!\?\+\-])\\1{1,}", "%1", nil, 0, 0);
	tmp_value = rex.gsub(tmp_value, "\"[\\p{L}\\d\\s]+\"", "", nil, 0, 0);

	stripped_length = string.len(rex.gsub(tmp_value, "[\\d\\s\\p{L}\.\:\,\%\&\/\>\<\\-)\!\|]+", "", nil, 0, 0));
		m.log(4, "stripped_length is: " .. stripped_length .. ".");
	overall_value = rex.gsub(tmp_value, "([\\d\\s\\p{L}\:\,\.]{3,})+", "aaa", nil, 0, 0);
		m.log(4, "overall_value is: " .. overall_value .. ".");
	overall_length = string.len(rex.gsub(overall_value, "\\s{2,}", "", nil, 0, 0));
		m.log(4, "overall_length is: " .. overall_length .. ".");

		if ((stripped_length ~= 0) and (overall_length/stripped_length <= threshold)) then 
                        ratio_value = (overall_length/stripped_length);
			ratio_name = rex.gsub(name, "^(.*)$", "tx.%1_centrifuge_ratio");
			m.setvar(ratio_name, ratio_value);
                        m.log(4, "Threshold is: " .. threshold .. " and Ratio Value is: " .. ratio_value .. ".");                        
                end
	end

	-- Examine each value
        if string.len(value) > 40 then

	converted = value;

	mytable = str_split_unique(converted)

	j = 1
	while mytable[j] do
	  print(mytable[j])
	  j = j + 1
	end

        converted = table.concat(mytable);
		m.log(4, "Unique/Sorted: " .. converted .. ".");	

	-- Replace all non-special chars
        converted =  rex.gsub(converted, "[\\w\\s\\p{L},\.:!]", "");
                m.log(4, "Replace non-special chars: " .. converted .. ".");


	-- Normalize certain tokens
	converted =  rex.gsub(converted, "(\\~|\\^|\\||\\*|\\%|\\&|\\/)", "+");
		m.log(4, "Normalize certain tokens: " .. converted .. ".");
	converted = rex.gsub(converted, "(\\+|\\-)\\s*\\d+", "+");
		m.log(4, "Normalize certain tokens: " .. converted .. ".");
	converted = rex.gsub(converted, "(\\(|\\)|\\[|\\]|\\{|\\})", "(");
                m.log(4, "Normalize certain tokens: " .. converted .. ".");
	converted = rex.gsub(converted, "(\\!|\\?|\\:|\=)", ":");
		m.log(4, "Normalize certain tokens: " .. converted .. ".");
	converted = rex.gsub(converted, "[^:(+]", "");
		m.log(4, "Normalize certain tokens: " .. converted .. ".");
	converted = string.gsub(converted, "\\", "");
                m.log(4, "Normalize certain tokens: " .. converted .. ".");

	mytable = str_split(converted)
	table.sort(mytable);	
        converted = table.concat(mytable);
                m.log(4, "Sorted: " .. converted .. ".");
	stripped_name = rex.gsub(name, "^(.*)$", "tx.%1_centrifuge_converted");
	m.setvar(stripped_name, converted);

	end


end

	if value ~= "." then

                return ("Normalized Payload: " .. name .. " = " .. value .. "");
        else
        -- Nothing wrong found.
        return nil;
        end


end
