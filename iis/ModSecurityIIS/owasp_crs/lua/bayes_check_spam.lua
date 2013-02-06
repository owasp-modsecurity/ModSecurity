#!/usr/bin/env lua
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
-- use command line moonfilter.lua to initially create the DBs outside
-- of ModSecurity
--mf.create()

local anomaly_score = m.getvar("TX.ANOMALY_SCORE", "none");
anomaly_score = tonumber(anomaly_score);

if not (anomaly_score) then
local score = ""

local args = {};
args = m.getvars("ARGS", {"none"});
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
	local class_result = mf.classify()

	class_result = table.tostring(class_result)
		m.log(4, "Classify Results: " .. class_result .. ".")
	class = string.gsub(class_result, ".*class=\"(.*)\".*", "%1")
		if (class == "/var/log/httpd/spam") then
			score = string.gsub(class_result, ".*prob=(%d\.%d%d%d%d%d%d%d%d%d%d%d%d%d%d)\,.*", "%1")
			score = tonumber(score)
			if (score) then
			  if (score > 0.90) then
				m.log(4, "Classify Results: " .. class .. ".")
            			m.setvar("tx.bayes_score", class_result);
				m.setvar("tx.bayes_var_name", name);
				m.setvar("tx.bayes_var", value);
	    			return("Bayesian Analaysis Alert for " .. name .. " with payload: \"" .. value .. "\"")
			  end
			end
          	end
     end
end
return nil;
end

