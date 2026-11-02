function main()
    local d = m.getvars("RULE.id");
    m.log(4, "RULE.id from Lua: " .. #d .. " value(s).");
    return nil;
end
