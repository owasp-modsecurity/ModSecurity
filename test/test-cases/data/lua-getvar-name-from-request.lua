function main()
    local name = m.getvar("ARGS.name");
    local v = m.getvar(name);
    if v == nil then
        m.log(4, "The variable named in the request is not available from Lua.");
    end
    return nil;
end
