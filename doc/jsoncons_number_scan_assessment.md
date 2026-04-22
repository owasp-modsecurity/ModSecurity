# Assessment of the jsoncons Number Path

# 1. Deutsch

## 1.1 Zweck

Dieses Dokument beschreibt den aktuellen Zahlpfad des `jsoncons`-Backends in dieser Repository-Version und die dazu ergaenzten Regression-Tests. Es dokumentiert den sichtbaren Implementierungsstand belegt am Code und an der vendorten `jsoncons`-Dokumentation, ohne Aenderungen an Third-Party-Code vorauszusetzen.

## 1.2 Zusammenfassung

Die aktuelle Implementierung ist im gegebenen Scope funktional korrekt und abgeschlossen, auch wenn der zusaetzliche Zahlenscan eine bekannte Performance-Kostenstelle bleibt.

Der aktuelle Code verarbeitet Zahlenevents im `jsoncons`-Backend ueber `emitEvent(...)` und verwendet dafuer zusaetzlich einen `RawJsonTokenCursor`, der Rohzahlentoken direkt aus dem Original-Input rekonstruiert (`src/request_body_processor/json_backend_jsoncons.cc:242-252`, `src/request_body_processor/json_backend_jsoncons.cc:523-539`, `src/request_body_processor/json_backend_jsoncons.cc:577-705`).

Eine backend-only Entfernung dieses Scans ist im sichtbaren Scope nicht belastbar belegt, weil der oeffentliche Cursor-Kontext der vendorten `jsoncons`-Version hier keine nutzbaren `begin_position()`- und `end_position()`-Werte bereitstellt (`others/jsoncons/include/jsoncons/json_cursor.hpp:405-457`, `others/jsoncons/include/jsoncons/ser_utils.hpp:18-46`).

Als Regression-Schutz wurden exakte Lexemtests am `JsonEventSink::on_number(std::string_view)`-Rand und zusaetzliche End-to-End-Regressionsfaelle fuer fehlende Root-Scalar-Zahlen ergaenzt (`test/unit/json_backend_depth_tests.cc:149-316`, `test/test-cases/regression/request-body-parser-json-backend-edgecases.json:47-360`).

## 1.3 Technische Umsetzung im aktuellen Code

`JSONAdapter::parse(...)` ist der gemeinsame Eintrittspunkt und ruft bei einem Build mit `MSC_JSON_BACKEND_JSONCONS` den Pfad `parseDocumentWithJsoncons(...)` auf (`src/request_body_processor/json_adapter.cc:59-78`).

`parseDocumentWithJsoncons(...)` konfiguriert `jsoncons::json_options` mit `max_nesting_depth`, `lossless_number(true)` und `lossless_bignum(true)`, baut einen `jsoncons::json_string_cursor` ueber dem Input auf und initialisiert parallel dazu einen `RawJsonTokenCursor` ueber demselben Eingabestring (`src/request_body_processor/json_backend_jsoncons.cc:716-760`).

Der zusaetzliche Rohscan ist im lokalen Hilfstyp `RawJsonTokenCursor` implementiert. `consumeNextNumberToken(...)` sucht ab der aktuellen Cursor-Position zuerst mit `skipToNextNumberToken(...)` nach dem naechsten Zahlentoken und liest das Token dann mit `consumeNumberAt(...)` ein (`src/request_body_processor/json_backend_jsoncons.cc:242-252`, `src/request_body_processor/json_backend_jsoncons.cc:425-486`, `src/request_body_processor/json_backend_jsoncons.cc:523-539`).

`consumeNumberAt(...)` behandelt dabei sichtbar Vorzeichen, Ganzzahlteil, Nachkommateil und Exponent einschliesslich Exponent-Vorzeichen (`src/request_body_processor/json_backend_jsoncons.cc:429-486`). `skipInsignificantAt(...)` ueberspringt Leerraum, Komma und Doppelpunkt, waehrend `isNumberBoundary(...)` Leerraum sowie `,`, `]` und `}` als Zahlgrenzen erkennt (`src/request_body_processor/json_backend_jsoncons.cc:298-315`).

Im Eventpfad gibt es zwei relevante Zahlzweige.

- Fuer `string_value`-Events, die intern als numerische Stringevents erkannt werden, versucht der Code zuerst `advanceExactNumber(decoded_number, ...)`. Wenn das gelingt, wird der dekodierte String direkt an `on_number(...)` weitergegeben. Wenn das nicht gelingt, faellt der Code auf `consumeNextNumberToken(...)` und danach auf `rawNumberFromContext(...)` zurueck (`src/request_body_processor/json_backend_jsoncons.cc:622-658`).
- Fuer `int64_value`, `uint64_value`, `double_value` und `half_value` wird immer zuerst `consumeNextNumberToken(...)` aufgerufen. Das Ergebnis wird dann ueber `rawNumberFromContext(...)` materialisiert und an `on_number(...)` weitergereicht (`src/request_body_processor/json_backend_jsoncons.cc:684-705`).

`rawNumberFromContext(...)` arbeitet in klarer Reihenfolge. Zuerst versucht die Funktion, einen Kandidaten ueber `context.begin_position()` und `context.end_position()` aus dem Original-Input auszuschneiden. Falls dieser Kandidat nicht passt, verwendet sie den bereits gescannten Tokenstring. Fuer numerische Stringevents kann sie zuletzt noch den dekodierten Event-String verwenden, sofern dieser selbst ein gueltiges JSON-Zahllexem ist (`src/request_body_processor/json_backend_jsoncons.cc:545-575`).

Die aktivierten Optionen beeinflussen die von `jsoncons` gelieferten Eventformen. Laut vendorter Dokumentation liest `lossless_number(true)` Zahlen mit Nachkommateil oder Exponent als String mit `semantic_tag::bigdec`, und `lossless_bignum(true)` behaelt uebergrosse Ganzzahlen als String mit `semantic_tag::bigint` und uebergrosse Gleitkommawerte als `bigdec` (`others/jsoncons/doc/ref/corelib/basic_json_options.md:25-29`).

Der Parsercode belegt diese Eventformen direkt. Bei Integer-Ueberlauf emittiert der Parser `string_value(..., semantic_tag::bigint, ...)`, und bei aktivem `lossless_number_` emittiert er fuer Zahlen mit Nachkommateil oder Exponent `string_value(..., semantic_tag::bigdec, ...)` (`others/jsoncons/include/jsoncons/json_parser.hpp:2458-2554`).

Die internen Parserpositionsdaten sind im Parser selbst vorhanden. Beim ersten `-`, bei `0` und bei `1` bis `9` setzt der Parser `begin_position_`, `parse_number(...)` verarbeitet Integer-, Fraction- und Exponent-Anteile, und die Parserklasse ueberschreibt `begin_position()` und `end_position()` (`others/jsoncons/include/jsoncons/json_parser.hpp:1096-1126`, `others/jsoncons/include/jsoncons/json_parser.hpp:1720-1940`, `others/jsoncons/include/jsoncons/json_parser.hpp:2380-2392`).

## 1.4 Warum die aktuelle Implementierung korrekt ist

Im sichtbaren Scope ist die beobachtbare Vertragsgrenze das an `JsonEventSink::on_number(std::string_view)` weitergereichte Rohzahllexem. Genau an dieser Grenze arbeitet die aktuelle Implementierung: primitive Zahlenevents werden ueber den Rohscan materialisiert, numerische Stringevents werden bevorzugt ueber `advanceExactNumber(...)` synchronisiert und sonst ebenfalls ueber den Rohscan abgesichert (`src/request_body_processor/json_backend_jsoncons.cc:622-705`).

Der lokale Rohscan bildet die JSON-Zahlsyntax im sichtbaren Code konkret ab. Der Scanner behandelt negatives Vorzeichen, Nachkommateil, Exponent, Exponent-Vorzeichen, Leerraum sowie die in Arrays und Objekten sichtbaren Trenner (`src/request_body_processor/json_backend_jsoncons.cc:298-315`, `src/request_body_processor/json_backend_jsoncons.cc:425-486`, `src/request_body_processor/json_backend_jsoncons.cc:523-539`).

Die neuen Unit-Tests pruefen genau diese beobachtbare Eigenschaft ohne Normalisierung: `collectNumberLexemes(...)` parst ueber `JSONAdapter::parse(...)`, sammelt jede `on_number(...)`-Nutzlast als String und vergleicht Root-Scalar-Faelle sowie einen Objekt/Array-Fall mit Whitespace und Trennern exakt gegen die erwarteten Lexeme (`test/unit/json_backend_depth_tests.cc:149-316`).

Die zusaetzlichen Regressionsfaelle im JSON-Testfile sichern denselben Vertrag noch einmal End-to-End ueber `ARGS:json` und den Debug-Log ab, jeweils mit exakter Stringerwartung fuer das urspruengliche Zahllexem (`test/test-cases/regression/request-body-parser-json-backend-edgecases.json:47-360`).

Diese Bewertung behauptet nicht, dass der Pfad performance-optimal ist. Sie beschreibt, dass die aktuelle Implementierung im gegebenen Scope das rohe Zahllexem funktional korrekt weiterreicht und deshalb als abgeschlossen bewertet werden kann.

## 1.5 Warum keine backend-only Entfernung des Scans moeglich ist

Die sichtbare `jsoncons`-Dokumentation beschreibt `basic_json_cursor::context()` als Rueckgabe des aktuellen `ser_context` (`others/jsoncons/doc/ref/corelib/basic_json_cursor.md:117-119`). In der vendorten Implementierung gibt `basic_json_cursor::context()` tatsaechlich `*this` zurueck (`others/jsoncons/include/jsoncons/json_cursor.hpp:405-407`).

Im selben Cursorcode sind nur `line()` und `column()` ueberschrieben (`others/jsoncons/include/jsoncons/json_cursor.hpp:450-457`). Die Basisklasse `ser_context` liefert fuer `begin_position()` und `end_position()` dagegen standardmaessig `0` (`others/jsoncons/include/jsoncons/ser_utils.hpp:18-46`).

Die vendorte Dokumentation sagt zu `ser_context`, dass `begin_position()` und `end_position()` derzeit nur fuer den JSON-Parser unterstuetzt werden (`others/jsoncons/doc/ref/corelib/ser_context.md:20-33`). Der Parser selbst besitzt diese Positionsdaten auch intern (`others/jsoncons/include/jsoncons/json_parser.hpp:1096-1126`, `others/jsoncons/include/jsoncons/json_parser.hpp:2380-2392`), aber im sichtbaren Backend-Code wird mit `cursor.context()` gearbeitet, nicht mit dem Parserobjekt selbst (`src/request_body_processor/json_backend_jsoncons.cc:758-760`).

Damit ist ein backend-only Fast Path ueber den oeffentlichen Cursor-Kontext in dieser Repository-Version nicht belastbar belegt. Genau deshalb bleibt der bestehende Zahlenscan im aktuellen Scope funktional notwendig.

## 1.6 Testabdeckung

Die ergaenzten Tests in `test/unit/json_backend_depth_tests.cc` fuegen einen kleinen `NumberCollectingSink` hinzu, der nur `on_number(std::string_view)` sammelt, und einen Helper, der `JSONAdapter::parse(...)` ueber beliebigen Input ausfuehrt (`test/unit/json_backend_depth_tests.cc:70-186`).

Die Root-Scalar-Regressionsfaelle pruefen unveraenderte Rohlexeme fuer `0`, `-0`, `1.0`, `1e3`, `-1.25e-4`, `18446744073709551615`, `18446744073709551616` und `123456789012345678901234567890` (`test/unit/json_backend_depth_tests.cc:229-253`).

Ein weiterer Test prueft dieselben Lexemtypen in einem gemischten Objekt/Array mit Leerraum und Trennern und vergleicht die gesamte `on_number(...)`-Sequenz exakt (`test/unit/json_backend_depth_tests.cc:256-275`).

Das bestehende Regressionsfile wurde nur um die bislang fehlenden Root-Scalar-Faelle `0`, `-1.25e-4`, `18446744073709551615` und `18446744073709551616` ergaenzt. Bereits vorhandene Faelle wie `1.0`, `1e3`, `-0` und `123456789012345678901234567890` blieben unveraendert (`test/test-cases/regression/request-body-parser-json-backend-edgecases.json:47-360`, `test/test-cases/regression/request-body-parser-json-backend-edgecases.json:362-405`).

Die Tests behaupten bewusst nicht, dass interne `jsoncons`-Positionsdaten korrekt nach aussen propagiert werden. Sie sichern das beobachtbare Backend-Verhalten am `on_number(...)`-Rand und ueber den bestehenden ModSecurity-Regressionspfad ab.

## 1.7 Fazit

Die aktuelle `jsoncons`-Implementierung ist in diesem Repository-Stand im gegebenen Scope funktional korrekt und abgeschlossen. Der zusaetzliche Zahlenscan ist sichtbar vorhanden und bleibt eine bekannte Performance-Kostenstelle, ist im aktuell erlaubten backend-only Rahmen aber weiterhin die massgebliche und notwendige Quelle fuer das Rohzahllexem.

Die neuen Tests liefern dafuer gezielten Regression-Schutz, ohne Third-Party-Code zu aendern oder unbelegte Aussagen ueber nicht sichtbare Schnittstellen zu treffen.

# 2. English

## 2.1 Purpose

This document records the current number-token path of the `jsoncons` backend in this repository revision and the regression tests that were added around it. It is intentionally limited to what is directly supported by the repository code and the vendored `jsoncons` documentation.

## 2.2 Summary

The current implementation is functionally correct and complete within the given scope, even though the additional numeric scan remains a known performance cost.

In the current code, numeric events are handled by `emitEvent(...)` together with an additional `RawJsonTokenCursor` that reconstructs raw numeric tokens from the original input (`src/request_body_processor/json_backend_jsoncons.cc:242-252`, `src/request_body_processor/json_backend_jsoncons.cc:523-539`, `src/request_body_processor/json_backend_jsoncons.cc:577-705`).

A backend-only removal of that scan is not currently supported by the visible scope, because the public cursor context exposed by the vendored `jsoncons` version does not provide usable `begin_position()` and `end_position()` values here (`others/jsoncons/include/jsoncons/json_cursor.hpp:405-457`, `others/jsoncons/include/jsoncons/ser_utils.hpp:18-46`).

As regression protection, exact lexeme comparisons were added at the `JsonEventSink::on_number(std::string_view)` boundary together with additional end-to-end regression cases for the missing root-scalar numeric inputs (`test/unit/json_backend_depth_tests.cc:149-316`, `test/test-cases/regression/request-body-parser-json-backend-edgecases.json:47-360`).

## 2.3 Current implementation

`JSONAdapter::parse(...)` is the shared entry point and dispatches to `parseDocumentWithJsoncons(...)` when ModSecurity is built with `MSC_JSON_BACKEND_JSONCONS` (`src/request_body_processor/json_adapter.cc:59-78`).

`parseDocumentWithJsoncons(...)` configures `jsoncons::json_options` with `max_nesting_depth`, `lossless_number(true)`, and `lossless_bignum(true)`, creates a `jsoncons::json_string_cursor` on the input, and also creates a `RawJsonTokenCursor` over the same input text (`src/request_body_processor/json_backend_jsoncons.cc:716-760`).

The additional raw scan is implemented in the local `RawJsonTokenCursor`. `consumeNextNumberToken(...)` first locates the next numeric token with `skipToNextNumberToken(...)` and then reads the token with `consumeNumberAt(...)` (`src/request_body_processor/json_backend_jsoncons.cc:242-252`, `src/request_body_processor/json_backend_jsoncons.cc:425-486`, `src/request_body_processor/json_backend_jsoncons.cc:523-539`).

`consumeNumberAt(...)` explicitly handles sign, integer part, fraction, exponent, and exponent sign (`src/request_body_processor/json_backend_jsoncons.cc:429-486`). `skipInsignificantAt(...)` skips whitespace, commas, and colons, while `isNumberBoundary(...)` treats whitespace together with `,`, `]`, and `}` as numeric boundaries (`src/request_body_processor/json_backend_jsoncons.cc:298-315`).

There are two relevant numeric event branches.

- For `string_value` events recognized as numeric string events, the code first attempts `advanceExactNumber(decoded_number, ...)`. If that succeeds, the decoded string is sent directly to `on_number(...)`. If it does not, the code falls back to `consumeNextNumberToken(...)` and `rawNumberFromContext(...)` (`src/request_body_processor/json_backend_jsoncons.cc:622-658`).
- For `int64_value`, `uint64_value`, `double_value`, and `half_value`, the code always calls `consumeNextNumberToken(...)` first, then materializes the token through `rawNumberFromContext(...)`, and finally passes it to `on_number(...)` (`src/request_body_processor/json_backend_jsoncons.cc:684-705`).

`rawNumberFromContext(...)` follows a fixed order. It first attempts to slice a candidate directly from the original input using `context.begin_position()` and `context.end_position()`. If that candidate is not usable, it falls back to the already scanned token. For numeric string events it can finally use the decoded event string, provided that string is itself a valid JSON number lexeme (`src/request_body_processor/json_backend_jsoncons.cc:545-575`).

The enabled `jsoncons` options change the event shapes delivered by the parser. The vendored documentation states that `lossless_number(true)` reads numbers with fractional parts or exponents as strings tagged `semantic_tag::bigdec`, and that `lossless_bignum(true)` preserves out-of-range integers as strings tagged `semantic_tag::bigint` and out-of-range floating-point values as `bigdec` (`others/jsoncons/doc/ref/corelib/basic_json_options.md:25-29`).

The parser implementation matches that documentation. On integer overflow it emits `string_value(..., semantic_tag::bigint, ...)`, and when `lossless_number_` is enabled it emits `string_value(..., semantic_tag::bigdec, ...)` for numbers with fractions or exponents (`others/jsoncons/include/jsoncons/json_parser.hpp:2458-2554`).

The parser also maintains internal position data. It sets `begin_position_` when it first sees `-`, `0`, or `1` through `9`, `parse_number(...)` handles integer, fraction, and exponent states, and the parser class overrides `begin_position()` and `end_position()` (`others/jsoncons/include/jsoncons/json_parser.hpp:1096-1126`, `others/jsoncons/include/jsoncons/json_parser.hpp:1720-1940`, `others/jsoncons/include/jsoncons/json_parser.hpp:2380-2392`).

## 2.4 Why the current implementation is correct

Within the visible scope, the relevant contract boundary is the raw numeric lexeme passed into `JsonEventSink::on_number(std::string_view)`. That is exactly what the current implementation preserves: primitive numeric events are materialized through the raw scan, and numeric string events are first synchronized through `advanceExactNumber(...)` when possible and otherwise protected by the same raw scan path (`src/request_body_processor/json_backend_jsoncons.cc:622-705`).

The local raw scanner explicitly models the JSON number syntax visible in this repository. It covers negative signs, fractions, exponents, exponent signs, whitespace, and the delimiters used inside arrays and objects (`src/request_body_processor/json_backend_jsoncons.cc:298-315`, `src/request_body_processor/json_backend_jsoncons.cc:425-486`, `src/request_body_processor/json_backend_jsoncons.cc:523-539`).

The added unit tests validate that exact observable behavior without normalization. `collectNumberLexemes(...)` parses through `JSONAdapter::parse(...)`, captures every `on_number(...)` payload as a string, and compares both root-scalar cases and a mixed object/array input with whitespace and delimiters against the exact expected lexemes (`test/unit/json_backend_depth_tests.cc:149-316`).

The added regression cases in the JSON test file check the same contract end-to-end through `ARGS:json` and the debug log, again with exact string expectations for the original numeric lexemes (`test/test-cases/regression/request-body-parser-json-backend-edgecases.json:47-360`).

This assessment does not claim that the path is performance-optimal. It documents that, within the current scope, the implementation functionally preserves the raw numeric lexeme and can therefore be treated as complete for this repository state.

## 2.5 Why backend-only removal is not currently supported

The visible `jsoncons` documentation describes `basic_json_cursor::context()` as returning the current `ser_context` (`others/jsoncons/doc/ref/corelib/basic_json_cursor.md:117-119`). In the vendored implementation, `basic_json_cursor::context()` does in fact return `*this` (`others/jsoncons/include/jsoncons/json_cursor.hpp:405-407`).

In that same cursor implementation, only `line()` and `column()` are overridden (`others/jsoncons/include/jsoncons/json_cursor.hpp:450-457`). The base `ser_context` implementation returns `0` for `begin_position()` and `end_position()` (`others/jsoncons/include/jsoncons/ser_utils.hpp:18-46`).

The vendored `ser_context` documentation says that `begin_position()` and `end_position()` are currently only supported for the JSON parser (`others/jsoncons/doc/ref/corelib/ser_context.md:20-33`). The parser itself does have those positions internally (`others/jsoncons/include/jsoncons/json_parser.hpp:1096-1126`, `others/jsoncons/include/jsoncons/json_parser.hpp:2380-2392`), but the visible backend code operates through `cursor.context()`, not through the parser object directly (`src/request_body_processor/json_backend_jsoncons.cc:758-760`).

For that reason, a backend-only fast path based on the public cursor context is not supported by the currently visible code. The existing numeric scan therefore remains the authoritative and necessary source of the raw numeric lexeme in the present scope.

## 2.6 Test coverage

The additions in `test/unit/json_backend_depth_tests.cc` introduce a small `NumberCollectingSink` that only records `on_number(std::string_view)` and a helper that executes `JSONAdapter::parse(...)` on arbitrary input (`test/unit/json_backend_depth_tests.cc:70-186`).

The root-scalar regression cases verify unchanged raw lexemes for `0`, `-0`, `1.0`, `1e3`, `-1.25e-4`, `18446744073709551615`, `18446744073709551616`, and `123456789012345678901234567890` (`test/unit/json_backend_depth_tests.cc:229-253`).

An additional test validates the same lexeme classes inside a mixed object/array input with whitespace and delimiters and compares the full `on_number(...)` sequence exactly (`test/unit/json_backend_depth_tests.cc:256-275`).

The existing regression file was extended only with the previously missing root-scalar cases `0`, `-1.25e-4`, `18446744073709551615`, and `18446744073709551616`. Existing cases such as `1.0`, `1e3`, `-0`, and `123456789012345678901234567890` were left in place (`test/test-cases/regression/request-body-parser-json-backend-edgecases.json:47-360`, `test/test-cases/regression/request-body-parser-json-backend-edgecases.json:362-405`).

The tests intentionally do not claim that internal `jsoncons` position data is propagated outward correctly. They protect the observable backend behavior at the `on_number(...)` boundary and through the established ModSecurity regression path.

## 2.7 Conclusion

In this repository state, the current `jsoncons` implementation is functionally correct and complete within the given scope. The additional numeric scan is visibly present and remains a known performance cost, but within the currently allowed backend-only scope it is still the decisive and necessary source for the raw numeric lexeme.

The added tests provide focused regression protection for that behavior without modifying third-party code or asserting capabilities that are not directly supported by the visible interfaces.
