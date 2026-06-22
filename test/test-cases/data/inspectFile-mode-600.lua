#!/usr/bin/lua

function main(filename)
    local pipe = io.popen(string.format("stat -c %%a %q", filename), "r")
    if pipe == nil then
        return nil
    end

    local mode = pipe:read("*l")
    pipe:close()

    if mode == "600" then
        return "1"
    end

    return nil
end
