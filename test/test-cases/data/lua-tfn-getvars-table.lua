function main()
    local d = m.getvars("ARGS:b", { "lowercase", "htmlEntityDecode" });
    m.log(4, "getvars table: " .. tostring(d[1].value));
    return nil;
end
