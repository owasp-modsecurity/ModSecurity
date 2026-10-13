#!/usr/bin/lua

function main(filename)
    local file = io.open(filename, "r")
    if file == nil then
        return nil
    end

    file:close()
    return "1"
end
