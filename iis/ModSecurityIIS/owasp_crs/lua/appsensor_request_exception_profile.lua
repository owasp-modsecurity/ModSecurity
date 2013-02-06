function main()

--[[ Global Vars ]]
--[[ Import Profile Learning Thresholds
 
 [resource.min_traffic_threshold] 
 Set the resource.min_traffic_threshold as the minimum number of "clean" transactions
 to profile/inspect before enforcement of the profile begins.

 [resource.min_pattern_threshold]
 resource.min_pattern_threshold is the minimum number of times that an individual match should occur
 in order to include the it into the learned profile

]]

MinPatternThreshold = tonumber(m.getvar("RESOURCE.min_pattern_threshold"))
MinTrafficThreshold = tonumber(m.getvar("RESOURCE.min_traffic_threshold")) 
TrafficCounter = m.getvar("RESOURCE.traffic_counter")
	if TrafficCounter == nil then
		TrafficCounter = "1"
		m.setvar("RESOURCE.traffic_counter", TrafficCounter)
        	m.log(4, "Traffic Counter: " ..TrafficCounter.. ".") 
	else
		TrafficCounter = tonumber(TrafficCounter + 1)
		m.setvar("RESOURCE.traffic_counter", TrafficCounter)
		m.log(4, "Traffic Counter: " ..TrafficCounter.. ".")
	end

	--[[ Profile Request Method ]]
	ProfileRequestMethod()

	--[[ Profile Number of Parameters/ARGS ]]
	ProfileNumOfArgs()

	--[[ Profile Parameter Names ]]
	ProfileArgsNames()

	--[[ Profile Parameter Lengths ]]
	ProfileArgsLength()

	--[[ Profile Parameter Character Class ]]
	ProfileArgCharClass()

	if (TrafficCounter == MinTrafficThreshold) then
		m.setvar("RESOURCE.enforce_re_profile", "1")
	end

m.log(4, "Ending Profile Analyzer Script")
return nil
end

--[[ Begin Profiler Functions ]]

function ProfileArgCharClass()
	local Args = {}
        Args = m.getvars("ARGS", {"none"})

	for k,v in pairs(Args) do
  		name = v["name"];
  		value = v["value"];
  		m.log(4, "CharClass Check - Arg Name: " ..name.. " and Value: " ..value.. ".");

		--[[ Check for Digits Character Class ]]
		if string.match(value, "^%d+$") then
			m.log(4, "Parameter " ..name.. " payload matches digit class.")
			local EnforceArgCharClassDigits = m.getvar("RESOURCE.enforce_charclass_digits")
			if not (EnforceArgCharClassDigits) then
				local ArgDigitCounter = m.getvar("RESOURCE." ..name.. "_digit_counter")
				if not (ArgDigitCounter) then
                                	ArgDigitCounter = 1
                                	m.log(4, "Creating " .. name .. " Digit Counter: " .. ArgDigitCounter)
                                	m.setvar("RESOURCE." .. name .. "_digit_counter", ArgDigitCounter)
                        	else
                                	ArgDigitCounter = ArgDigitCounter + 1
                                	m.log(4, "Updating " .. name .. " Digit Counter: " .. ArgDigitCounter)
                                	m.setvar("RESOURCE." .. name .. "_digit_counter", ArgDigitCounter)
                        	end
				
				if (ArgDigitCounter == MinPatternThreshold) then
                			if not (EnforceArgCharClassDigits) then
                        			EnforceArgCharClassDigits = name
                			else
                        			EnforceArgCharClassDigits = EnforceArgCharClassDigits .. ", " .. name
                			end

                			m.log(4, "Arg Name: " .. name .. " Reached Pattern Threshold. Adding it to the Digits Enforcement list: " .. EnforceArgCharClassDigits)
                			m.setvar("RESOURCE.enforce_charclass_digits", EnforceArgCharClassDigits)
					m.setvar("!RESOURCE." .. name .. "_digit_counter", "0")
        			end
			else
				local CheckArgCharClassDigits = string.find(EnforceArgCharClassDigits, name)
   				if (CheckArgCharClassDigits) then
        				m.log(4, "Arg Name: " .. name .. " already in Digits Enforcement list.")
   				else
					local ArgDigitCounter = m.getvar("RESOURCE." ..name.. "_digit_counter")
                                	if not (ArgDigitCounter) then
                                        	ArgDigitCounter = 1
                                        	m.log(4, "Creating " .. name .. " Digit Counter: " .. ArgDigitCounter)
                                        	m.setvar("RESOURCE." .. name .. "_digit_counter", ArgDigitCounter)
                                	else
                                        	ArgDigitCounter = ArgDigitCounter + 1
                                        	m.log(4, "Updating " .. name .. " Digit Counter: " .. ArgDigitCounter)
                                        	m.setvar("RESOURCE." .. name .. "_digit_counter", ArgDigitCounter)
                                	end

                                	if (ArgDigitCounter == MinPatternThreshold) then
                                        	if not (EnforceArgCharClassDigits) then
                                                	EnforceArgCharClassDigits = name
                                        	else                                                
							EnforceArgCharClassDigits = EnforceArgCharClassDigits .. ", " .. name
                                        	end

                                        	m.log(4, "Arg Name: " .. name .. " Reached Pattern Threshold. Adding it to the Digits Enforcement list: " .. EnforceArgCharClassDigits)
                                        	m.setvar("RESOURCE.enforce_charclass_digits", EnforceArgCharClassDigits)
                                	end
				end
			end
                        if (TrafficCounter == MinTrafficThreshold) then
                                m.setvar("!RESOURCE." .. name .. "_digit_counter", "0")
                        end

	

		--[[ Check for Email Class ]]
                elseif string.match(value, "^[A-Za-z0-9%.%%%+%-]+@[A-Za-z0-9%.%%%+%-]+%.%w%w%w?%w?$") then
                        m.log(4, "Parameter " ..name.. " payload matches email class.")
			local EnforceArgCharClassEmail = m.getvar("RESOURCE.enforce_charclass_email")
			if not (EnforceArgCharClassEmail) then
				local ArgEmailCounter = m.getvar("RESOURCE." ..name.. "_email_counter")
				if not (ArgEmailCounter) then
                                	ArgEmailCounter = 1
                                	m.log(4, "Creating " .. name .. " Email Counter: " .. ArgEmailCounter)
                                	m.setvar("RESOURCE." .. name .. "_email_counter", ArgEmailCounter)
                        	else
                                	ArgEmailCounter = ArgEmailCounter + 1
                                	m.log(4, "Updating " .. name .. " Email Counter: " .. ArgEmailCounter)
                                	m.setvar("RESOURCE." .. name .. "_email_counter", ArgEmailCounter)
                        	end
				
				if (ArgEmailCounter == MinPatternThreshold) then
                			if not (EnforceArgCharClassEmail) then
                        			EnforceArgCharClassEmail = name
                			else
                        			EnforceArgCharClassEmail = EnforceArgCharClassEmail .. ", " .. name
                			end

                			m.log(4, "Arg Name: " .. name .. " Reached Pattern Threshold. Adding it to the Email Enforcement list: " .. EnforceArgCharClassEmail)
                			m.setvar("RESOURCE.enforce_charclass_email", EnforceArgCharClassEmail)
        			end
			else
				local CheckArgCharClassEmail = string.find(EnforceArgCharClassEmail, name)
   				if (CheckArgCharClassEmail) then
        				m.log(4, "Arg Name: " .. name .. " already in Email Enforcement list.")
   				else
					local ArgEmailCounter = m.getvar("RESOURCE." ..name.. "_email_counter")
                                	if not (ArgEmailCounter) then
                                        	ArgEmailCounter = 1
                                        	m.log(4, "Creating " .. name .. " Email Counter: " .. ArgEmailCounter)
                                        	m.setvar("RESOURCE." .. name .. "_email_counter", ArgEmailCounter)
                                	else
                                        	ArgEmailCounter = ArgEmailCounter + 1
                                        	m.log(4, "Updating " .. name .. " Email Counter: " .. ArgEmailCounter)
                                        	m.setvar("RESOURCE." .. name .. "_email_counter", ArgEmailCounter)
                                	end

                                	if (ArgEmailCounter == MinPatternThreshold) then
                                        	if not (EnforceArgCharClassEmail) then
                                                	EnforceArgCharClassEmail = name
                                        	else                                                
							EnforceArgCharClassEmail = EnforceArgCharClassEmail .. ", " .. name
                                        	end

                                        	m.log(4, "Arg Name: " .. name .. " Reached Pattern Threshold. Adding it to the Email Enforcement list: " .. EnforceArgCharClassEmail)
                                        	m.setvar("RESOURCE.enforce_charclass_email", EnforceArgCharClassEmail)
                                	end
				end
			end
			if (TrafficCounter == MinTrafficThreshold) then
                		m.setvar("!RESOURCE." .. name .. "_email_counter", "0")
        		end



		--[[ Check for URL Class ]]
                elseif string.match(value, "[A-Za-z]+://[A-Za-z0-9-_]+%.[A-Za-z0-9-_.]+/?") then
                        m.log(4, "Parameter " ..name.. " payload matches url class.")
			local EnforceArgCharClassUrl = m.getvar("RESOURCE.enforce_charclass_url")
			if not (EnforceArgCharClassUrl) then
				local ArgUrlCounter = m.getvar("RESOURCE." ..name.. "_url_counter")
				if not (ArgUrlCounter) then
                                	ArgUrlCounter = 1
                                	m.log(4, "Creating " .. name .. " Url Counter: " .. ArgUrlCounter)
                                	m.setvar("RESOURCE." .. name .. "_url_counter", ArgUrlCounter)
                        	else
                                	ArgUrlCounter = ArgUrlCounter + 1
                                	m.log(4, "Updating " .. name .. " Url Counter: " .. ArgUrlCounter)
                                	m.setvar("RESOURCE." .. name .. "_url_counter", ArgUrlCounter)
                        	end
				
				if (ArgUrlCounter == MinPatternThreshold) then
                			if not (EnforceArgCharClassUrl) then
                        			EnforceArgCharClassUrl = name
                			else
                        			EnforceArgCharClassUrl = EnforceArgCharClassUrl .. ", " .. name
                			end

                			m.log(4, "Arg Name: " .. name .. " Reached Pattern Threshold. Adding it to the Url Enforcement list: " .. EnforceArgCharClassUrl)
                			m.setvar("RESOURCE.enforce_charclass_url", EnforceArgCharClassUrl)
        			end
			else
				local CheckArgCharClassUrl = string.find(EnforceArgCharClassUrl, name)
   				if (CheckArgCharClassUrl) then
        				m.log(4, "Arg Name: " .. name .. " already in Url Enforcement list.")
   				else
					local ArgUrlCounter = m.getvar("RESOURCE." ..name.. "_url_counter")
                                	if not (ArgUrlCounter) then
                                        	ArgUrlCounter = 1
                                        	m.log(4, "Creating " .. name .. " Url Counter: " .. ArgUrlCounter)
                                        	m.setvar("RESOURCE." .. name .. "_url_counter", ArgUrlCounter)
                                	else
                                        	ArgUrlCounter = ArgUrlCounter + 1
                                        	m.log(4, "Updating " .. name .. " Url Counter: " .. ArgUrlCounter)
                                        	m.setvar("RESOURCE." .. name .. "_url_counter", ArgUrlCounter)
                                	end

                                	if (ArgUrlCounter == MinPatternThreshold) then
                                        	if not (EnforceArgCharClassUrl) then
                                                	EnforceArgCharClassUrl = name
                                        	else                                                
							EnforceArgCharClassUrl = EnforceArgCharClassUrl .. ", " .. name
                                        	end

                                        	m.log(4, "Arg Name: " .. name .. " Reached Pattern Threshold. Adding it to the Url Enforcement list: " .. EnforceArgCharClassUrl)
                                        	m.setvar("RESOURCE.enforce_charclass_url", EnforceArgCharClassUrl)
                                	end
				end
			end
			if (TrafficCounter == MinTrafficThreshold) then
                		m.setvar("!RESOURCE." .. name .. "_url_counter", "0")
        		end



		--[[ Check for Path Class ]]
                elseif string.match(value, "[-a-zA-Z0-9/._]*/[-a-zA-Z0-9/._]*") then
                        m.log(4, "Parameter " ..name.. " payload matches path class.")
			local EnforceArgCharClassPath = m.getvar("RESOURCE.enforce_charclass_path")
			if not (EnforceArgCharClassPath) then
				local ArgPathCounter = m.getvar("RESOURCE." ..name.. "_path_counter")
				if not (ArgPathCounter) then
                                	ArgPathCounter = 1
                                	m.log(4, "Creating " .. name .. " Path Counter: " .. ArgPathCounter)
                                	m.setvar("RESOURCE." .. name .. "_path_counter", ArgPathCounter)
                        	else
                                	ArgPathCounter = ArgPathCounter + 1
                                	m.log(4, "Updating " .. name .. " Path Counter: " .. ArgPathCounter)
                                	m.setvar("RESOURCE." .. name .. "_path_counter", ArgPathCounter)
                        	end
				
				if (ArgPathCounter == MinPatternThreshold) then
                			if not (EnforceArgCharClassPath) then
                        			EnforceArgCharClassPath = name
                			else
                        			EnforceArgCharClassPath = EnforceArgCharClassPath .. ", " .. name
                			end

                			m.log(4, "Arg Name: " .. name .. " Reached Pattern Threshold. Adding it to the Path Enforcement list: " .. EnforceArgCharClassPath)
                			m.setvar("RESOURCE.enforce_charclass_path", EnforceArgCharClassPath)
        			end
			else
				local CheckArgCharClassPath = string.find(EnforceArgCharClassPath, name)
   				if (CheckArgCharClassPath) then
        				m.log(4, "Arg Name: " .. name .. " already in Path Enforcement list.")
   				else
					local ArgPathCounter = m.getvar("RESOURCE." ..name.. "_path_counter")
                                	if not (ArgPathCounter) then
                                        	ArgPathCounter = 1
                                        	m.log(4, "Creating " .. name .. " Path Counter: " .. ArgPathCounter)
                                        	m.setvar("RESOURCE." .. name .. "_path_counter", ArgPathCounter)
                                	else
                                        	ArgPathCounter = ArgPathCounter + 1
                                        	m.log(4, "Updating " .. name .. " Path Counter: " .. ArgPathCounter)
                                        	m.setvar("RESOURCE." .. name .. "_path_counter", ArgPathCounter)
                                	end

                                	if (ArgPathCounter == MinPatternThreshold) then
                                        	if not (EnforceArgCharClassPath) then
                                                	EnforceArgCharClassPath = name
                                        	else                                                
							EnforceArgCharClassPath = EnforceArgCharClassPath .. ", " .. name
                                        	end

                                        	m.log(4, "Arg Name: " .. name .. " Reached Pattern Threshold. Adding it to the Path Enforcement list: " .. EnforceArgCharClassPath)
                                        	m.setvar("RESOURCE.enforce_charclass_path", EnforceArgCharClassPath)
                                	end
				end
			end
			if (TrafficCounter == MinTrafficThreshold) then
                		m.setvar("!RESOURCE." .. name .. "_path_counter", "0")
        		end



		--[[ Check for Flag Parameter Class ]]
                elseif string.match(value, "^$") then
                        m.log(4, "Parameter " ..name.. " payload matches flag parameter class.")
			local EnforceArgCharClassFlag = m.getvar("RESOURCE.enforce_charclass_flag")
			if not (EnforceArgCharClassFlag) then
				local ArgFlagCounter = m.getvar("RESOURCE." ..name.. "_flag_counter")
				if not (ArgFlagCounter) then
                                	ArgFlagCounter = 1
                                	m.log(4, "Creating " .. name .. " Flag Counter: " .. ArgFlagCounter)
                                	m.setvar("RESOURCE." .. name .. "_flag_counter", ArgFlagCounter)
                        	else
                                	ArgFlagCounter = ArgFlagCounter + 1
                                	m.log(4, "Updating " .. name .. " Flag Counter: " .. ArgFlagCounter)
                                	m.setvar("RESOURCE." .. name .. "_flag_counter", ArgFlagCounter)
                        	end
				
				if (ArgFlagCounter == MinPatternThreshold) then
                			if not (EnforceArgCharClassFlag) then
                        			EnforceArgCharClassFlag = name
                			else
                        			EnforceArgCharClassFlag = EnforceArgCharClassFlag .. ", " .. name
                			end

                			m.log(4, "Arg Name: " .. name .. " Reached Pattern Threshold. Adding it to the Flag Enforcement list: " .. EnforceArgCharClassFlag)
                			m.setvar("RESOURCE.enforce_charclass_flag", EnforceArgCharClassFlag)
        			end
			else
				local CheckArgCharClassFlag = string.find(EnforceArgCharClassFlag, name)
   				if (CheckArgCharClassFlag) then
        				m.log(4, "Arg Name: " .. name .. " already in Flag Enforcement list.")
   				else
					local ArgFlagCounter = m.getvar("RESOURCE." ..name.. "_flag_counter")
                                	if not (ArgFlagCounter) then
                                        	ArgFlagCounter = 1
                                        	m.log(4, "Creating " .. name .. " Flag Counter: " .. ArgFlagCounter)
                                        	m.setvar("RESOURCE." .. name .. "_flag_counter", ArgFlagCounter)
                                	else
                                        	ArgFlagCounter = ArgFlagCounter + 1
                                        	m.log(4, "Updating " .. name .. " Flag Counter: " .. ArgFlagCounter)
                                        	m.setvar("RESOURCE." .. name .. "_flag_counter", ArgFlagCounter)
                                	end

                                	if (ArgFlagCounter == MinPatternThreshold) then
                                        	if not (EnforceArgCharClassFlag) then
                                                	EnforceArgCharClassFlag = name
                                        	else                                                
							EnforceArgCharClassFlag = EnforceArgCharClassFlag .. ", " .. name
                                        	end

                                        	m.log(4, "Arg Name: " .. name .. " Reached Pattern Threshold. Adding it to the Flag Enforcement list: " .. EnforceArgCharClassFlag)
                                        	m.setvar("RESOURCE.enforce_charclass_flag", EnforceArgCharClassFlag)
                                	end
				end
			end
			if (TrafficCounter == MinTrafficThreshold) then
                		m.setvar("!RESOURCE." .. name .. "_flag_counter", "0")
        		end




		--[[ Check for Alpha/Letters Character Class ]]
		elseif string.match(value, "^%a+$") then
			m.log(4, "Parameter " ..name.. " payload matches alpha class.")
			local EnforceArgCharClassAlpha = m.getvar("RESOURCE.enforce_charclass_alphas")
			if not (EnforceArgCharClassAlpha) then
				local ArgAlphaCounter = m.getvar("RESOURCE." ..name.. "_alpha_counter")
				if not (ArgAlphaCounter) then
                                	ArgAlphaCounter = 1
                                	m.log(4, "Creating " .. name .. " Alpha Counter: " .. ArgAlphaCounter)
                                	m.setvar("RESOURCE." .. name .. "_alpha_counter", ArgAlphaCounter)
                        	else
                                	ArgAlphaCounter = ArgAlphaCounter + 1
                                	m.log(4, "Updating " .. name .. " Alpha Counter: " .. ArgAlphaCounter)
                                	m.setvar("RESOURCE." .. name .. "_alpha_counter", ArgAlphaCounter)
                        	end
				
				if (ArgAlphaCounter == MinPatternThreshold) then
                			if not (EnforceArgCharClassAlpha) then
                        			EnforceArgCharClassAlpha = name
                			else
                        			EnforceArgCharClassAlpha = EnforceArgCharClassAlpha .. ", " .. name
                			end

                			m.log(4, "Arg Name: " .. name .. " Reached Pattern Threshold. Adding it to the Alpha Enforcement list: " .. EnforceArgCharClassAlpha)
                			m.setvar("RESOURCE.enforce_charclass_alphas", EnforceArgCharClassAlpha)
					m.setvar("!RESOURCE." .. name .. "_alpha_counter", "0")
        			end
			else
				local CheckArgCharClassAlpha = string.find(EnforceArgCharClassAlpha, name)
   				if (CheckArgCharClassAlpha) then
        				m.log(4, "Arg Name: " .. name .. " already in Alpha Enforcement list.")
   				else
					local ArgAlphaCounter = m.getvar("RESOURCE." ..name.. "_alpha_counter")
                                	if not (ArgAlphaCounter) then
                                        	ArgAlphaCounter = 1
                                        	m.log(4, "Creating " .. name .. " Alpha Counter: " .. ArgAlphaCounter)
                                        	m.setvar("RESOURCE." .. name .. "_alpha_counter", ArgAlphaCounter)
                                	else
                                        	ArgAlphaCounter = ArgAlphaCounter + 1
                                        	m.log(4, "Updating " .. name .. " Alpha Counter: " .. ArgAlphaCounter)
                                        	m.setvar("RESOURCE." .. name .. "_alpha_counter", ArgAlphaCounter)
                                	end

                                	if (ArgAlphaCounter == MinPatternThreshold) then
                                        	if not (EnforceArgCharClassAlpha) then
                                                	EnforceArgCharClassAlpha = name
                                        	else                                                
							EnforceArgCharClassAlpha = EnforceArgCharClassAlpha .. ", " .. name
                                        	end

                                        	m.log(4, "Arg Name: " .. name .. " Reached Pattern Threshold. Adding it to the Alpha Enforcement list: " .. EnforceArgCharClassAlpha)
                                        	m.setvar("RESOURCE.enforce_charclass_alphas", EnforceArgCharClassAlpha)
                                	end
				end

			end
			if (TrafficCounter == MinTrafficThreshold) then
                                m.setvar("!RESOURCE." .. name .. "_alpha_counter", "0")
                        end



		--[[ Check for AlphaNumeric Character Class ]]
		elseif string.match(value, "^%w+$") then
			m.log(4, "Parameter " ..name.. " payload matches alphanumeric class.")
			local EnforceArgCharClassAlphaNumeric = m.getvar("RESOURCE.enforce_charclass_alphanumeric")
			if not (EnforceArgCharClassAlphaNumeric) then
				local ArgAlphaNumericCounter = m.getvar("RESOURCE." ..name.. "_alphanumeric_counter")
				if not (ArgAlphaNumericCounter) then
                                	ArgAlphaNumericCounter = 1
                                	m.log(4, "Creating " .. name .. " AlphaNumeric Counter: " .. ArgAlphaNumericCounter)
                                	m.setvar("RESOURCE." .. name .. "_alphanumeric_counter", ArgAlphaNumericCounter)
                        	else
                                	ArgAlphaNumericCounter = ArgAlphaNumericCounter + 1
                                	m.log(4, "Updating " .. name .. " AlphaNumeric Counter: " .. ArgAlphaNumericCounter)
                                	m.setvar("RESOURCE." .. name .. "_alphanumeric_counter", ArgAlphaNumericCounter)
                        	end
				
				if (ArgAlphaNumericCounter == MinPatternThreshold) then
                			if not (EnforceArgCharClassAlphaNumeric) then
                        			EnforceArgCharClassAlphaNumeric = name
                			else
                        			EnforceArgCharClassAlphaNumeric = EnforceArgCharClassAlphaNumeric .. ", " .. name
                			end

                			m.log(4, "Arg Name: " .. name .. " Reached Pattern Threshold. Adding it to the AlphaNumeric Enforcement list: " .. EnforceArgCharClassAlphaNumeric)
                			m.setvar("RESOURCE.enforce_charclass_alphanumeric", EnforceArgCharClassAlphaNumeric)
					m.setvar("!RESOURCE." .. name .. "_alphanumeric_counter", "0")
        			end
			else
				local CheckArgCharClassAlphaNumeric = string.find(EnforceArgCharClassAlphaNumeric, name)
   				if (CheckArgCharClassAlphaNumeric) then
        				m.log(4, "Arg Name: " .. name .. " already in AlphaNumeric Enforcement list.")
   				else
					local ArgAlphaNumericCounter = m.getvar("RESOURCE." ..name.. "_alphanumeric_counter")
                                	if not (ArgAlphaNumericCounter) then
                                        	ArgAlphaNumericCounter = 1
                                        	m.log(4, "Creating " .. name .. " AlphaNumeric Counter: " .. ArgAlphaNumericCounter)
                                        	m.setvar("RESOURCE." .. name .. "_alphanumeric_counter", ArgAlphaNumericCounter)
                                	else
                                        	ArgAlphaNumericCounter = ArgAlphaNumericCounter + 1
                                        	m.log(4, "Updating " .. name .. " AlphaNumeric Counter: " .. ArgAlphaNumericCounter)
                                        	m.setvar("RESOURCE." .. name .. "_alphanumeric_counter", ArgAlphaNumericCounter)
                                	end

                                	if (ArgAlphaNumericCounter == MinPatternThreshold) then
                                        	if not (EnforceArgCharClassAlphaNumeric) then
                                                	EnforceArgCharClassAlphaNumeric = name
                                        	else                                                
							EnforceArgCharClassAlphaNumeric = EnforceArgCharClassAlphaNumeric .. ", " .. name
                                        	end

                                        	m.log(4, "Arg Name: " .. name .. " Reached Pattern Threshold. Adding it to the AlphaNumeric Enforcement list: " .. EnforceArgCharClassAlphaNumeric)
                                        	m.setvar("RESOURCE.enforce_charclass_alphanumeric", EnforceArgCharClassAlphaNumeric)
                                	end
				end
			end
                        if (TrafficCounter == MinTrafficThreshold) then
                                m.setvar("!RESOURCE." .. name .. "_alphanumeric_counter", "0")
                        end


		--[[ Check for SafeText Character Class ]]
                elseif string.match(value, "^[a-zA-Z0-9%s_%.%-]+$") then
                        m.log(4, "Parameter " ..name.. " payload matches safetext class.")
			local EnforceArgCharClassSafeText = m.getvar("RESOURCE.enforce_charclass_safetext")
			if not (EnforceArgCharClassSafeText) then
				local ArgSafeTextCounter = m.getvar("RESOURCE." ..name.. "_safetext_counter")
				if not (ArgSafeTextCounter) then
                                	ArgSafeTextCounter = 1
                                	m.log(4, "Creating " .. name .. " SafeText Counter: " .. ArgSafeTextCounter)
                                	m.setvar("RESOURCE." .. name .. "_safetext_counter", ArgSafeTextCounter)
                        	else
                                	ArgSafeTextCounter = ArgSafeTextCounter + 1
                                	m.log(4, "Updating " .. name .. " SafeText Counter: " .. ArgSafeTextCounter)
                                	m.setvar("RESOURCE." .. name .. "_safetext_counter", ArgSafeTextCounter)
                        	end
				
				if (ArgSafeTextCounter == MinPatternThreshold) then
                			if not (EnforceArgCharClassSafeText) then
                        			EnforceArgCharClassSafeText = name
                			else
                        			EnforceArgCharClassSafeText = EnforceArgCharClassSafeText .. ", " .. name
                			end

                			m.log(4, "Arg Name: " .. name .. " Reached Pattern Threshold. Adding it to the SafeText Enforcement list: " .. EnforceArgCharClassSafeText)
                			m.setvar("RESOURCE.enforce_charclass_safetext", EnforceArgCharClassSafeText)
        			end
			else
				local CheckArgCharClassSafeText = string.find(EnforceArgCharClassSafeText, name)
   				if (CheckArgCharClassSafeText) then
        				m.log(4, "Arg Name: " .. name .. " already in SafeText Enforcement list.")
   				else
					local ArgSafeTextCounter = m.getvar("RESOURCE." ..name.. "_safetext_counter")
                                	if not (ArgSafeTextCounter) then
                                        	ArgSafeTextCounter = 1
                                        	m.log(4, "Creating " .. name .. " SafeText Counter: " .. ArgSafeTextCounter)
                                        	m.setvar("RESOURCE." .. name .. "_safetext_counter", ArgSafeTextCounter)
                                	else
                                        	ArgSafeTextCounter = ArgSafeTextCounter + 1
                                        	m.log(4, "Updating " .. name .. " SafeText Counter: " .. ArgSafeTextCounter)
                                        	m.setvar("RESOURCE." .. name .. "_safetext_counter", ArgSafeTextCounter)
                                	end

                                	if (ArgSafeTextCounter == MinPatternThreshold) then
                                        	if not (EnforceArgCharClassSafeText) then
                                                	EnforceArgCharClassSafeText = name
                                        	else                                                
							EnforceArgCharClassSafeText = EnforceArgCharClassSafeText .. ", " .. name
                                        	end

                                        	m.log(4, "Arg Name: " .. name .. " Reached Pattern Threshold. Adding it to the SafeText Enforcement list: " .. EnforceArgCharClassSafeText)
                                        	m.setvar("RESOURCE.enforce_charclass_safetext", EnforceArgCharClassSafeText)
                                	end
				end
			end
			if (TrafficCounter == MinTrafficThreshold) then
                		m.setvar("!RESOURCE." .. name .. "_safetext_counter", "0")
        		end



		end

	end
end

function ProfileArgsLength()
	local ArgsLength = {}
        ArgsLength = m.getvars("ARGS", {"none", "length"})

 for k,v in pairs(ArgsLength) do
  name = v["name"];
  value = v["value"];
  m.log(4, "Arg Name: " ..name.. " and Length: " ..value.. ".");

  local EnforceArgLength = m.getvar("RESOURCE.enforce_" ..name .. "_length")
  if EnforceArgsLength ~= nil then
  local CheckArgsLength = string.find(EnforceArgLength, value)
   if (CheckArgsLength) then
	m.log(4, "Arg Name: " .. name .. " with Length: :" ..value.. " already in Enforcement list.")
   else
        local ArgLengthCounter = m.getvar("RESOURCE." .. name .. "_length_" ..value.. "_counter")
        if not (ArgLengthCounter) then
                ArgLengthCounter = 1
                m.log(4, "Creating " .. name .. " Length " ..value.. " Counter: " .. ArgLengthCounter)
                m.setvar("RESOURCE." .. name .. "_length_" ..value.. "_counter", ArgLengthCounter)
        else
                ArgLengthCounter = ArgLengthCounter + 1
                m.log(4, "Increasing " .. name .. " Length " .. value .. " Counter: " .. ArgLengthCounter)
                m.setvar("RESOURCE." .. name .. "_length_" ..value.. "_counter", ArgLengthCounter)
        end


	if (ArgLengthCounter == MinPatternThreshold) then
                if not (EnforceArgLength) then
                        EnforceArgLength = value
                else
                        EnforceArgLength = EnforceArgLength .. ", " .. value
                end

                m.log(4, "Arg Name: " .. name .. " with Length: " .. value .. " Reached Pattern Threshold. Adding it to the Enforcement list: " .. EnforceArgLength)
                m.setvar("RESOURCE.enforce_" ..name .. "_length", EnforceArgLength)
        end
   end
  else
	local ArgLengthCounter = m.getvar("RESOURCE." .. name .. "_length_" ..value.. "_counter") 
        if not (ArgLengthCounter) then
                ArgLengthCounter = 1
		m.log(4, "Creating " .. name .. " Length " ..value.. " Counter: " .. ArgLengthCounter)
                m.setvar("RESOURCE." .. name .. "_length_" ..value.. "_counter", ArgLengthCounter)
        else
                ArgLengthCounter = ArgLengthCounter + 1
                m.log(4, "Increasing " .. name .. " Length " .. value .. " Counter: " .. ArgLengthCounter)
                m.setvar("RESOURCE." .. name .. "_length_" ..value.. "_counter", ArgLengthCounter)
        end

        if (ArgLengthCounter == MinPatternThreshold) then
                if not (EnforceArgLength) then
                        EnforceArgLength = value
                else
                        EnforceArgLength = EnforceArgLength .. ", " .. value
                end

                m.log(4, "Arg Name: " .. name .. " with Length: " .. value .. " Reached Pattern Threshold. Adding it to the Enforcement list: " .. EnforceArgLength)
                m.setvar("RESOURCE.enforce_" ..name.. "_length", EnforceArgLength)
        end
  end

	if (TrafficCounter == MinTrafficThreshold) then
                i=1
                length_of_arg={}
                for num in string.gmatch(EnforceArgLength, "%d+") do
                        length_of_arg[i]=num;i=i+1;
                end
                local MinArgLength = math.min(unpack(length_of_arg))
                m.setvar("RESOURCE." .. name .. "_length_min", MinArgLength)
                local MaxArgLength = math.max(unpack(length_of_arg))
                m.setvar("RESOURCE." .. name .. "_length_max", MaxArgLength)
                m.log(4, "Min Length of " .. name .. ": " ..MinArgLength.. " and Max Length: " ..MaxArgLength.. ".")
		m.setvar("!RESOURCE." .. name .. "_length_" ..value.. "_counter", "0")
        end

 end
end

function ProfileArgsNames()
	local ArgsNames = {}
        ArgsNames = m.getvars("ARGS_NAMES", {"none"})
	local EnforceArgsNames = m.getvar("RESOURCE.enforce_args_names")

 for k,v in pairs(ArgsNames) do
  name = v["name"];
  value = v["value"];
  m.log(4, "ArgsName: " ..value.. ".");

  if EnforceArgsNames ~= nil then
  local CheckArgsNames = string.find(EnforceArgsNames, value)
   if (CheckArgsNames) then
	m.log(4, "Arg Name: " .. value .. " already in Enforcement list.")
   else

        local ArgsNamesCounter = m.getvar("RESOURCE.args_names_counter_" .. value)
        if not (ArgsNamesCounter) then
                ArgsNamesCounter = 1
                m.log(4, "Creating " .. value .. " Pattern Score: " .. ArgsNamesCounter)
                m.setvar("RESOURCE.args_names_counter_" .. value, ArgsNamesCounter)
        else
                ArgsNamesCounter = ArgsNamesCounter + 1
                m.log(4, "Increasing " .. value .. " Pattern Score to: " .. ArgsNamesCounter)
                m.setvar("RESOURCE.args_names_counter_" .. value, ArgsNamesCounter)
        end


	if (ArgsNamesCounter == MinPatternThreshold) then
                if not (EnforceArgsNames) then
                        EnforceArgsNames = value
                else
                        EnforceArgsNames = EnforceArgsNames .. ", " .. value
                end

                m.log(4, "Args Names: " .. value .. " Reached Pattern Threshold. Adding it to the Enforcement list: " .. EnforceArgsNames)
                m.setvar("RESOURCE.enforce_args_names", EnforceArgsNames)
                m.setvar("!RESOURCE.args_names_counter_" .. value, "0")
        end
   end
  else
	local ArgsNamesCounter = m.getvar("RESOURCE.args_names_counter_" .. value)
        if not (ArgsNamesCounter) then
                ArgsNamesCounter = 1
                m.log(4, "Creating " .. value .. " Pattern Score: " .. ArgsNamesCounter)
                m.setvar("RESOURCE.args_names_counter_" .. value, ArgsNamesCounter)
        else
                ArgsNamesCounter = ArgsNamesCounter + 1
                m.log(4, "Increasing " .. value .. " Pattern Score to: " .. ArgsNamesCounter)
                m.setvar("RESOURCE.args_names_counter_" .. value, ArgsNamesCounter)
        end


        if (ArgsNamesCounter == MinPatternThreshold) then
                if not (EnforceArgsNames) then
                        EnforceArgsNames = value
                else
                        EnforceArgsNames = EnforceArgsNames .. ", " .. value
                end

                m.log(4, "Args Names: " .. value .. " Reached Pattern Threshold. Adding it to the Enforcement list: " .. EnforceArgsNames)
                m.setvar("RESOURCE.enforce_args_names", EnforceArgsNames)
                m.setvar("!RESOURCE.args_names_counter_" .. value, "0")
        end
  end
 end
end

function ProfileRequestMethod()
	local RequestMethod = m.getvar("REQUEST_METHOD", {"none"})

	local EnforceRequestMethods = m.getvar("RESOURCE.enforce_request_methods")
	if EnforceRequestMethods ~= nil then
	local CheckEnforceMethods = string.find(EnforceRequestMethods, RequestMethod)
        	if (CheckEnforceMethods) then
			m.log(4, "Request Method " .. RequestMethod .. " already in Enforcement List.")
		end
	end

	local RequestMethodCounter = m.getvar("RESOURCE.request_method_counter_" .. RequestMethod)
        if not (RequestMethodCounter) then
                RequestMethodCounter = 1
		m.log(4, "Creating " .. RequestMethod .. " Pattern Score: " .. RequestMethodCounter)
                m.setvar("RESOURCE.request_method_counter_" .. RequestMethod, RequestMethodCounter) 
        else
                RequestMethodCounter = RequestMethodCounter + 1
		m.log(4, "Increasing " .. RequestMethod .. " Pattern Score to: " .. RequestMethodCounter)
                m.setvar("RESOURCE.request_method_counter_" .. RequestMethod, RequestMethodCounter)
        end

	if (RequestMethodCounter == MinPatternThreshold) then
                if not (EnforceRequestMethods) then
                        EnforceRequestMethods = RequestMethod 
                else
                        EnforceRequestMethods = EnforceRequestMethods .. ", " .. RequestMethod 
                end

		m.log(4, "Request Method Reached Pattern Threshold. Adding it to the EnforceRequestMethods list: " .. EnforceRequestMethods)
		m.setvar("RESOURCE.enforce_request_methods", EnforceRequestMethods)
	end

	if (TrafficCounter == MinTrafficThreshold) then
		m.setvar("!RESOURCE.request_method_counter_" .. RequestMethod, "0")
	end

end

function ProfileNumOfArgs()
	local ARGS = {}
	local ARGS = m.getvars("ARGS", {"none"})
        local NumOfArgs = tonumber(#ARGS)

      	local EnforceNumOfArgs = m.getvar("RESOURCE.enforce_num_of_args")
        if EnforceNumOfArgs ~= nil then
		local CheckNumOfArgs = string.find(EnforceNumOfArgs, NumOfArgs)
        	if (CheckNumOfArgs) then
                	m.log(4, "ARGS #: " .. NumOfArgs .. " already in Enforcement List.")
        	end
	end

	local NumOfArgsCounter = m.getvar("RESOURCE.NumOfArgs_counter_" .. NumOfArgs)
        if not (NumOfArgsCounter) then
                NumOfArgsCounter = 1
		m.log(4, "Current # of ARGS: " ..NumOfArgs.. " has not been previously seen.")
                m.log(4, "Creating " .. NumOfArgs .. " Pattern Score to: " .. NumOfArgsCounter)
		m.setvar("RESOURCE.NumOfArgs_counter_" .. NumOfArgs, NumOfArgsCounter)
        else
                NumOfArgsCounter = NumOfArgsCounter + 1
		m.log(4, "Current # of ARGS: " ..NumOfArgs.. " has been previously seen.")
                m.log(4, "Increasing " .. NumOfArgs .. " Pattern Score to: " .. NumOfArgsCounter)
                m.setvar("RESOURCE.NumOfArgs_counter_" .. NumOfArgs, NumOfArgsCounter)
        end


        if (NumOfArgsCounter == MinPatternThreshold) then
        	if not (EnforceNumOfArgs) then
                	EnforceNumOfArgs = NumOfArgs
		else
			EnforceNumOfArgs = EnforceNumOfArgs.. ", " ..NumOfArgs
		end
                m.log(4, "NumOfArgs Reached Pattern Threshold. Adding it to the EnforceRequestMethods list: " .. EnforceNumOfArgs)
                m.setvar("RESOURCE.enforce_num_of_args", EnforceNumOfArgs)
        end

	if (TrafficCounter == MinTrafficThreshold) then 
		i=1
		num_of_args={}
		for num in string.gmatch(EnforceNumOfArgs, "%d+") do 
			num_of_args[i]=num;i=i+1; 
		end
		local MinNumOfArgs = math.min(unpack(num_of_args))
		m.setvar("RESOURCE.MinNumOfArgs", MinNumOfArgs)
		local MaxNumOfArgs = math.max(unpack(num_of_args))
		m.setvar("RESOURCE.MaxNumOfArgs", MaxNumOfArgs)
		m.log(4, "Min # of ARGS: " ..MinNumOfArgs.. " and Max # of ARGS: " ..MaxNumOfArgs.. ".")
		m.setvar("!RESOURCE.NumOfArgs_counter_" .. NumOfArgs, "0")
	end
end

