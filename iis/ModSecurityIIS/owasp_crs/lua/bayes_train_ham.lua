#!/usr/bin/lua
require("io");

function main()
local mf = require "moonfilter"
-- define the classes to use
mf.classes("/var/log/httpd/spam", "/var/log/httpd/ham")
-- create ham+spam DB on disk -- this is only necessary the first time
-- mf.create()

local score = ""
local value = "" 

local args = {};
args = m.getvars("ARGS", {"none"});
  if (#args == 0) then
        m.log(4, "# of ARGS: " ..#args.. ".");
        return nil;
  end
     -- Place ARGS data into key/value pairs for inspection
    for k,v in pairs(args) do
	name = v["name"];
	value = v["value"];
	m.log(4, "Arg Name: " ..name.. " and Arg Value: " ..value.. ".");
	mf.text = value;
	local train_result = mf.train("/var/log/httpd/ham")

    end
-- return nil;
 m.log(4, "Low Bayesian Score: " .. score .. ". Training payloads as non-malicious.")
 m.setvar("tx.bayes_msg", "Training payload as ham: " .. value .. ".");
 return ("Training payloads as non-malicious: " .. value .. ".");

end
