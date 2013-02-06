#!/opt/local/bin/lua

function main()

  local response_body = m.getvar("RESPONSE_BODY", "none");
  
  if response_body ~= "" then
  
  local _, nscripts = string.gsub(response_body, "<script", "");
  local _, niframes = string.gsub(response_body, "<iframe", "");
  local _, nlinks = string.gsub(response_body, "a href", "");
  local _, nimages = string.gsub(response_body, "<img", "");

  if nscripts == nil then
    nscripts = 0
  end
  if niframes == nil then
    niframes = 0
  end
  if nlinks == nil then
    nlinks = 0
  end
  if nimages == nil then
    nimages = 0
  end
  m.log(3, "niframes[" .. niframes .. "]");
  m.setvar("tx.niframes", niframes);
  m.log(3, "nscripts[" .. nscripts .. "]");
  m.setvar("tx.nscripts", nscripts);  
  m.log(3, "nlinks[" .. nlinks .. "]");
  m.setvar("tx.nlinks", nlinks);
  m.log(3, "nimages[" .. nimages .. "]");
  m.setvar("tx.nimages", nimages); 
   return nil;
end

  return nil;
end
