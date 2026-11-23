function main()
    local v = m.getvar("ARGS:a", { "lowercase", "none" });
    m.log(4, "none last: " .. tostring(v));
    return nil;
end
