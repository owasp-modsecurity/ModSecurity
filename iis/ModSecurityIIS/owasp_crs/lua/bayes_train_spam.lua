#!/usr/bin/lua
require("io");

function table.val_to_str ( v )
  if "string" == type( v ) then
    v = string.gsub( v, "\n", "\\n" )
    if string.match( string.gsub(v,"[^'\"]",""), '^"+$' ) then
      return "'" .. v .. "'"
    end
    return '"' .. string.gsub(v,'"', '\\"' ) .. '"'
  else
    return "table" == type( v ) and table.tostring( v ) or
      tostring( v )
  end
end

function table.key_to_str ( k )
  if "string" == type( k ) and string.match( k, "^[_%a][_%a%d]*$" ) then
    return k
  else
    return "[" .. table.val_to_str( k ) .. "]"
  end
end

function table.tostring( tbl )
  local result, done = {}, {}
  for k, v in ipairs( tbl ) do
    table.insert( result, table.val_to_str( v ) )
    done[ k ] = true
  end
  for k, v in pairs( tbl ) do
    if not done[ k ] then
      table.insert( result,
        table.key_to_str( k ) .. "=" .. table.val_to_str( v ) )
    end
  end
  return "{" .. table.concat( result, "," ) .. "}"
end

function main()
local mf = require "moonfilter"
-- define the classes to use
mf.classes("/var/log/httpd/spam", "/var/log/httpd/ham")
-- create ham+spam DB on disk -- this is only necessary the first time
-- mf.create()

local args = {};
args = m.getvars("MATCHED_VARS", {"none"});
  if (#args == "0") then
        m.log(4, "# of ARGS: " ..#args.. ".");
        return nil;
  end
     -- Place ARGS data into key/value pairs for inspection
    for k,v in pairs(args) do
        name = v["name"];
        value = v["value"];
        m.log(4, "Arg Name: " ..name.. " and Arg Value: " ..value.. ".");
        mf.text = value;
	local train_result = mf.train("/var/log/httpd/spam")
	train_result = table.tostring(train_result)
                m.log(4, "Train Results: " .. train_result .. ".")

   	m.setvar("tx.bayes_msg", "Completed Bayesian SPAM Training on Payload: " .. mf.text .. ".");
   	return("Completed Bayesian SPAM Training on Payload: " .. mf.text .. ".");
    end
return nil;
end
