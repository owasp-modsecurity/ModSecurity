# Faktenprüfung: bekannte Stolperfallen und Quellenkonflikte

## Gültigkeit und Verwendung

Geprüfter Quellstand: `owasp-modsecurity/ModSecurity`, `v3/master`, **`effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`**.
Diese Übersicht beschreibt Unterschiede zwischen Kommentaren, Beispielen, Buildregeln und Implementierung. Sie behauptet keine Sicherheitslücken und keine ausgeführten Laufzeittests.
Die Absätze **Prüffrage** sind eigene Empfehlungen für neue Chats. Die Absätze **Befund** beruhen auf den jeweils verlinkten Quellen dieses Commits.

## 1. Der README-Ausschnitt entspricht nicht überall der aktuellen C++-API

**Befund:** Das README-Beispiel verwendet `ModSecurity::Rules`, einen Transaktionskonstruktor mit zwei Argumenten, `processConnection()` mit nur einer Adresse und `intervention()` ohne Ausgabeparameter. Die aktuellen Header deklarieren `modsecurity::RulesSet`, Konstruktoren mit `logCbData`, vier Verbindungsparameter und `intervention(ModSecurityIntervention *)`. Quellen: [README-Beispiel][readme-api], [RulesSet][rules-api], [Transaktionskonstruktor und Verbindung][transaction-args], [Intervention][intervention-api].

**Prüffrage:** Wurde eine verwendete Signatur direkt im Header und in der Implementierung geprüft, oder nur aus dem README übernommen? Für C++-Beispiele die tatsächlichen Quellen unter `examples/` und [CPP_GUIDE.md](CPP_GUIDE.md) heranziehen.

## 2. Die Testprogramme liegen unter `test/`

**Befund:** Im Kompilierungsabschnitt des README steht `tests/`. Das oberste Makefile nimmt tatsächlich `test` in den Build auf; dessen Makefile definiert `unit_tests` und `regression_tests`. Quellen: [README-Schreibweise][readme-tests], [Build-Unterverzeichnisse][root-make], [Unit-Testziel][unit-target], [Regressionstestziel][regression-target].

**Prüffrage:** Stimmen Programmname, Testdatei und Arbeitsverzeichnis? Die direkten Beispielaufrufe in [BUILD_AND_TEST.md](BUILD_AND_TEST.md) starten innerhalb von `test/`.

## 3. Der belegte Sprachstandard ist C++17

**Befund:** `configure.ac` verlangt `AX_CXX_COMPILE_STDCXX(17, noext, mandatory)`. Der Windows-Build verlangt ebenfalls C++17. Der Standard wird hier durch die Builddefinitionen belegt, nicht aus dem Alter des Projekts oder einzelner Syntaxstellen geschätzt. Quellen: [Unix-Vorgabe][cpp-standard], [Windows-Vorgabe][cpp-standard-win].

**Prüffrage:** Passt eine vorgeschlagene Sprachkonstruktion zu diesem Stand und zum tatsächlich verwendeten Compiler? Bei einem anderen Commit erneut dessen Builddefinitionen lesen.

## 4. YAJL: README-Anforderung und Unix-Buildlogik unterscheiden sich

**Befund:** Das README nennt YAJL verpflichtend. Das YAJL-Makro verwendet dagegen die generische Bibliothekserkennung, deren automatischer Suchpfad bei Fehlen weiterläuft. Nur eine ausdrücklich verpflichtende Anfrage führt dort zum Konfigurationsfehler. `TEST_UTILITIES` wird nur bei `YAJL_FOUND=1` aktiviert. Quellen: [README][readme-dependencies], [YAJL-Makro][yajl-macro], [Erkennungslogik][find-library], [Testbedingung][test-condition].

**Prüffrage:** Wurde YAJL erkannt und wurden die Testprogramme tatsächlich gebaut? Für einen beabsichtigten Testbuild ist `--with-yajl=yes` die in dieser Arbeitsdokumentation gewählte Variante. Ein erfolgreicher Bibliotheksbuild allein belegt keine vorhandenen Testprogramme.

## 5. Exitcode 0 kann ohne ausgeführte Regressionen entstehen

**Befund:** Bei `NO_LOGS` gibt `regression_tests` eine Meldung über fehlende Logging-Unterstützung aus und kehrt unmittelbar mit `0` zurück. Dieser Pfad liegt vor Argumentauswertung und Testladen. Quelle: [NO_LOGS-Zweig][no-logs].

**Prüffrage:** Welche Fälle wurden wirklich ausgeführt? Neben dem Exitcode auch Fallzahlen, Skips, deaktivierte Fälle und die Meldungen des Runners protokollieren. Aus diesem Befund folgt keine Aussage, dass andere Buildvarianten ihre Tests bestehen.

## 6. Windows-LMDB ist im CMake-Standard ausgeschaltet

**Befund:** Die Windows-README zählt LMDB zu den standardmäßig aktivierten Features. `build/win32/CMakeLists.txt` setzt `WITH_LMDB` jedoch auf `OFF`; Lua, libxml2, MaxMind und curl stehen dort auf `ON`. Quellen: [README-Featureliste][windows-readme], [CMake-Optionen][windows-options].

**Prüffrage:** Welche CMake-Optionen wurden tatsächlich gesetzt? Die Existenz einer Conan-Abhängigkeit oder die allgemeine README-Liste ist kein Nachweis dafür, dass die betreffende Funktion im Binary aktiv ist.

## 7. Erkannt, abgewiesen und wirkungslos sind verschiedene Zustände

**Befund:** Die Grammatik behandelt die folgenden bekannten Direktiven unterschiedlich. Diese Tabelle beschreibt die jeweiligen Parserzweige, keine vollständige v2/v3-Kompatibilitätsliste.

| Direktive | Belegtes Verhalten im Grammatikzweig |
| --- | --- |
| `SecRequestBodyInMemoryLimit` | Meldet fehlende Unterstützung und löst `YYERROR` aus. [Quelle][memory-limit] |
| `SecPcreMatchLimitRecursion` | Hat dort keine ausführende Aktion; die Fehlermeldung ist auskommentiert. Das benachbarte `SecPcreMatchLimit` setzt dagegen eine Eigenschaft. [Quelle][recursion-limit] |
| `SecTmpDir`, `SecDataDir`, `SecStatusEngine` | Die gezeigten Zweige nehmen keine entsprechende Wertzuweisung vor; Fehlermeldungen sind auskommentiert. [Quelle][compat-directives] |

**Prüffrage:** Was geschieht nach Erkennung des Tokens? Grammatikaktion, gespeicherte Eigenschaft und Laufzeitzugriff verfolgen. Weder „alle unbekannten Wirkungen erzeugen Fehler“ noch „erfolgreich geparst bedeutet wirksam“ ist hier eine belastbare Schlussfolgerung.

## 8. Eine Operator-Datei beweist keine Zulassung in SecRules

**Befund:** `Operator::instantiate()` kennt `gsblookup`, `rsub` und `validatehash`. Die SecRules-Grammatik weist `@gsbLookup`, `@rsub` und `@validateHash` aber ausdrücklich mit `OPERATOR_NOT_SUPPORTED` zurück; das Makro meldet einen Fehler und führt `YYERROR` aus. Die jeweiligen `evaluate()`-Dateien enthalten Implementierungs-TODOs und geben unmittelbar `true` zurück. Quellen: [Factory][operator-factory], [Grammatikzweige][operator-grammar], [Fehlermakro][unsupported-macro], [GsbLookup][gsb-stub], [Rsub][rsub-stub], [ValidateHash][hash-stub].

**Prüffrage:** Wurde der normale SecRules-Ladepfad geprüft? Klassenexistenz oder Factory-Eintrag allein rechtfertigen keine Aussage, der Operator sei als Regeloperator verfügbar. Daraus wird hier auch keine ausnutzbare Sicherheitswirkung abgeleitet.

## 9. Interne Phasenwerte sind keine SecRules-Phasennummern

**Befund:** Das Enum enthält `ConnectionPhase`, `UriPhase`, `RequestHeadersPhase` und weitere Phasen. `Phase::init()` übersetzt `phase:1` nach `RequestHeadersPhase`, dessen interner Wert `2` ist. `phase:0` wird nach `ConnectionPhase` übersetzt. `processURI()` befüllt Variablen und ruft im untersuchten Funktionskörper keine Auswertung von `UriPhase` auf. Quellen: [Enum][phase-enum], [Zuordnung][phase-action], [URI-Funktion][uri].

**Prüffrage:** Stammt eine Zahl aus SecRules, einem internen Enum, einer API oder einer Logdarstellung? Die vollständige Zuordnung steht in [ARCHITECTURE.md](ARCHITECTURE.md).

## 10. Dieses Repository beschreibt die Bibliothek

**Befund:** Das Upstream-README grenzt libmodsecurity von den separat gepflegten Webserver-Connectors ab. Die Bibliothek verarbeitet vom Verwender übergebene HTTP-Daten. Die Beispiele und öffentlichen Transaktionsmethoden belegen eine API-Nutzung, aber keine vollständige Integration einer bestimmten Nginx-, Apache- oder IIS-Version. Quellen: [Projektabgrenzung][scope], [Transaktions-API][transaction-public].

**Prüffrage:** Liegt die untersuchte Wirkung in der Bibliothek oder im Connector? Für einen externen Connector dessen eigene Quelle und Version anfordern beziehungsweise im autorisierten Umfang untersuchen. Eine Bibliotheksintervention nicht ohne weiteren Beleg mit einer tatsächlich versendeten HTTP-Antwort gleichsetzen.

## 11. Submodule-Versionen stammen aus Gitlinks

**Befund:** `.gitmodules` nennt Pfade und URLs. Die folgenden Commit-Pins wurden aus dem Git-Tree des fixierten Hauptrepository-Commits gelesen, nicht aus Zweignamen oder README-Beispielausgaben. Quellen: [.gitmodules][gitmodules], [fixierter Upstream-Tree][source-tree].

| Submodulpfad | Gitlink-Commit |
| --- | --- |
| `bindings/python` | `bc625d5bb0bac6a64bcce8dc9902208612399348` |
| `others/libinjection` | `211782219663f889f471650150df12b623c5766e` |
| `others/mbedtls` | `0fe989b6b514192783c469039edd325fd0989806` |
| `test/test-cases/secrules-language-tests` | `f73c73027ef49ccf99c7911744929a71d15ff419` |

**Prüffrage:** Stimmen Checkout und [upstream-lock.json](upstream-lock.json) mit diesen Gitlinks überein? Die Zuordnung ist mit `git ls-tree -r effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729` nachvollziehbar. Der Pin allein belegt weder initialisierte Submodule noch eine inhaltliche Prüfung ihrer Dateien.

## 12. Das Scanner-Image erzwingt keinen grünen Testlauf

**Befund:** `.oss-scanner/Dockerfile` bricht bei Fehlern in seiner Buildkette ab, behandelt `make check` danach aber mit `|| echo "WARNING: make check reported failures"`. Ein Fehler dieses Testaufrufs muss deshalb nicht den Image-Build fehlschlagen lassen. Der Kommentar vor der Testzeile weist ebenfalls auf diese Behandlung hin. Quelle: [Build- und Testschritte][scanner-docker].

**Prüffrage:** Ist „Image gebaut“ oder „Tests bestanden“ belegt? Testausgaben und den ursprünglichen Teststatus getrennt auswerten; den erfolgreichen Docker-Build nicht als bestandene Testsuite protokollieren.

## Pflege dieser Liste

**Eigene Empfehlung:** Bei einem neueren Quellstand die betroffenen Implementierungen erneut lesen. Einträge nur mit nachvollziehbarer Quelle ändern oder entfernen; keine erledigten Konflikte in neuen Chats weitertragen. Für zusätzliche Beobachtungen immer Befund, Quelle, praktische Bedeutung und verbleibende Unsicherheit festhalten.

## Quellen am fixierten Commit

[readme-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L233-L255
[rules-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/rules_set.h#L38-L77
[transaction-args]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L335-L350
[intervention-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L398-L403
[readme-tests]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L53-L54
[root-make]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/Makefile.am#L1-L17
[unit-target]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/Makefile.am#L29-L35
[regression-target]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/Makefile.am#L93-L99
[cpp-standard]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L49-L63
[cpp-standard-win]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/CMakeLists.txt#L102-L104
[readme-dependencies]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L101-L106
[yajl-macro]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/yajl.m4#L10-L11
[find-library]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/msc_find_lib.m4#L48-L97
[test-condition]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L313-L324
[no-logs]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/regression/regression.cc#L437-L442
[windows-readme]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/README.md#L64-L74
[windows-options]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/CMakeLists.txt#L1-L9
[memory-limit]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1655-L1663
[recursion-limit]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1696-L1707
[compat-directives]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1748-L1785
[operator-factory]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/operators/operator.cc#L148-L181
[operator-grammar]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L992-L1026
[unsupported-macro]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L298-L300
[gsb-stub]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/operators/gsblookup.cc#L26-L32
[rsub-stub]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/operators/rsub.cc#L26-L32
[hash-stub]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/operators/validate_hash.cc#L25-L31
[phase-enum]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/modsecurity.h#L108-L170
[phase-action]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/phase.cc#L25-L66
[uri]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L337-L443
[scope]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L21-L46
[transaction-public]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L335-L419
[gitmodules]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.gitmodules#L1-L12
[source-tree]: https://github.com/owasp-modsecurity/ModSecurity/tree/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729
[scanner-docker]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.oss-scanner/Dockerfile#L29-L37
