function main()
    local r = m.setvar("tx.a");
    m.log(4, "m.setvar() with one argument returned: " .. tostring(r));
    return nil;
end
