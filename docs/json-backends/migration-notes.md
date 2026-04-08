# Migration Notes
# Migrationsnotizen

## English
Completed:
- Internal JSON generator abstraction wired into existing JSON output call sites.
- Functional JSON walker for json-c and nlohmann/json.
- Configure options for backend selection, prefer mode, and debug mode.
- Header existence checks for bundled json-c and bundled nlohmann/json.

Open TODOs:
- Migrate request body YAJL SAX processor to internal event interface.
- Implement full backend implementations for yyjson, jansson, cJSON, jsoncpp, jsoncons, simdjson, glaze.

## Deutsch
Abgeschlossen:
- Interne JSON-Generator-Abstraktion in bestehende JSON-Output-Callsites integriert.
- Funktionaler JSON-Walker für json-c und nlohmann/json.
- Configure-Optionen für Backend-Auswahl, Prefer-Modus und Debug-Modus ergänzt.
- Header-Existenzprüfung für bundled json-c und bundled nlohmann/json eingebaut.

Offene TODOs:
- YAJL-SAX-Request-Body-Prozessor auf internes Event-Interface migrieren.
- Vollständige Backend-Implementierungen für yyjson, jansson, cJSON, jsoncpp, jsoncons, simdjson, glaze umsetzen.
