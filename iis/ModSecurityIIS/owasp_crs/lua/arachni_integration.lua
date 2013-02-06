--
-- Include Arachni RPC client code
--
require "client"

--
-- Call main ModSecurity Lua function
--
function main()

--
-- Set the remote Arachni RPC host
--
arachni_host = '192.168.168.128'

--
-- Extract Request Data
--
host = m.getvar("REQUEST_HEADERS.host")
	m.log(4, "Arachni: Host: " .. host)
request_filename = m.getvar("REQUEST_FILENAME")
	m.log(4, "Arachni: Filename: " .. request_filename)
url_to_scan = "http://" .. host .. request_filename
	m.log(4, "Arachni: URL to scan is: " .. url_to_scan)
request_method = m.getvar("REQUEST_METHOD")
	m.log(4, "Arachni: Request Method is: " .. request_method)

--
-- Convert ModSecurity ARGS data into a local table called args
--
ARGS = {}
ARGS = m.getvars("ARGS")
args = {}

for k,v in pairs(ARGS) do
	name = v["name"];
	name = string.gsub(name, "ARGS:(.*)", "%1")
	value = v["value"];
	m.log(4, "Arachni: Arg Name: " ..name.. " and Value: " ..value.. ".");
	
	args[name] = value
end

local yaml_args = yaml.dump ( args )
m.log(4, "Arachni: Updated ARGS table is: " .. yaml_args)


--
-- Convert ModSecrity COOKIE data into a local table called cookies_table
--
COOKIES = {}
COOKIES = m.getvars("REQUEST_COOKIES")
cookies_table = {}

for k,v in pairs(COOKIES) do
        name = v["name"];
        name = string.gsub(name, "REQUEST_COOKIES:(.*)", "%1")
        value = v["value"];
        m.log(4, "Arachni: Cookie Name: " ..name.. " and Value: " ..value.. ".");

        cookies_table[name] = value
end

local yaml_cookies = yaml.dump ( cookies_table )
m.log(4, "Arachni: Updated Cookies table is: " .. yaml_cookies)

--
-- Initiate Arachni RPC Dispatchers
--
dispatcher = ArachniRPCClient:new( { host = arachni_host, port = 7331 } )
instance_info = dispatcher:call( 'dispatcher.dispatch' )

--
-- Check to see if we have previously initiated a scan for the resource
--
-- If we have not, then we will contact the Dispatcher and start a scan
--
local arachni_scan_initiated = m.getvar("RESOURCE.arachni_scan_initiated")
if arachni_scan_initiated == nil then

	--
	-- Set the host to match the remote Dispatcher
	--
	instance = ArachniRPCClient:new({
    		host  = arachni_host,
    		port  = instance_info.port,
    		token = instance_info.token
	})


  	opts = {
    		url = url_to_scan,
    		audit_links = true,
    		audit_forms = true,
    		audit_cookies = true,
		-- only audit the stuff passed to vector feed
    		link_count_limit = 0,
		cookies = cookies_table
  	}

	instance:call( 'modules.load', { 'xss', 'sqli', 'path_traversal' } )

	vectors = {}

	-- add a form var (for POST params)
	table.insert( vectors, {
    		type = 'form',
    		method = request_method,
    		action = url_to_scan,
    		inputs = args
	})

	local yaml_vectors = yaml.dump( vectors )
	m.log(4, "Arachni: Yaml output of vectors is: " .. yaml_vectors)
	
	plugins = {
    		vector_feed = {
        	vectors = vectors
    		}	
	}
	instance:call( 'plugins.load', plugins )
	instance:call( 'opts.set', opts )
	instance:call( 'framework.run' )

	--
	-- Save the Dispatcher port/token data to pull the report later
	--
	m.setvar("RESOURCE.arachni_scan_initiated", "1")
	m.setvar("RESOURCE.arachni_instance_info_port", instance_info.port)
	m.setvar("RESOURCE.arachni_instance_info_token", instance_info.token)
	return ("Arachni: Scan Initiated. Exiting")

else

	--
	-- If we have previously initiated a scan, we will now check for a report
	--
	m.log(4, "Arachni: Previous scan was initiated, checking scan status.")
	local instance_info_port = m.getvar("RESOURCE.arachni_instance_info_port")
	local instance_info_token = m.getvar("RESOURCE.arachni_instance_info_token")
	m.log(4, "Arachni: Port info: " .. instance_info_port .. " and Token info: " .. instance_info_token)

	instance = ArachniRPCClient:new({
                host  = arachni_host,
                port  = instance_info_port,
                token = instance_info_token
        })

	if instance:call( 'framework.busy?' ) then
		m.log(4, "Arachni: Scan still in progress, framework is busy. Exiting.")
		return ("Arachni scan still in progress, framework is busy. Exiting.")
	else
		m.log(4, "Arachni: Scan completed - calling for report.")
		local results = instance:call( 'framework.issues_as_hash' )
		yaml_results = yaml.dump( results )
		m.log(4, "Arachni: Yaml Results: " .. yaml_results)

		for k,v in pairs(results) do
  			name = v["name"];
  			value = v["value"];

			if ( v["mod_name"] == "XSS" ) then
				local XssVulnParams = m.getvar("RESOURCE.xss_vulnerable_params")
                        		if not (XssVulnParams) then
						m.log(4, "Arachni: Vulnerability Identified for Parameter: \"" .. v["var"] .. "\", Vulnerability Type: \"" .. v["mod_name"] .. "\"")
        					m.setvar("RESOURCE.xss_vulnerable_params", v["var"])
					else
						local CheckArgInXssVulnParams = string.find(XssVulnParams, v["var"])
                                		if (CheckArgInXssVulnParams) then
                                        		m.log(4, "Arachni: Arg Name: " .. v["var"] .. " already in XSS Vuln list.")
                                		else
				        		m.log(4, "Arachni: Vulnerability Identified for Parameter: \"" .. v["var"] .. "\", Vulnerability Type: \"" .. v["mod_name"] .. "\"")
				        		XssVulnParams = XssVulnParams .. ", " .. v["var"]
                                        		m.setvar("RESOURCE.xss_vulnerable_params", XssVulnParams)
						end
					end
			
			end

			if ( v["mod_name"] == "SQLInjection" ) then
                		local SQLiVulnParams = m.getvar("RESOURCE.sqli_vulnerable_params")
                        		if not (SQLiVulnParams) then
                                		m.log(4, "Arachni: Vulnerability Identified for Parameter: \"" .. v["var"] .. "\", Vulnerability Type: \"" .. v["mod_name"] .. "\"")
                                		m.setvar("RESOURCE.sqli_vulnerable_params", v["var"])
                        		else
                                		local CheckArgInSQLiVulnParams = string.find(SQLiVulnParams, v["var"])
                                		if (CheckArgInSQLiVulnParams) then
                                        		m.log(4, "Arachni: Arg Name: " .. v["var"] .. " already in SQLi Vuln list.")
                                		else
                                        		m.log(4, "Arachni: Vulnerability Identified for Parameter: \"" .. v["var"] .. "\", Vulnerability Type: \"" .. v["mod_name"] .. "\"")
                                        		SQLiVulnParams = SQLiVulnParams .. ", " .. v["var"]
                                        		m.setvar("RESOURCE.sqli_vulnerable_params", SQLiVulnParams)
                                		end
                        		end
        		end

 		end
		
		 instance:call( 'service.shutdown' )
                 m.setvar("RESOURCE.arachni_scan_completed", "1")
                 return ("Arachni: Done")
	end

end
end
