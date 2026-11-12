function main()
    local r = m.setvar("tx.a", {});
    m.log(4, "m.setvar() with a table value returned: " .. tostring(r));
    return nil;
end
