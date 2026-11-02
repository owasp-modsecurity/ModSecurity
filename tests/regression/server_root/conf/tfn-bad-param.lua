function main()
    local v = m.getvar("ARGS.a", true);
    m.log(4, "bad param: " .. tostring(v));
    return nil;
end
