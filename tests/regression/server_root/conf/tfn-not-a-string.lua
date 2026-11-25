function main()
    local v = m.getvar("ARGS.a", { "lowercase", {}, 12 });
    m.log(4, "not a string: " .. tostring(v));
    return nil;
end
