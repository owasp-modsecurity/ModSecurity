# Migration notes

## Fertig in diesem Schritt
- Interne JSON-Schicht mit Capability-Flags und Writer-Kompatibilität eingeführt.
- Direkte YAJL-Generator-Includes aus `transaction.cc` und `modsecurity.cc` entfernt und auf interne API umgestellt.
- Build-/Configure-Auswahl für C- und C++-JSON-Backends ergänzt.
- Funktionale Walker integriert für:
  - `json-c` (C)
  - `nlohmann/json` (C++)
- Vendor-Struktur und Importskript für `nlohmann/json` ergänzt.

## Weiter offen
1. Request-Body-JSON parser (`request_body_processor/json.*`) auf internes Event-Interface migrieren.
2. Backend-spezifische Writer/DOM-Implementierungen für yyjson/jansson/cJSON/jsoncpp/jsoncons/simdjson/glaze vervollständigen.
3. Tests für Backend-Matrix ergänzen (auto/system/bundled).
