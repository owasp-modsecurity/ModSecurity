function main()
    local v = m.getvar("UNKNOWN_VARIABLE");
    if v == nil then
        m.log(4, "UNKNOWN_VARIABLE is not available from Lua.");
    else
        m.log(4, "UNKNOWN_VARIABLE from Lua: " .. v);
    end
    return nil;
end
