-- Test matching Lua Script to just print debug messages
function main()
    m.log(1, "Test message.");
    m.log(2, "Test message.");
    m.log(3, "Test message.");
    m.log(4, "Test message.");
    m.log(5, "Test message.");
    m.log(6, "Test message.");
    m.log(7, "Test message.");
    m.log(8, "Test message.");
    m.log(9, "Test message.");

    return "Lua script matched.";
end
