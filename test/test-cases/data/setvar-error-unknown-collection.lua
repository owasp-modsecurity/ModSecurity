function main()
    local r = m.setvar("FOO.bar", "x");
    m.log(4, "m.setvar() with an unknown collection returned: " .. tostring(r));
    return nil;
end
