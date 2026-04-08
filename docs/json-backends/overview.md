# JSON backend overview

Diese Migration führt eine interne JSON-Backend-Schicht (`src/json/`) ein, damit YAJL schrittweise aus direkten Call-Sites entfernt werden kann.

## Implementiert
- Übergangs-Writer-API inkl. Capability-Flags in `src/json/json_backend.h/.cc`.
- Event-Walker-Interface in `src/json/json_walk.h`.
- Funktionaler DOM->Event-Walker für:
  - C: `json-c`
  - C++: `nlohmann/json`

## Build-Auswahl
- `--with-json-c=auto|yyjson|json-c|jansson|cjson|none`
- `--with-json-cxx=auto|nlohmannjson|jsoncpp|jsoncons|simdjson|glaze|none`
- `--with-json-prefer=system|bundled|auto`
- `--with-json-mode=auto|dom|events`
- `--enable-json-backend-debug`

## Auto-Priorität
C: `yyjson > json-c > jansson > cjson`

C++: `simdjson > nlohmann/json > jsoncons > jsoncpp > glaze`

## Noch YAJL-gebunden
- Request-Body-JSON-Prozessor (`src/request_body_processor/json.*`) bleibt vorerst YAJL/SAX-basiert und wird in Folgeschritten auf den Event-Walker migriert.
