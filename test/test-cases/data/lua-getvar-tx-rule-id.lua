function main()
    local v = m.getvar("TX.rule_id");
    m.log(4, "Called from rule " .. tostring(v) .. ".");
    return nil;
end
