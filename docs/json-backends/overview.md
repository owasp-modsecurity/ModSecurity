# JSON Backend Overview
# Übersicht JSON-Backends

## English
This repository now uses an internal JSON abstraction layer in `src/json/`.
It decouples core logic from direct YAJL generator usage and supports gradual migration.

Implemented now:
- Internal generator abstraction (`json_backend.*`)
- Event walker API (`json_walk.*`)
- Functional walker backends:
  - C: `json-c`
  - C++: `nlohmann/json`

Still pending:
- Request-body YAJL SAX parser migration (`src/request_body_processor/json.*`)
- Full backend implementations for all remaining providers

## Deutsch
Dieses Repository nutzt jetzt eine interne JSON-Abstraktionsschicht in `src/json/`.
Sie entkoppelt Kernlogik von direkter YAJL-Generator-Nutzung und ermöglicht eine schrittweise Migration.

Aktuell implementiert:
- Interne Generator-Abstraktion (`json_backend.*`)
- Event-Walker-API (`json_walk.*`)
- Funktionale Walker-Backends:
  - C: `json-c`
  - C++: `nlohmann/json`

Noch offen:
- Migration des YAJL-SAX-Request-Body-Parsers (`src/request_body_processor/json.*`)
- Vollständige Backend-Implementierungen für die übrigen Provider
