#!/opt/local/bin/lua

local request_filename = m.getvar("REQUEST_FILENAME", "none")
local args = {};
args = m.getvars("ARGS_NAMES", "none")

function main ()
for line in io.lines("/usr/local/apache/conf/modsec_current/base_rules/vulnerabilities.txt") do
    if line:find(request_filename) then
	if string.find(line, "^%d+\,") then
		for k,v in pairs(args) do
		local arg_name = v["value"] .. "=";
			if string.find(line, arg_name) then
			m.setvar("resource.osvdb_check", "1")
			m.setvar("resource.osvdb_vulnerable", "1")
			m.setvar("tx.osvdb_msg", line)
			return(line) 
			end
		end
	end
    end
end
	m.setvar("resource.osvdb_check", "1")
	return nil
end
