# Build and Configure Integration
# Build- und Configure-Integration

## English
Configure options:
- `--with-json-c=auto|yyjson|json-c|jansson|cjson|none`
- `--with-json-cxx=auto|nlohmannjson|jsoncpp|jsoncons|simdjson|glaze|none`
- `--with-json-prefer=system|bundled|auto`
- `--with-json-mode=auto|dom|events`
- `--enable-json-backend-debug`

Selection rules:
1. Validate option values.
2. Resolve backend according to requested backend and priority.
3. Prefer system or bundled by `--with-json-prefer`.
4. For bundled mode, verify required headers exist.
5. On explicit request, fail with clear error if unresolved.

C auto order: `yyjson > json-c > jansson > cjson`.

## Deutsch
Configure-Optionen:
- `--with-json-c=auto|yyjson|json-c|jansson|cjson|none`
- `--with-json-cxx=auto|nlohmannjson|jsoncpp|jsoncons|simdjson|glaze|none`
- `--with-json-prefer=system|bundled|auto`
- `--with-json-mode=auto|dom|events`
- `--enable-json-backend-debug`

Auswahllogik:
1. Werte validieren.
2. Backend anhand gewünschter Auswahl und Priorität auflösen.
3. System oder Bundled gemäß `--with-json-prefer` bevorzugen.
4. Im Bundled-Modus notwendige Header prüfen.
5. Bei expliziter Auswahl mit klarer Fehlermeldung abbrechen, falls nicht auflösbar.

C-Auto-Reihenfolge: `yyjson > json-c > jansson > cjson`.
