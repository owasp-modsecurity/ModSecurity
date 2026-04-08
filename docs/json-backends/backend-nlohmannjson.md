# Backend: nlohmann/json

Status: **funktional (DOM + Walker/Event-Adapter)**.

- Configure token: `--with-json-cxx=nlohmannjson`
- Erkennung: Header-Check `nlohmann/json.hpp`
- Fallback: `third_party/nlohmann_json/include/nlohmann/json.hpp`
- Capabilities: `DOM`, `EVENTS`, `TYPED_BINDING`

Hinweis: Der Walker in `src/json/json_walk.cc` kann JSON über `nlohmann::json` parsen und als internes Event-Interface ablaufen.
