function dump(o)
   if type(o) == 'table' then
      local s = '{ '
      for k,v in pairs(o) do
         -- we create a local var because in Lua55
	 -- variables k and v get an implicit 'const' modifier
	 -- this works in previous Lua versions too
         local key_str = k
         if type(key_str) ~= 'number' then
            key_str = '"'..key_str..'"'
         end
         s = s .. '['..key_str..'] = ' .. dump(v) .. ','
      end
      return s .. '} '
   else
      return tostring(o)
   end
end

function main()
    ret = nil
    m.log(9, "Here I am");
    z = m.getvars("QUERY_STRING");
    m.log(9, "Z: " .. dump(z))

    return ret
end
