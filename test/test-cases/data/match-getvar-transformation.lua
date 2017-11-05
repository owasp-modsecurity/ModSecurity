function main()
    ret = nil

    var = m.getvar("tx.test");
    if var == nil then
        m.log(9, "Don't know what to say...");
        return ret
    end

    if var == "FELIPE"
        m.log(9, "Ops.");
    elseif var == "felipe"
        m.log(9, "Just fine.");
        ret ="ok";
    else
        m.log(9, "Really?");
    end

    return ret
end
