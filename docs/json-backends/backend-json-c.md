# Backend: json-c

Status: **funktional (DOM + Walker/Event-Adapter)**.

- Configure token: `--with-json-c=json-c`
- Erkennung: `pkg-config json-c` + Header/Link-Checks über `MSC_CHECK_LIB`
- Fallback: `third_party/json-c` (bundled)
- Capabilities: `DOM`, `EVENTS`, `WRITER`

Hinweis: Writer-Pfad läuft über die interne JSON-API; Walker nutzt echtes `json-c`-DOM Parsing in `src/json/json_walk.cc`.
