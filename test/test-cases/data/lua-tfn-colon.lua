function main()
    local d = m.getvars("ARGS:a", "lowercase:");
    m.log(4, "colon: " .. tostring(d[1].value));
    return nil;
end
