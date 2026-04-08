# Autotools integration

## Configure-Optionen
- `--with-json-c=auto|yyjson|json-c|jansson|cjson|none`
- `--with-json-cxx=auto|nlohmannjson|jsoncpp|jsoncons|simdjson|glaze|none`
- `--with-json-prefer=system|bundled|auto`
- `--with-json-mode=auto|dom|events`
- `--enable-json-backend-debug`

## Auflösungslogik
1. Werte werden strikt validiert (`configure` bricht bei ungültigen Werten ab).
2. Je nach Auswahl wird zuerst System oder Bundled bevorzugt (`--with-json-prefer`).
3. Bei explizit angefordertem Backend führt fehlendes System+Bundled zu einem klaren Fehler.
4. Bei `auto` gilt die feste Prioritätsreihenfolge.

## Build-Variablen
- `JSON_BACKEND_CFLAGS`
- `JSON_BACKEND_LDFLAGS`
- `JSON_BACKEND_LDADD`

Diese Variablen werden in `src/Makefile.am` in CPPFLAGS/LDFLAGS/LIBADD injiziert.

## Konfigurationszusammenfassung
`configure` zeigt:
- aktives C-Backend + Quelle
- aktives C++-Backend + Quelle
- Prefer-/Mode-Einstellung
- Debug-Status
