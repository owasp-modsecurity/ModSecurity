function main()
    local r = m.setvar("novar", "x");
    m.log(4, "m.setvar() without a collection returned: " .. tostring(r));
    return nil;
end
