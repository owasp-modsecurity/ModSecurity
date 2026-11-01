function main()
    local d = m.getvars("RULE");
    m.log(4, "RULE from Lua: " .. #d .. " value(s).");
    return nil;
end
