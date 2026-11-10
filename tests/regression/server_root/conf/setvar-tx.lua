function main()
    local r = m.setvar("tx.a", "x");
    m.log(4, "m.setvar() with TX returned: " .. tostring(r));
    m.log(4, "tx.a is now: " .. tostring(m.getvar("tx.a")));
    return nil;
end
