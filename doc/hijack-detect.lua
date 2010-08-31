--[[
    This is an example ModSecurity 2.5 Lua script to perform Session Hijacking
    Detection by storing session information into a SQL database.

    This script simply keeps track of the IP address and User Agent
    string for a session.  If these change, but keep the same session,
    then it is considered hijacking.

    Requires luasqlite3: http://luasqlite.luaforge.net/

    Author: Brian Rectanus <brectanu@gmail.com>
]]--

local t0 = os.clock();
m.log(9, "HijackDetect: Begin loading script.");

-- Requires luasqlite3: http://luasqlite.luaforge.net/
require "lsqlite3";


----------------------------------------
--[[ Edit these to your environment ]]--
----------------------------------------

-- Name of the database file
HIJACK_DB = "/tmp/modsec-hijack.db";

-- Expire session in N seconds (stop tracking session if idle longer than this)
HIJACK_EXPIRE = 300;

-- Always alert even if only one of IP or UserAgent is a mismatch if set true
HIJACK_ALWAYS_ALERT = true;

-- Do not issue warnings at all if set true
HIJACK_NO_WARN = false;

-- What session IDs to look for
HIJACK_SESSID = {
    ["REQUEST_COOKIES:sessionid"]       = 1,
    ["REQUEST_COOKIES:jsessionid"]      = 1,
    ["REQUEST_COOKIES:sessid"]          = 1,
    ["REQUEST_COOKIES:phpsessid"]       = 1,
--    ["REQUEST_COOKIES:aspsession"]      = 1,
--    ["REQUEST_COOKIES:jservsession"]    = 1,
--    ["REQUEST_COOKIES:jwsession"]       = 1,
--    ["REQUEST_COOKIES:aspsession-id"]   = 1,
--    ["REQUEST_COOKIES:jservsession-id"] = 1,
--    ["REQUEST_COOKIES:jwsession-id"]    = 1,
--    ["REQUEST_COOKIES:aspsession_id"]   = 1,
--    ["REQUEST_COOKIES:jservsession_id"] = 1,
--    ["REQUEST_COOKIES:jwsession_id"]    = 1,
--    ["REQUEST_COOKIES:cfid"]            = 1,
--    ["REQUEST_COOKIES:token"]           = 1,
--    ["REQUEST_COOKIES:sid"]             = 1,
    ["ARGS:sessionid"]                  = 1,
    ["ARGS:jsessionid"]                 = 1,
    ["ARGS:sessid"]                     = 1,
    ["ARGS:phpsessid"]                  = 1,
--    ["ARGS:aspsession"]                 = 1,
--    ["ARGS:jservsession"]               = 1,
--    ["ARGS:jwsession"]                  = 1,
--    ["ARGS:aspsession-id"]              = 1,
--    ["ARGS:jservsession-id"]            = 1,
--    ["ARGS:jwsession-id"]               = 1,
--    ["ARGS:aspsession_id"]              = 1,
--    ["ARGS:jservsession_id"]            = 1,
--    ["ARGS:jwsession_id"]               = 1,
--    ["ARGS:cfid"]                       = 1,
--    ["ARGS:token"]                      = 1,
--    ["ARGS:sid"]                        = 1,
};

----------------------------------------

-- // All the SQL that is used.  Probably do not want to touch this. // --

HIJACK_CREATE_SQL = [[
    CREATE TABLE sess (
          ip  varchar(256)
        , ua  varchar(256)
        , id  varchar(256)
        , val varchar(256)
        , ts  INTEGER
        , PRIMARY KEY ( ip, ua, id ) 
    )
]];
HIJACK_INDEX_SQL = [[
    CREATE INDEX IF NOT EXISTS session ON sess ( id, val )
]];
HIJACK_GET_SQL = [[
    SELECT ua, ip, id, val
      FROM sess
     WHERE id = :id AND val = :val AND (ip <> :ip OR ua <> :ua)
]];
HIJACK_ADD_SQL = [[
    INSERT OR REPLACE INTO sess
    VALUES (:ip, :ua, :id, :val, :ts)
]]
HIJACK_CLEANUP_SQL = [[
    DELETE FROM sess
     WHERE ts <= :ts
]]

-- // The code executed by ModSecurity. // --

function main()
    m.log(9, "HijackDetect: Begin execution.");
    local now = os.time();

    local db = sqlite3.open(HIJACK_DB);
    if db == nil then
        return "HijackDetect: Failed to open database, \"" .. HIJACK_DB .. "\"";
    end

    -- Retrieve the User Agent String
    local ua = m.getvar("REQUEST_HEADERS.User-Agent");
    local ip = m.getvar("REMOTE_ADDR");
    local sess = {};
    local sessn = 0;

    -- Loop through the targets for well-known session IDs
    local cookies = m.getvars("REQUEST_COOKIES", "lowercase" );
    for k,v in pairs(cookies) do
        name = v["name"];
        if HIJACK_SESSID[name] == 1 then
            m.log(9, "HijackDetect: Using sessid " .. name);
            sess[name] = v["value"];
            sessn = sessn + 1;
        end
    end

    local args = m.getvars("ARGS");
    for k,v in pairs(args) do
        name = v["name"];
        if HIJACK_SESSID[name] == 1 then
            m.log(9, "HijackDetect: Using sessid " .. name);
            sess[name] = v["value"];
            sessn = sessn + 1;
        end
    end

    local n = 0;
    local s;
    local rc;

    -- Now attempt to create the table (fails if exists, but that is fine)
    db:exec(HIJACK_CREATE_SQL);
    db:exec(HIJACK_INDEX_SQL);

    -- Cleanup old sessions
    m.log(9, "HijackDetect: Cleanup");
    s = db:prepare(HIJACK_CLEANUP_SQL);
    if s == nil then
        m.log(4, "HijackDetect: Error preparing SQL \"" .. HIJACK_CLEANUP_SQL .."\"" .. rc);
        return nil;
    end
    rc = s:bind_names{ ts=(now - HIJACK_EXPIRE) };
    if rc ~= sqlite3.OK then
        m.log(4, "HijackDetect: Error binding data - " .. rc);
        return nil;
    end
    rc = s:step();
    if rc ~= sqlite3.DONE then
        m.log(4, "HijackDetect: Error executing - " .. rc);
        return nil;
    end
    s:finalize();

    -- Make sure the session IDs are the same
    m.log(9, "HijackDetect: Fetching");
    s = db:prepare(HIJACK_GET_SQL);
    if s == nil then
        m.log(4, "HijackDetect: Error preparing SQL \"" .. HIJACK_GET_SQL .."\"" .. rc);
        return nil;
    end
    for sessid, sessval in pairs(sess) do
        rc = s:bind_names{ ua=ua, ip=ip, id=sessid, val=sessval};
        if rc ~= sqlite3.OK then
            m.log(4, "HijackDetect: Error binding data - " .. rc);
            return nil;
        end

        -- Alert if another ip/ua has this session
        if s:step() == sqlite3.ROW then
            local row = s:get_named_values();

            -- Only alert of both ip/ua change and warn otherwise
            if HIJACK_ALWAYS_ALERT or (row["ip"] ~= ip and row["ua"] ~= ua) then
                m.log(9, "HijackDetect: End execution (match).");
                return string.format("HijackDetect: Detected session hijacking [ip \"%s\"] [ua \"%s\"] [sessid \"%s\"] [sessval \"%s\"]", ip, ua, sessid, sessval);
            elseif HIJACK_NO_WARN == false then
                m.log(9, "HijackDetect: End execution (no match).");
                m.log(3, string.format("Warning. Detected possible session hijacking [ip \"%s\"] [ua \"%s\"] [sessid \"%s\"] [sessval \"%s\"]", ip, ua, sessid, sessval));
                return nil;
            end

        end

        s:reset();
    end
    s:finalize();

    -- Insert/Update sessions
    m.log(9, "HijackDetect: Inserting");
    s = db:prepare(HIJACK_ADD_SQL);
    if s == nil then
        m.log(4, "HijackDetect: Error preparing SQL \"" .. HIJACK_ADD_SQL .."\"" .. rc);
        return nil;
    end
    for k,v in pairs(sess) do
        m.log(9, "HijackDetect: Storing sessid \"" .. k .. "\".");
        rc = s:bind_names{ ua=ua, ip=ip, id=k, val=v, ts=now };
        if rc ~= sqlite3.OK then
            m.log(4, "HijackDetect: Error binding data - " .. rc);
            return nil;
        end
        rc = s:step();
        if rc ~= sqlite3.DONE then
            m.log(4, "HijackDetect: Error executing - " .. rc);
            return nil;
        end
        s:reset();
    end
    s:finalize();

    m.log(9, "HijackDetect: End execution (no match).");
    return nil;
end

local t1 = os.clock();
m.log(9, "HijackDetect: End loading script (" .. os.difftime(t2,t1) .. ").");
