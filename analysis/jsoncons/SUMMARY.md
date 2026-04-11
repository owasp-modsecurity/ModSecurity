# Summary: jsoncons

Diese Übersicht basiert auf statischer Analyse von Dokumentation, Headern, Tests, Beispielen und Benchmarks im Repository. Vorhandene Tests und Benchmarks wurden in diesem Turn nicht ausgeführt.

## Übersicht aller geprüften Dateien

| Doku-Datei | Status | Optimierbar | Priorität | Hauptgrund |
| --- | --- | --- | --- | --- |
| `others/jsoncons/doc/ref/corelib/byte_string_arg_t.md` | widersprüchlich | nein | hohe Priorität | Der dokumentierte Include-Header ist falsch. |
| `others/jsoncons/doc/ref/corelib/json_array_arg_t.md` | widersprüchlich | nein | hohe Priorität | Der dokumentierte Include-Header ist falsch. |
| `others/jsoncons/doc/ref/corelib/json_object_arg_t.md` | widersprüchlich | nein | hohe Priorität | Der dokumentierte Include-Header ist falsch. |
| `others/jsoncons/doc/ref/jsonpath/flatten.md` | widersprüchlich | ja | hohe Priorität | Der dokumentierte Include-Pfad im Kopf ist veraltet oder falsch. |
| `others/jsoncons/doc/ref/jsonpointer/unflatten_options.md` | widersprüchlich | ja | hohe Priorität | Der dokumentierte Include-Pfad ist falsch; korrekt ist `jsoncons_ext/jsonpointer/jsonpointer.hpp`. |
| `others/jsoncons/doc/ref/jsonschema/json_schema/validate.md` | widersprüchlich | ja | hohe Priorität | Die Doku nennt für Overload (1) `void validate(const Json& instance) const`, der Header implementiert aber `Json validate(const Json& instance) const` und gibt den Patch zurück. |
| `others/jsoncons/doc/ref/jsonschema/json_validator.md` | wahrscheinlich veraltet | nein | hohe Priorität | Die dokumentierte Klasse ist im aktuellen Repository nicht in der beschriebenen Form implementiert. |
| `others/jsoncons/doc/ref/jsonschema/schema_error.md` | widersprüchlich | nein | hohe Priorität | Der dokumentierte Include-Pfad ist falsch; die Implementierung liegt in `jsonschema_error.hpp`. |
| `others/jsoncons/doc/ref/jsonschema/schema_version.md` | widersprüchlich | nein | hohe Priorität | Der dokumentierte Include-Pfad ist falsch. |
| `others/jsoncons/doc/ref/jsonschema/validation_output.md` | wahrscheinlich veraltet | nein | hohe Priorität | Der dokumentierte Typ `validation_output` ist im aktuellen Repository nicht nachweisbar. |
| `others/jsoncons/doc/ref/toon/decode_toon.md` | widersprüchlich | ja | hohe Priorität | Der dokumentierte Include-Pfad `jsoncons/decode_toon.hpp` ist falsch; korrekt ist `jsoncons_ext/toon/decode_toon.hpp`. |
| `others/jsoncons/doc/Tutorials/Unicode support.md` | teilweise unklar | nein | mittlere Priorität | Es ist nicht sicher belegbar, ob die prose-lastigen Aussagen durch einen spezifischen Test oder ein Beispiel direkt abgesichert sind. |
| `others/jsoncons/doc/ref/corelib/basic_default_json_visitor.md` | teilweise unklar | nein | mittlere Priorität | Es ist nicht sicher belegbar, ob die prose-lastigen Aussagen durch einen spezifischen Test oder ein Beispiel direkt abgesichert sind. |
| `others/jsoncons/doc/ref/corelib/basic_staj_event.md` | teilweise unklar | nein | mittlere Priorität | Es ist nicht sicher belegbar, ob die prose-lastigen Aussagen durch einen spezifischen Test oder ein Beispiel direkt abgesichert sind. |
| `others/jsoncons/doc/ref/corelib/conversion_error.md` | teilweise unklar | nein | mittlere Priorität | Es ist nicht sicher belegbar, ob die prose-lastigen Aussagen durch einen spezifischen Test oder ein Beispiel direkt abgesichert sind. |
| `others/jsoncons/doc/ref/corelib/conversion_result.md` | teilweise unklar | nein | mittlere Priorität | Es ist nicht sicher belegbar, ob die prose-lastigen Aussagen durch einen spezifischen Test oder ein Beispiel direkt abgesichert sind. |
| `others/jsoncons/doc/ref/corelib/json_type_traits/Eigen-Matrix-example.md` | teilweise unklar | ja | mittlere Priorität | Die Seite beschreibt eher Konzept/Grammatik/Überblick als ein einzelnes Symbol; eine punktgenaue Verifikation bleibt deshalb teilweise unklar. |
| `others/jsoncons/doc/ref/corelib/read_error.md` | teilweise unklar | nein | mittlere Priorität | Es ist nicht sicher belegbar, ob die prose-lastigen Aussagen durch einen spezifischen Test oder ein Beispiel direkt abgesichert sind. |
| `others/jsoncons/doc/ref/corelib/read_result.md` | teilweise unklar | nein | mittlere Priorität | Es ist nicht sicher belegbar, ob die prose-lastigen Aussagen durch einen spezifischen Test oder ein Beispiel direkt abgesichert sind. |
| `others/jsoncons/doc/ref/corelib/reflect/Eigen-Matrix-example.md` | teilweise unklar | ja | mittlere Priorität | Die Seite beschreibt eher Konzept/Grammatik/Überblick als ein einzelnes Symbol; eine punktgenaue Verifikation bleibt deshalb teilweise unklar. |
| `others/jsoncons/doc/ref/corelib/reflect/uses-allocator-construction-example.md` | teilweise unklar | ja | mittlere Priorität | Die Seite beschreibt eher Konzept/Grammatik/Überblick als ein einzelnes Symbol; eine punktgenaue Verifikation bleibt deshalb teilweise unklar. |
| `others/jsoncons/doc/ref/corelib/staj_cursor.md` | teilweise unklar | nein | mittlere Priorität | Es ist nicht sicher belegbar, ob die prose-lastigen Aussagen durch einen spezifischen Test oder ein Beispiel direkt abgesichert sind. |
| `others/jsoncons/doc/ref/corelib/write_result.md` | teilweise unklar | nein | mittlere Priorität | Es ist nicht sicher belegbar, ob die prose-lastigen Aussagen durch einen spezifischen Test oder ein Beispiel direkt abgesichert sind. |
| `others/jsoncons/doc/ref/jsonpath/grammar.md` | teilweise unklar | nein | mittlere Priorität | Es ist nicht sicher belegbar, ob die prose-lastigen Aussagen durch einen spezifischen Test oder ein Beispiel direkt abgesichert sind. |
| `others/jsoncons/doc/ref/jsonpath/jsoncons-jsonpath-abnf.md` | teilweise unklar | nein | mittlere Priorität | Es ist nicht sicher belegbar, ob die prose-lastigen Aussagen durch einen spezifischen Test oder ein Beispiel direkt abgesichert sind. |
| `others/jsoncons/doc/ref/jsonpath/jsonpath_grammer.md` | teilweise unklar | nein | mittlere Priorität | Es ist nicht sicher belegbar, ob die prose-lastigen Aussagen durch einen spezifischen Test oder ein Beispiel direkt abgesichert sind. |
| `others/jsoncons/doc/ref/jsonpath/path_node_kind.md` | teilweise unklar | nein | mittlere Priorität | Es ist nicht sicher belegbar, ob die prose-lastigen Aussagen durch einen spezifischen Test oder ein Beispiel direkt abgesichert sind. |
| `others/jsoncons/doc/ref/jsonpointer/jsonpointer_errc.md` | teilweise unklar | nein | mittlere Priorität | Es ist nicht sicher belegbar, ob die prose-lastigen Aussagen durch einen spezifischen Test oder ein Beispiel direkt abgesichert sind. |
| `others/jsoncons/doc/ref/jsonschema/jsonschema.md` | teilweise unklar | ja | mittlere Priorität | Mindestens ein gezeigtes C++-Beispiel ist in der Doku syntaktisch unvollständig. |
| `others/jsoncons/doc/ref/toon/key_folding_kind.md` | teilweise unklar | nein | mittlere Priorität | Es ist nicht sicher belegbar, ob die prose-lastigen Aussagen durch einen spezifischen Test oder ein Beispiel direkt abgesichert sind. |
| `others/jsoncons/doc/Examples.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/Pages/index.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/Reference.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/Tutorials/Basics.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/build.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/bson/basic_bson_cursor.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/bson/basic_bson_encoder.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/bson/bson.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/bson/bson_options.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/bson/decode_bson.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/bson/encode_bson.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/cbor/basic_cbor_cursor.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/cbor/basic_cbor_encoder.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/cbor/cbor.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/cbor/cbor_options.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/cbor/decode_cbor.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/cbor/encode_cbor.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/allocator_set.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/basic_json.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/basic_json_cursor.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/basic_json_encoder.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/basic_json_filter.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/basic_json_options.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/basic_json_parser.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/basic_json_reader.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/basic_json_visitor.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/bigint_chars_format.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/bignum_format_kind.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/byte_string.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/byte_string_arg.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/byte_string_chars_format.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/byte_string_view.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/conv_error.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/data-model.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/decode_json.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/encode_json.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/err_handler.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/float_chars_format.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/half_arg.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/half_arg_t.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/indenting.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/allocators.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/array_range.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/as.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/at.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/constructor.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/destructor.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/dump.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/emplace.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/emplace_back.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/erase.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/get_value_or.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/insert.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/insert_or_assign.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/is.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/key_value.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/make_array.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/merge.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/merge_or_update.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/object_range.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/operator=.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/operator_at.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/parse.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/push_back.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/range.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json/try_emplace.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json_array_arg.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json_decoder.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json_error.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json_object_arg.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json_type.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json_type_traits/built-in-specializations.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json_type_traits/custom-specializations.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/json_type_traits/json_type_traits.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/line_split_kind.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/ojson.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/reflect.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/reflect/json_conv_traits.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/reflect/reflect-traits-gen.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/rename_object_key_filter.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/semantic_tag.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/ser_context.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/ser_error.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/spaces_option.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/staj_array_iterator.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/staj_event_type.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/staj_object_iterator.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/utility/bigint.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/utility/uri.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/wjson.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/corelib/wojson.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/csv/basic_csv_cursor.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/csv/basic_csv_encoder.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/csv/basic_csv_options.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/csv/basic_csv_reader.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/csv/csv.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/csv/csv_mapping_kind.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/csv/decode_csv.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/csv/encode_csv.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/csv/quote_style_kind.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/deprecated.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jmespath/jmespath.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jmespath/jmespath_errc.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jmespath/jmespath_error.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jmespath/jmespath_expression.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jmespath/make_expression.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jmespath/search.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpatch/apply_patch.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpatch/from_diff.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpatch/jsonpatch.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpatch/jsonpatch_errc.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpatch/jsonpatch_error.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/basic_json_location.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/basic_path_element.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/basic_path_node.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/functions/abs.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/functions/avg.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/functions/ceil.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/functions/contains.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/functions/ends_with.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/functions/floor.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/functions/keys.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/functions/length.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/functions/max.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/functions/min.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/functions/prod.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/functions/starts_with.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/functions/sum.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/functions/to_number.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/functions/tokenize.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/get.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/json_query.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/json_replace.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/jsonpath.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/jsonpath_error.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/jsonpath_expression.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/jsonpath_expression/evaluate.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/jsonpath_expression/select.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/jsonpath_expression/select_paths.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/jsonpath_expression/update.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/make_expression.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/remove.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/replace.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpath/result_options.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpointer/add.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpointer/add_if_absent.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpointer/basic_json_pointer.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpointer/contains.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpointer/flatten.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpointer/get.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpointer/jsonpointer.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpointer/jsonpointer_error.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpointer/remove.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonpointer/replace.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonschema/custom-error-messages.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonschema/evaluation_options.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonschema/json_schema.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonschema/json_schema/is_valid.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonschema/json_schema/walk.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonschema/make_json_schema.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonschema/validation_error.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonschema/validation_message.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/jsonschema/walk_result.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/mergepatch/apply_merge_patch.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/mergepatch/from_diff.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/mergepatch/mergepatch.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/msgpack/basic_msgpack_cursor.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/msgpack/basic_msgpack_encoder.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/msgpack/decode_msgpack.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/msgpack/encode_msgpack.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/msgpack/msgpack.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/msgpack/msgpack_options.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/toon/encode_toon.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/toon/toon.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/toon/toon_delimiter_kind.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/toon/toon_options.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/ubjson/basic_ubjson_cursor.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/ubjson/basic_ubjson_encoder.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/ubjson/decode_ubjson.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/ubjson/encode_ubjson.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/ubjson/ubjson.md` | korrekt | ja | mittlere Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |
| `others/jsoncons/doc/ref/ubjson/ubjson_options.md` | korrekt | nein | niedrige Priorität | Header-/Symbolabgleich und Test-/Beispielzuordnung vorhanden |

## Korrektheit

- Bestätigt: 183 Dateien.
- Beispiele bestätigt: `others/jsoncons/doc/Examples.md`, `others/jsoncons/doc/Pages/index.md`, `others/jsoncons/doc/Reference.md`, `others/jsoncons/doc/Tutorials/Basics.md`, `others/jsoncons/doc/build.md`, `others/jsoncons/doc/ref/bson/basic_bson_cursor.md`, `others/jsoncons/doc/ref/bson/basic_bson_encoder.md`, `others/jsoncons/doc/ref/bson/bson.md`
- Teilweise unklar: 19 Dateien.
- Beispiele teilweise unklar: `others/jsoncons/doc/Tutorials/Unicode support.md`, `others/jsoncons/doc/ref/corelib/basic_default_json_visitor.md`, `others/jsoncons/doc/ref/corelib/basic_staj_event.md`, `others/jsoncons/doc/ref/corelib/conversion_error.md`, `others/jsoncons/doc/ref/corelib/conversion_result.md`, `others/jsoncons/doc/ref/corelib/json_type_traits/Eigen-Matrix-example.md`, `others/jsoncons/doc/ref/corelib/read_error.md`, `others/jsoncons/doc/ref/corelib/read_result.md`, `others/jsoncons/doc/ref/corelib/reflect/Eigen-Matrix-example.md`, `others/jsoncons/doc/ref/corelib/reflect/uses-allocator-construction-example.md`, `others/jsoncons/doc/ref/corelib/staj_cursor.md`, `others/jsoncons/doc/ref/corelib/write_result.md`
- Widersprüchlich: 9 Dateien.
- Widersprüche mit hoher Relevanz: `others/jsoncons/doc/ref/corelib/byte_string_arg_t.md`, `others/jsoncons/doc/ref/corelib/json_array_arg_t.md`, `others/jsoncons/doc/ref/corelib/json_object_arg_t.md`, `others/jsoncons/doc/ref/jsonpath/flatten.md`, `others/jsoncons/doc/ref/jsonpointer/unflatten_options.md`, `others/jsoncons/doc/ref/jsonschema/json_schema/validate.md`, `others/jsoncons/doc/ref/jsonschema/schema_error.md`, `others/jsoncons/doc/ref/jsonschema/schema_version.md`, `others/jsoncons/doc/ref/toon/decode_toon.md`
- Wahrscheinlich veraltet: 2 Dateien.
- Wahrscheinlich veraltete Referenzen: `others/jsoncons/doc/ref/jsonschema/json_validator.md`, `others/jsoncons/doc/ref/jsonschema/validation_output.md`

## Performance

- Hohe Priorität: `others/jsoncons/doc/ref/jsonpath/flatten.md`, `others/jsoncons/doc/ref/jsonpointer/unflatten_options.md`, `others/jsoncons/doc/ref/jsonschema/json_schema/validate.md`, `others/jsoncons/doc/ref/toon/decode_toon.md`
- Mittlere Priorität: `others/jsoncons/doc/ref/corelib/json_type_traits/Eigen-Matrix-example.md`, `others/jsoncons/doc/ref/corelib/reflect/Eigen-Matrix-example.md`, `others/jsoncons/doc/ref/corelib/reflect/uses-allocator-construction-example.md`, `others/jsoncons/doc/ref/jsonschema/jsonschema.md`, `others/jsoncons/doc/ref/bson/basic_bson_cursor.md`, `others/jsoncons/doc/ref/bson/basic_bson_encoder.md`, `others/jsoncons/doc/ref/bson/bson.md`, `others/jsoncons/doc/ref/bson/decode_bson.md`, `others/jsoncons/doc/ref/bson/encode_bson.md`, `others/jsoncons/doc/ref/cbor/basic_cbor_cursor.md`, `others/jsoncons/doc/ref/cbor/basic_cbor_encoder.md`, `others/jsoncons/doc/ref/cbor/cbor.md`, `others/jsoncons/doc/ref/cbor/decode_cbor.md`, `others/jsoncons/doc/ref/cbor/encode_cbor.md`, `others/jsoncons/doc/ref/corelib/basic_json.md`, `others/jsoncons/doc/ref/corelib/basic_json_cursor.md`, `others/jsoncons/doc/ref/corelib/basic_json_encoder.md`, `others/jsoncons/doc/ref/corelib/basic_json_parser.md`, `others/jsoncons/doc/ref/corelib/basic_json_reader.md`, `others/jsoncons/doc/ref/corelib/decode_json.md`
- Bereichsübergreifend auffällig: wiederholte Expression-Kompilierung in JSONPath/JMESPath, wiederholte Reader/Decoder/Encoder-Initialisierung in Convenience-APIs und wiederholtes Pointer-Parsen in string-basierten JSON-Pointer-Overloads.

## Verarbeitung

- Hohe Priorität: `others/jsoncons/doc/ref/jsonpointer/unflatten_options.md`, `others/jsoncons/doc/ref/jsonschema/json_schema/validate.md`, `others/jsoncons/doc/ref/toon/decode_toon.md`
- Mittlere Priorität: `others/jsoncons/doc/ref/jsonschema/jsonschema.md`, `others/jsoncons/doc/ref/toon/key_folding_kind.md`, `others/jsoncons/doc/ref/bson/basic_bson_cursor.md`, `others/jsoncons/doc/ref/bson/basic_bson_encoder.md`, `others/jsoncons/doc/ref/bson/bson.md`, `others/jsoncons/doc/ref/bson/decode_bson.md`, `others/jsoncons/doc/ref/bson/encode_bson.md`, `others/jsoncons/doc/ref/cbor/basic_cbor_cursor.md`, `others/jsoncons/doc/ref/cbor/basic_cbor_encoder.md`, `others/jsoncons/doc/ref/cbor/cbor.md`, `others/jsoncons/doc/ref/cbor/decode_cbor.md`, `others/jsoncons/doc/ref/cbor/encode_cbor.md`, `others/jsoncons/doc/ref/corelib/basic_json_cursor.md`, `others/jsoncons/doc/ref/corelib/basic_json_parser.md`, `others/jsoncons/doc/ref/corelib/basic_json_reader.md`, `others/jsoncons/doc/ref/corelib/decode_json.md`, `others/jsoncons/doc/ref/corelib/encode_json.md`, `others/jsoncons/doc/ref/csv/basic_csv_cursor.md`, `others/jsoncons/doc/ref/csv/basic_csv_encoder.md`, `others/jsoncons/doc/ref/csv/basic_csv_reader.md`
- Niedrige Priorität: `others/jsoncons/doc/ref/bson/bson_options.md`, `others/jsoncons/doc/ref/cbor/cbor_options.md`, `others/jsoncons/doc/ref/csv/basic_csv_options.md`, `others/jsoncons/doc/ref/csv/csv_mapping_kind.md`, `others/jsoncons/doc/ref/csv/quote_style_kind.md`, `others/jsoncons/doc/ref/msgpack/msgpack_options.md`, `others/jsoncons/doc/ref/toon/toon_delimiter_kind.md`, `others/jsoncons/doc/ref/toon/toon_options.md`, `others/jsoncons/doc/ref/ubjson/ubjson_options.md`

## Doku-Anpassungen

- Hohe Priorität: `others/jsoncons/doc/ref/corelib/byte_string_arg_t.md`, `others/jsoncons/doc/ref/corelib/json_array_arg_t.md`, `others/jsoncons/doc/ref/corelib/json_object_arg_t.md`, `others/jsoncons/doc/ref/jsonpath/flatten.md`, `others/jsoncons/doc/ref/jsonpointer/unflatten_options.md`, `others/jsoncons/doc/ref/jsonschema/json_schema/validate.md`, `others/jsoncons/doc/ref/jsonschema/json_validator.md`, `others/jsoncons/doc/ref/jsonschema/schema_error.md`, `others/jsoncons/doc/ref/jsonschema/schema_version.md`, `others/jsoncons/doc/ref/jsonschema/validation_output.md`, `others/jsoncons/doc/ref/toon/decode_toon.md`
- Mittlere Priorität: `others/jsoncons/doc/Tutorials/Unicode support.md`, `others/jsoncons/doc/ref/corelib/basic_default_json_visitor.md`, `others/jsoncons/doc/ref/corelib/basic_staj_event.md`, `others/jsoncons/doc/ref/corelib/conversion_error.md`, `others/jsoncons/doc/ref/corelib/conversion_result.md`, `others/jsoncons/doc/ref/corelib/json_type_traits/Eigen-Matrix-example.md`, `others/jsoncons/doc/ref/corelib/read_error.md`, `others/jsoncons/doc/ref/corelib/read_result.md`, `others/jsoncons/doc/ref/corelib/reflect/Eigen-Matrix-example.md`, `others/jsoncons/doc/ref/corelib/reflect/uses-allocator-construction-example.md`, `others/jsoncons/doc/ref/corelib/staj_cursor.md`, `others/jsoncons/doc/ref/corelib/write_result.md`, `others/jsoncons/doc/ref/jsonpath/grammar.md`, `others/jsoncons/doc/ref/jsonpath/jsoncons-jsonpath-abnf.md`, `others/jsoncons/doc/ref/jsonpath/jsonpath_grammer.md`, `others/jsoncons/doc/ref/jsonpath/path_node_kind.md`, `others/jsoncons/doc/ref/jsonpointer/jsonpointer_errc.md`, `others/jsoncons/doc/ref/jsonschema/jsonschema.md`, `others/jsoncons/doc/ref/toon/key_folding_kind.md`, `others/jsoncons/doc/ref/bson/basic_bson_cursor.md`, `others/jsoncons/doc/ref/bson/basic_bson_encoder.md`, `others/jsoncons/doc/ref/bson/bson.md`, `others/jsoncons/doc/ref/bson/decode_bson.md`, `others/jsoncons/doc/ref/bson/encode_bson.md`, `others/jsoncons/doc/ref/cbor/basic_cbor_cursor.md`, `others/jsoncons/doc/ref/cbor/basic_cbor_encoder.md`, `others/jsoncons/doc/ref/cbor/cbor.md`, `others/jsoncons/doc/ref/cbor/decode_cbor.md`, `others/jsoncons/doc/ref/cbor/encode_cbor.md`, `others/jsoncons/doc/ref/corelib/basic_json.md`
- Niedrige Priorität: `others/jsoncons/doc/Examples.md`, `others/jsoncons/doc/Pages/index.md`, `others/jsoncons/doc/Reference.md`, `others/jsoncons/doc/Tutorials/Basics.md`, `others/jsoncons/doc/build.md`, `others/jsoncons/doc/ref/bson/bson_options.md`, `others/jsoncons/doc/ref/cbor/cbor_options.md`, `others/jsoncons/doc/ref/corelib/allocator_set.md`, `others/jsoncons/doc/ref/corelib/basic_json_filter.md`, `others/jsoncons/doc/ref/corelib/basic_json_options.md`, `others/jsoncons/doc/ref/corelib/basic_json_visitor.md`, `others/jsoncons/doc/ref/corelib/bigint_chars_format.md`, `others/jsoncons/doc/ref/corelib/bignum_format_kind.md`, `others/jsoncons/doc/ref/corelib/byte_string.md`, `others/jsoncons/doc/ref/corelib/byte_string_arg.md`, `others/jsoncons/doc/ref/corelib/byte_string_chars_format.md`, `others/jsoncons/doc/ref/corelib/byte_string_view.md`, `others/jsoncons/doc/ref/corelib/conv_error.md`, `others/jsoncons/doc/ref/corelib/data-model.md`, `others/jsoncons/doc/ref/corelib/err_handler.md`, `others/jsoncons/doc/ref/corelib/float_chars_format.md`, `others/jsoncons/doc/ref/corelib/half_arg.md`, `others/jsoncons/doc/ref/corelib/half_arg_t.md`, `others/jsoncons/doc/ref/corelib/indenting.md`, `others/jsoncons/doc/ref/corelib/json/allocators.md`, `others/jsoncons/doc/ref/corelib/json/array_range.md`, `others/jsoncons/doc/ref/corelib/json/as.md`, `others/jsoncons/doc/ref/corelib/json/at.md`, `others/jsoncons/doc/ref/corelib/json/constructor.md`, `others/jsoncons/doc/ref/corelib/json/destructor.md`
- Konkrete Schwerpunkte: Include-Pfade korrigieren (`schema_error`, `schema_version`, `decode_toon`, `flatten`, `unflatten_options`, `*_arg_t`), veraltete Seiten (`validation_output`, `json_validator`) bereinigen und fehlerhafte Beispiele (`json_schema/validate`, `jsonschema`) reparieren.
