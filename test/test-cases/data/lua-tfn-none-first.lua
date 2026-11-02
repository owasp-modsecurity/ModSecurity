function main()
    local v = m.getvar("ARGS:a", { "none", "lowercase" });
    m.log(4, "none first: " .. tostring(v));
    return nil;
end
