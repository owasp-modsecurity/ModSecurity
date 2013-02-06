function main()

--[[ Enforce Request Method ]]
EnforceRequestMethod()

--[[ Enforce Number of Parameters/ARGS ]]
EnforceNumOfArgs()

--[[ Enforce Parameter Names ]]
EnforceArgsNames()

--[[ Enforce Parameter Lengths ]]
EnforceArgsLength()

--[[ Enforce Parameter Character Class ]]
EnforceArgCharClass()

m.log(4, "Ending Profile Enforcer Script")
return nil
end

--[[ Begin Enforcement Functions ]]

function EnforceArgCharClass()
	local Args = {}
        Args = m.getvars("ARGS", {"none"})
	local EnforceArgCharClassEmail = m.getvar("RESOURCE.enforce_charclass_email")
	local EnforceArgCharClassDigits = m.getvar("RESOURCE.enforce_charclass_digits")
	local EnforceArgCharClassUrl = m.getvar("RESOURCE.enforce_charclass_url")
	local EnforceArgCharClassPath = m.getvar("RESOURCE.enforce_charclass_path")	
	local EnforceArgCharClassFlag = m.getvar("RESOURCE.enforce_charclass_flag")
	local EnforceArgCharClassAlpha = m.getvar("RESOURCE.enforce_charclass_alphas")
	local EnforceArgCharClassAlphaNumeric = m.getvar("RESOURCE.enforce_charclass_alphanumeric")
	local EnforceArgCharClassSafeText = m.getvar("RESOURCE.enforce_charclass_safetext")

	for k,v in pairs(Args) do
  		name = v["name"];
  		value = v["value"];
  		m.log(4, "CharClass Check - Arg Name: " ..name.. " and Value: " ..value.. ".");

		--[[ Check for Digits Character Class ]]
		if (EnforceArgCharClassDigits) then
			local CheckArgCharClassDigits = string.find(EnforceArgCharClassDigits, name)
   			if (CheckArgCharClassDigits) then
        			m.log(4, "Arg Name: " .. name .. " in Digits Enforcement list.")
				if string.match(value, "^%d+$") then
                        		m.log(4, "Parameter " ..name.. " payload matches digit class.")
				else
					m.log(4, "Parameter " ..name.. " payload does not match digit class.")
					m.setvar("TX." ..name.. "_digits_violation", value)
					m.setvar("TX.digits_violation_name", name)
				end
			end
		end

		--[[ Check for Email Character Class ]]
		if (EnforceArgCharClassEmail) then
                        local CheckArgCharClassEmail = string.find(EnforceArgCharClassEmail, name)
                        if (CheckArgCharClassEmail) then
                                m.log(4, "Arg Name: " .. name .. " in Email Enforcement list.")
                                if string.match(value, "^[A-Za-z0-9%.%%%+%-]+@[A-Za-z0-9%.%%%+%-]+%.%w%w%w?%w?$") then
                                        m.log(4, "Parameter " ..name.. " payload matches email class.")
                                else
                                        m.log(4, "Parameter " ..name.. " payload does not match email class.")
                                        m.setvar("TX." ..name.. "_email_violation", value)
                                        m.setvar("TX.email_violation_name", name)
                                end
                        end

 		end

		--[[ Check for URL Class ]]
		if (EnforceArgCharClassUrl) then
                        local CheckArgCharClassUrl = string.find(EnforceArgCharClassUrl, name)
                        if (CheckArgCharClassUrl) then
                                m.log(4, "Arg Name: " .. name .. " in Url Enforcement list.")
                                if string.match(value, "[A-Za-z]+://[A-Za-z0-9-_]+%.[A-Za-z0-9-_.]+/?") then
                                        m.log(4, "Parameter " ..name.. " payload matches url class.")
                                else
                                        m.log(4, "Parameter " ..name.. " payload does not match url class.")
                                        m.setvar("TX." ..name.. "_url_violation", value)
                                        m.setvar("TX.url_violation_name", name)
                                end
                        end

		end

		--[[ Check for Path Class ]]
		if (EnforceArgCharClassPath) then
                        local CheckArgCharClassPath = string.find(EnforceArgCharClassPath, name)
                        if (CheckArgCharClassPath) then
                                m.log(4, "Arg Name: " .. name .. " in Path Enforcement list.")
                                if string.match(value, "[-a-zA-Z0-9/._]*/[-a-zA-Z0-9/._]*") then
                                        m.log(4, "Parameter " ..name.. " payload matches path class.")
                                else
                                        m.log(4, "Parameter " ..name.. " payload does not match path class.")
                                        m.setvar("TX." ..name.. "_path_violation", value)
                                        m.setvar("TX.path_violation_name", name)
                                end
                        end

		end

		--[[ Check for Flag Parameter Class ]]
		if (EnforceArgCharClassFlag) then
                        local CheckArgCharClassFlag = string.find(EnforceArgCharClassFlag, name)
                        if (CheckArgCharClassFlag) then
                                m.log(4, "Arg Name: " .. name .. " in Flag Enforcement list.")
                                if string.match(value, "^$") then
                                        m.log(4, "Parameter " ..name.. " payload matches flag class.")
                                else
                                        m.log(4, "Parameter " ..name.. " payload does not match flag class.")
                                        m.setvar("TX." ..name.. "_flag_violation", value)
                                        m.setvar("TX.flag_violation_name", name)
                                end
                        end
		end

		--[[ Check for Alpha/Letters Character Class ]]
		if (EnforceArgCharClassAlpha) then
                        local CheckArgCharClassAlpha = string.find(EnforceArgCharClassAlpha, name)
                        if (CheckArgCharClassAlpha) then
                                m.log(4, "Arg Name: " .. name .. " in Alpha Enforcement list.")
                                if string.match(value, "^%a+$") then
                                        m.log(4, "Parameter " ..name.. " payload matches alpha class.")
                                else
                                        m.log(4, "Parameter " ..name.. " payload does not match alpha class.")
                                        m.setvar("TX." ..name.. "_alpha_violation", value)
                                        m.setvar("TX.alpha_violation_name", name)
                                end
                        end
		end

		--[[ Check for AlphaNumeric Character Class ]]
		if (EnforceArgCharClassAlphaNumeric) then
                        local CheckArgCharClassAlphaNumeric = string.find(EnforceArgCharClassAlphaNumeric, name)
                        if (CheckArgCharClassAlphaNumeric) then
                                m.log(4, "Arg Name: " .. name .. " in AlphaNumeric Enforcement list.")
                                if string.match(value, "^%w+$") then
                                        m.log(4, "Parameter " ..name.. " payload matches alphanumeric class.")
                                else
                                        m.log(4, "Parameter " ..name.. " payload does not match alphanumeric class.")
                                        m.setvar("TX." ..name.. "_alphanumeric_violation", value)
                                        m.setvar("TX.alphanumeric_violation_name", name)
                                end
                        end
		end

		--[[ Check for SafeText Character Class ]]
		if (EnforceArgCharClassSafeText) then
                        local CheckArgCharClassSafeText = string.find(EnforceArgCharClassSafeText, name)
                        if (CheckArgCharClassSafeText) then
                                m.log(4, "Arg Name: " .. name .. " in SafeText Enforcement list.")
                                if string.match(value, "^[a-zA-Z0-9%s_%.%-]+$") then
                                        m.log(4, "Parameter " ..name.. " payload matches safetext class.")
                                else
                                        m.log(4, "Parameter " ..name.. " payload does not match safetext class.")
                                        m.setvar("TX." ..name.. "_safetext_violation", value)
                                        m.setvar("TX.safetext_violation_name", name)
                                end
                        end
		end

	end
end

function EnforceArgsLength()
 local ArgsLength = {}
 ArgsLength = m.getvars("ARGS", {"none", "length"})

 for k,v in pairs(ArgsLength) do
  name = v["name"];
  value = v["value"];
  value = tonumber(value);
  m.log(4, "Arg Name: " ..name.. " and Length: " ..value.. ".");

  local MinArgLength = tonumber(m.getvar("RESOURCE." .. name .. "_length_min", {"none"}))
  local MaxArgLength = tonumber(m.getvar("RESOURCE." .. name .. "_length_max", {"none"}))
 
   if ((value > MinArgLength) and (value < MaxArgLength)) then
	m.log(4, "Arg Name: " .. name .. " with Length: :" ..value.. " is within normal range.")
   elseif value < MinArgLength then
	m.log(4, "Arg Name: " .. name .. " Length " ..value.. " is below the normal range.")
	m.setvar("TX." .. name .. "_min_length_violation", value)
	m.setvar("TX.MinArgLength", MinArgLength)
	m.setvar("TX.MinArgLengthName", name)
   elseif value > MaxArgLength then
        m.log(4, "Arg Name: " .. name .. " Length " ..value.. " is above the normal range.")
        m.setvar("TX." .. name .. "_max_length_violation", value)
        m.setvar("TX.MaxArgLength", MaxArgLength)
	m.setvar("TX.MaxArgLengthName", name)
   end
 end
end

function EnforceArgsNames()
	local ArgsNames = {}
        ArgsNames = m.getvars("ARGS_NAMES", {"none"})
	local EnforceArgsNames = m.getvar("RESOURCE.enforce_args_names")

 for k,v in pairs(ArgsNames) do
  name = v["name"];
  value = v["value"];
  m.log(4, "ArgsName: " ..value.. ".");

  local CheckArgsNames = string.find(EnforceArgsNames, value)
   if (CheckArgsNames) then
	m.log(4, "Arg Name: " .. value .. " is valid.")
   else
	m.log(4, "Args Name: " .. value .. " is not valid.")
	m.setvar("TX.args_names_violation", name)
   end
 end
end

function EnforceRequestMethod()
	local RequestMethod = m.getvar("REQUEST_METHOD", {"none"})

	local EnforceRequestMethods = m.getvar("RESOURCE.enforce_request_methods")
	local EnforceMethods = string.find(EnforceRequestMethods, RequestMethod)
        	if (EnforceMethods) then
			m.log(4, "Request Method " .. RequestMethod .. " already in Enforcement List.")
		else
			m.log(4, "Request Method: " .. RequestMethod .. " profile violation.")
                	m.setvar("TX.request_method_violation", "1")
	
		end

end

function EnforceNumOfArgs()
	local ARGS = {}
	local ARGS = m.getvars("ARGS", {"none"})
        local NumOfArgs = tonumber(#ARGS)
	local MinNumOfArgs = tonumber(m.getvar("RESOURCE.MinNumOfArgs", {"none"}))
	local MaxNumOfArgs = tonumber(m.getvar("RESOURCE.MaxNumOfArgs", {"none"}))
      	local EnforceNumOfArgs = m.getvar("RESOURCE.enforce_num_of_args")

	if ((NumOfArgs > MinNumOfArgs) and (NumOfArgs < MaxNumOfArgs)) then
		m.log(4, "Number of ARGS is within normal range.")
	elseif NumOfArgs < MinNumOfArgs then
               	m.log(4, "Number of ARGS is less than MinNumOfArgs: " .. MinNumOfArgs .. ".")
		m.setvar("TX.MIN_NUM_ARGS_VIOLATION", "1")
		m.setvar("TX.NUM_OF_ARGS", NumOfArgs)
	elseif NumOfArgs > MaxNumOfArgs then
		m.log(4, "Number of ARGS is more than MxxiaxinNumOfArgs: " .. MaxNumOfArgs .. ".")
                m.setvar("TX.MAX_NUM_ARGS_VIOLATION", "1")
		m.setvar("TX.NUM_OF_ARGS", NumOfArgs)
       	end
end

