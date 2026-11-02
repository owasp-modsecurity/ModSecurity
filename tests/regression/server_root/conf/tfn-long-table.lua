function main()
    local t = {};
    for i = 1, 1000 do t[i] = "lowercase"; end
    local v = m.getvar("ARGS.a", t);
    m.log(4, "long table: " .. tostring(v));
    return nil;
end
