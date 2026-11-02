function main()
    local v = m.getvar("RULE.id");
    if v == nil then
        m.log(4, "RULE.id is not available from Lua.");
    else
        m.log(4, "RULE.id from Lua: " .. v);
    end
    return nil;
end
