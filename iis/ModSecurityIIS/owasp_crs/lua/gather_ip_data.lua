#!/opt/local/bin/lua
require("io");

function main()
local anomaly_score = m.getvar("TX.ANOMALY_SCORE", "none");
	m.log(4, "Anomaly Score is: " .. anomaly_score .. ".");
local remote_addr = m.getvar("ARGS.REMOTE_ADDR", "none");
	m.log(4, "Remote IP is: " .. remote_addr .. ".");
local ip_hostname = m.getvar("IP.HOSTNAME", "none");

if ((anomaly_score ~= nil) and (ip_hostname == nil)) then
local hostname = "NONE";
local abuse_contact = "NONE";
	n = os.tmpname ()
	os.execute ("nslookup '" .. remote_addr .. "' > " .. n)
	os.execute ("whois '" .. remote_addr .. "' >> " .. n)
	for line in io.lines (n) do
	  if string.match(line, "name = ") then
		hostname = line
	  end

	  if string.match(line, "abuse") then
                abuse_contact = line
          end
	end
                m.log(4, "Hostname is: " .. hostname .. ".");
                m.setvar("tx.hostname", hostname);

                m.log(4, "Abuse Contact is: " .. abuse_contact .. ".");
                m.setvar("tx.abuse_contact", abuse_contact);

	os.remove (n)
return("Nslookup: " .. hostname .. " and WHOIS Abuse Info: " .. abuse_contact .. "");

end
return nil;
end
