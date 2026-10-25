# C++ und C im ModSecurity-Quellcode verstehen

## Grundlage und Ziel

Dieser Leitfaden erklärt die im Repository tatsächlich verwendeten Sprachmittel und API-Grenzen anhand von `owasp-modsecurity/ModSecurity` bei Commit **`effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`**, Branch `v3/master`. Er ist eine Lese- und Änderungshilfe, kein vollständiger C++-Sprachkurs und kein Nachweis eines erfolgreichen Builds. Alle verlinkten Implementierungen sind auf denselben Quellstand festgelegt.

Für den Ablauf der Bibliothek zuerst [ARCHITECTURE.md](ARCHITECTURE.md) lesen. Hier geht es anschließend darum, Funktionssignaturen, Datenbesitz, Rückgaben und bestehende Codeformen korrekt zu verstehen.

## 1. Welche Sprache an welcher Stelle arbeitet

| Bereich | Belegter Einsatz | Bedeutung für die Arbeit |
| --- | --- | --- |
| C++17 | `configure.ac` verlangt `AX_CXX_COMPILE_STDCXX(17, noext, mandatory)`; der Windows-CMake-Build setzt `CMAKE_CXX_STANDARD 17`. | Bei Sprachmerkmalen und Compilerfehlern ist C++17 der belegte Ausgangspunkt. [Autotools][cxx17], [Windows][cxx17-win] |
| C++-Klassen und `.cc`-Dateien | `ModSecurity`, `RulesSet` und `Transaction` sind in öffentlichen Headern deklariert und in `src/` implementiert. | Header zeigen den Aufrufvertrag, Implementierungen den tatsächlichen Ablauf. [Engine-Header][engine-api], [Ruleset-Header][rules-api], [Transaktions-Header][transaction-api] |
| C-Schnittstelle | `msc_*`-Funktionen werden mit `extern "C"` definiert; C-Verwender erhalten opaque Typen. | C-Aufrufer greifen über Funktionen auf die C++-Objekte zu. [Opaque Typen][c-types], [C-Wrapper][c-construct] |
| SecRules-Konfiguration | `modsecurity.conf-recommended` enthält unter anderem `SecRuleEngine`, `SecRule` und Aktionslisten. | Dies ist Eingabesprache der Engine; eine Regeländerung ist nicht automatisch eine C++-Änderung. [Konfiguration][recommended] |
| Scanner und Parser | `src/parser/Makefile.am` führt `seclang-parser.yy` und `seclang-scanner.ll` als Quellen auf und erzeugt zugehörige Dateien. | Eine Sprachänderung beginnt bei Grammatik beziehungsweise Scanner und den erzeugten Objekten. [Parser-Build][parser-build], [Driver][driver-rules] |

In der C++-API heißt der Namespace **`modsecurity`**, beispielsweise `modsecurity::RulesSet`. Das `::` ordnet den Typ einem Namensraum zu. Das README enthält an einer Stelle ältere Bezeichner wie `ModSecurity::Rules`; die aktuellen Header und ausführbaren Beispielquellen zeigen dagegen `modsecurity::RulesSet`. Diese Beispiele im README deshalb nicht ungeprüft als aktuelle Signaturen übernehmen. Quellen: [Aktuelle Deklarationen][rules-api], [aktuelles Beispiel][example-setup], [abweichendes README-Beispiel][readme-example].

## 2. Header, Implementierung und öffentliche Schnittstelle

Eine Deklaration im Header beschreibt Namen, Parametertypen und Rückgabetyp. Der zugehörige Methodenrumpf beschreibt, was beim Aufruf tatsächlich geschieht. Für `RulesSet::loadFromUri(const char *uri)` steht die Deklaration in `headers/modsecurity/rules_set.h`; der Rumpf in `src/rules_set.cc` verwendet `Parser::Driver`, übernimmt Parsefehler und vereinigt die geladenen Daten. Ein Kommentar kann beim Einstieg helfen, ersetzt diesen Abgleich aber nicht. Quellen: [Deklaration][rules-api], [Ladeimplementierung][load].

Viele Header unter `headers/modsecurity/` werden laut `src/Makefile.am` installiert. Änderungen dort können folglich Bibliotheksverwender betreffen. Die ABI ist die binäre Schnittstelle bereits kompilierter Verwender; der Code bewahrt an einzelnen Stellen alte Signaturen ausdrücklich aus ABI-Gründen, etwa bei `AnchoredSetVariable::resolveRegularExpression()`. **Arbeitsregel für neue Änderungen:** Signaturen, Klassenlayout und Ownership in solchen Headern nicht als rein lokale Implementierungsdetails behandeln. Quellen: [installierte Header][installed-headers], [ABI-Kommentar][abi].

## 3. Klassen, Konstruktoren und Lebensdauer

`ModSecurity` und `Transaction` besitzen Konstruktoren und Destruktoren. Der Konstruktor stellt den Anfangszustand her; der Destruktor gibt verwaltete Ressourcen frei. Beide Klassen löschen Kopieroperationen ausdrücklich mit `= delete`. Das verhindert beispielsweise, dass eine Transaktion durch eine normale Kopie mit denselben Ressourcenzeigern vervielfältigt wird. Quellen: [ModSecurity][engine-api], [Transaction][transaction-api].

Ein vorhandenes Beispiel für automatische Objektlebensdauer ist `test/common/modsecurity_test_context.h`: Der Kontext enthält `ModSecurity` und `RulesSet` als direkte Member; `create_transaction()` erzeugt eine Transaktion mit Zeigern auf diese Member. Die Methode liefert unmittelbar ein neu konstruiertes Objekt zurück. Dieser Code ist zusammen mit der geforderten C++17-Version zu lesen; ein gelöschter Kopierkonstruktor ist kein Anlass, zur Behebung eines Fehlers ohne Ursachenprüfung eine Kopierfunktion hinzuzufügen. Quellen: [Testkontext][test-context], [C++17-Vorgabe][cxx17].

### Wer besitzt welche Ressource?

Die folgende Tabelle unterscheidet tatsächliche Besitzverwaltung von bloßen Referenzen. Die Lebensdauerregeln für geliehene Zeiger sind aus Konstruktoren, Destruktoren und Aufrufern abgeleitet.

| Ressource | Erzeugung beziehungsweise Übergabe | Freigabe oder Lebensdauer |
| --- | --- | --- |
| Engine-Collections | `ModSecurity` erzeugt Collection-Backends mit `new`. | `ModSecurity::~ModSecurity()` löscht sie. [Quelle][engine-life] |
| `Transaction::m_ms`, `m_rules` | Der Aufrufer übergibt Engine und Ruleset; der Konstruktor speichert die Zeiger. | Die Transaktion löscht sie nicht. Beide müssen während ihrer Nutzung gültig bleiben. [Quelle][transaction-life] |
| JSON-/XML-Prozessoren der Transaktion | Der Konstruktor erzeugt sie abhängig von Build-Makros. | Der Destruktor löscht sie unter denselben Makros. [Quelle][transaction-life] |
| Transaktions-Collection `TX` | `Collections` erzeugt `InMemoryPerProcess("TX")`. | `Collections::~Collections()` löscht sie; die übrigen Collection-Zeiger sind übergeben. [Quelle][collections] |
| Regeln in `Rules` | `std::vector<std::shared_ptr<Rule>>`. | Die Smart Pointer verwalten die gehaltenen Regelobjekte. [Quelle][rules-storage] |
| Neues Parser-Regelobjekt | `Driver::addSecRule(std::unique_ptr<RuleWithActions>)`. | Übergang in `std::shared_ptr` für Phasenspeicherung beziehungsweise Kettenkind. [Driver][driver-rules], [Kettenfelder][chain-fields] |
| Operator einer `RuleWithOperator` | Der Konstruktor übernimmt `Operator *`. | `RuleWithOperator::~RuleWithOperator()` löscht ihn sowie übernommene Variablenobjekte. [Quelle][operator-life] |
| Transformations- und Aktionsobjekte | `RuleWithActions` hält mehrere Zeiger und Zeigercontainer. | Der Destruktor löscht die dort verwalteten Objekte. [Quelle][actions-life] |

`std::unique_ptr` drückt alleinigen Besitz aus; `std::shared_ptr` ermöglicht gemeinsamen Besitz. Ein roher Zeiger allein sagt nicht, wer freigeben muss. Das Repository verwendet alle drei Formen. Eine mechanische Umstellung sämtlicher roher Zeiger würde daher die tatsächlichen Übergabegrenzen verfehlen. Erst Konstruktion, Speicherung, Weitergabe und Freigabe zusammen lesen. Belegte Beispiele dafür stehen in der Tabelle; diese Empfehlung ist eine daraus abgeleitete Arbeitsweise.

### `const` bedeutet nicht „muss nie freigegeben werden“

`AnchoredSetVariable::resolve()` legt mit `new VariableValue(...)` Kopien an und schreibt Zeiger darauf in einen `std::vector<const VariableValue *>`. Die Regelverarbeitung löscht diese Elemente nach der Auswertung ausdrücklich. `const` beschränkt hier die Änderung des Objekts über diesen Zeiger; es beschreibt nicht den Ressourcenbesitz. `resolveFirst()` hat einen anderen Vertrag und liefert einen `std::unique_ptr<std::string>`. Quellen: [Erzeugung in resolve()][resolve], [Verwendung und delete][evaluate], [resolveFirst()][resolve-first].

Bei `const std::string&` handelt es sich um eine Referenz auf eine vorhandene Zeichenkette. So geben `VariableValue::getValue()` und `getKey()` Referenzen auf Member zurück. Nach dem Löschen des `VariableValue` kann eine solche Referenz nicht weiterverwendet werden. Diese Lebensdauer ist bei Umbauten der Auswertungsschleife mitzudenken. Quellen: [Getter und Member][variable-value], [Löschstellen][evaluate].

## 4. Strings, Bytes und Längen

Die API unterscheidet mehrere Arten von Texteingaben. `processURI()` nimmt nullterminierte C-Zeichenketten entgegen. Die Header-Überladung ohne Längen berechnet Größen über `strlen()`. Die Überladung mit `key_n` und `value_n` übernimmt dagegen genau diese Längen in `std::string::assign()`. C-Aufrufer finden dieselbe Unterscheidung als `msc_add_request_header()` gegenüber `msc_add_n_request_header()` und entsprechend für Response-Header. Quellen: [URI-Verarbeitung][uri], [Header-Überladungen][header-lengths], [C-Deklarationen][c-trans-api].

| Eingabeform | Verhalten im Code | Konsequenz für einen Aufrufer |
| --- | --- | --- |
| `const char *` für URI, Methode und HTTP-Version | Wird als C-Zeichenkette verwendet. | Gültige Nullterminierung und Lebensdauer für den Aufruf sicherstellen. [Quelle][uri] |
| Header-Zeiger ohne Länge | Verwendet `strlen()`. | Nicht für einen beliebigen, nicht nullterminierten Netzwerk-Ausschnitt verwenden. [Quelle][header-lengths] |
| Header-Zeiger mit `size_t`-Längen | Erstellt Stringwerte mit expliziten Längen. | Tatsächliche, gültige Puffergrenzen übergeben. [Quelle][header-lengths] |
| Body-Zeiger mit Länge | `appendRequestBody()` schreibt `len` Bytes beziehungsweise den verbleibenden Limitanteil in den Stream. | Die übergebene Bytezahl ist maßgeblich; den Returnwert und die Intervention separat prüfen. [Quelle][request-append] |

`reinterpret_cast<const char *>(buf)` im Append-Pfad ändert die Sicht auf den Zeiger für den Stream-Aufruf; es führt keine URL-, Unicode- oder Zeichensatzdekodierung aus. Die URI-Dekodierung findet ausdrücklich über `utils::uri_decode()` in `processURI()` statt. Änderungen an der Interpretation von Eingaben müssen daher an der betreffenden Verarbeitung und nicht nur an einem Cast erklärt werden. Quellen: [Append-Pfad][request-append], [URI-Dekodierung][uri].

`processURI()` hält sowohl rohe als auch abgeleitete URI-Daten vor und berechnet Offsets. `VariableValue` kann Herkunftsangaben enthalten, die später in Match-Referenzen eingehen. Ein vermeintlich einfacher Wechsel von einer rohen zur dekodierten Zeichenkette kann deshalb Vergleichswerte, Variablennamen und Diagnosepositionen gleichzeitig betreffen. Dies ist eine aus dem Datenfluss abgeleitete Prüffrage, keine Behauptung über einen bestimmten Fehler. Quellen: [URI][uri], [VariableValue][variable-value], [Match-Verarbeitung][evaluate].

## 5. Vererbung und virtuelle Aufrufe

`Rule` definiert virtuelle `evaluate()`-Methoden und einen virtuellen Destruktor. `RuleWithActions` erweitert diesen Typ; `RuleWithOperator` ergänzt den Operator und die Zielvariablen. `RulesSet::evaluate()` kann so über einen allgemeinen Regelzeiger die konkrete Auswertung aufrufen. Der Code benutzt außerdem `dynamic_cast`, um beispielsweise zu prüfen, ob eine Regel Aktionsmetadaten besitzt. Quellen: [Rule-Basisklasse][rule-base], [RuleWithActions][chain-fields], [konkreter Operator][operator-life], [Phasenschleife][rules-evaluate].

Bei einer neuen Regelart sind damit mehrere getrennte Stellen relevant: Wie erzeugt der Parser das Objekt? In welche Phase gelangt es? Welche virtuelle Auswertung läuft? Wem gehören die beteiligten Objekte? Ein neuer Name in der Grammatik allein beantwortet diese Fragen nicht. Die vorhandenen Aufrufstellen sind der Ausgangspunkt für eine solche Änderung. Quellen: [Driver][driver-rules], [Phasenschleife][rules-evaluate], [Operator-Auswertung][evaluate].

## 6. Rückgabewerte nicht vereinheitlichen

Das Repository hat keinen einzigen universellen Bedeutungsvertrag für `int`-Rückgaben. Die Bedeutung muss pro Funktion gelesen werden. Insbesondere ist ein erfolgreicher API-Aufruf keine Aussage, dass die Anfrage erlaubt ist oder dass eine bestimmte Regelphase tatsächlich ausgeführt wurde.

| Funktion/Pfad | Belegtes Ergebnis | Typischer Lesefehler |
| --- | --- | --- |
| `RulesSet::loadFromUri()`, `load()` | `-1` bei den dort behandelten Fehlern; ansonsten Anzahl übernommener Regeln. [Quelle][load] | `0` pauschal als Fehler behandeln oder die Anzahl mit einer Freigabeentscheidung verwechseln. |
| `processRequestHeaders()` | Liefert auch bei deaktivierter Engine `true`/`1`. [Quelle][request-phase] | Aus `1` ableiten, dass Regeln ausgeführt wurden. |
| `appendRequestBody()` | Der Partial-Zweig liefert `false`; ein Reject-Zweig kann eine Intervention setzen und trotzdem `true` liefern. [Quelle][request-append] | Nur den Returnwert prüfen und die Intervention übersehen. |
| `Transaction::intervention()` | Liefert den disruptiven Zustand; bei `false` wird die Ausgabestruktur im Funktionskörper nicht beschrieben. [Quelle][intervention] | Rückgabe und Struktur unabhängig voneinander auswerten. |
| `msc_rules_add_file()` | Reicht die Lade-Rückgabe durch; bei negativer Rückgabe wird eine Fehlermeldung mit `strdup` bereitgestellt. [Quelle][c-rules] | Einen Fehlerpuffer als statische Zeichenkette behandeln. |

Auch Konstruktor-Wrapper sind genau zu lesen: `msc_init()`, `msc_create_rules_set()` und `msc_new_transaction()` verwenden im untersuchten Code schlicht `new`. Der Transaktionskommentar beschreibt einen möglichen `NULL`-Rückgabewert; der Wrapper enthält jedoch weder `new (std::nothrow)` noch einen `catch`-Block. Daher hier keine allgemeine Garantie dokumentieren, dass sämtliche C++-Fehler automatisch in einen C-Fehlercode übersetzt würden. Quellen: [Engine-Erzeugung][c-engine], [Ruleset-Erzeugung][c-rules], [Transaktionskommentar und Wrapper][c-construct].

## 7. C++-API und C-API zuordnen

Die C-Wrapper delegieren überwiegend an die C++-Objekte. Für C werden die Objektstrukturen verborgen; die `msc_*`-Funktionen bieten Zugriff über Zeiger. `extern "C"` bezeichnet die C-Sprachanbindung dieser Funktionen und ist keine Aussage, dass ihre Implementierung in C geschrieben wäre. Quellen: [C-Typen][c-types], [Wrapper][c-engine], [Transaktions-Wrapper][c-construct].

| Aufgabe | C++-Einstieg | C-Einstieg |
| --- | --- | --- |
| Engine erzeugen und beenden | `ModSecurity`, Destruktor | `msc_init()`, `msc_cleanup()` |
| Connector benennen | `setConnectorInformation()` | `msc_set_connector_info()` |
| Ruleset erzeugen und beenden | `RulesSet`, Destruktor | `msc_create_rules_set()`, `msc_rules_cleanup()` |
| Regeldatei laden | `loadFromUri()` | `msc_rules_add_file()` |
| Regeln aus Text laden | `load()` | `msc_rules_add()` |
| Transaktion erzeugen | `Transaction(ms, rules, logCbData)` | `msc_new_transaction()` |
| Eigene Transaktions-ID übergeben | `Transaction(ms, rules, id, logCbData)` | `msc_new_transaction_with_id()` |
| Verbindung und URI übergeben | `processConnection()`, `processURI()` | `msc_process_connection()`, `msc_process_uri()` |
| Body-Daten anhängen | `appendRequestBody()`, `appendResponseBody()` | `msc_append_request_body()`, `msc_append_response_body()` |
| Regelphasen ausführen | `processRequestHeaders()`, `processRequestBody()`, `processResponseHeaders()`, `processResponseBody()` | Gleichnamige `msc_process_*`-Funktionen in C-Schreibweise |
| Eingriff abholen | `intervention()` | `msc_intervention()` |
| Logging abschließen | `processLogging()` | `msc_process_logging()` |
| Transaktion beenden | Destruktor | `msc_transaction_cleanup()` |

Quellen für die Zuordnung: [Engine-Deklarationen][engine-api], [Ruleset-Deklarationen][rules-api], [C++-Transaktion][transaction-api], [C-Transaktion][c-trans-api]. Die Tabelle ist eine Auswahl der zentralen Aufrufe; die Header enthalten weitere Funktionen. Sie behauptet keine vollständige Funktionsgleichheit der C- und C++-Schnittstellen.

### Zugehörige Freigaben beachten

`msc_rules_error_cleanup()` verwendet `free()` für den von den Regel-Wrappern angelegten Fehlertext. `msc_intervention_cleanup()` gibt die Puffer einer Intervention frei und setzt ihre Felder zurück. Engine, Ruleset und Transaktion verwenden dagegen ihre jeweiligen Objekt-Cleanup-Funktionen, die intern `delete` ausführen. `free()` und `delete` deshalb nicht anhand des sichtbaren Zeigertyps austauschen. Quellen: [Regel-Fehler und Cleanup][c-rules], [Interventions-Cleanup][c-cleanup], [Engine-Cleanup][c-engine].

Ein weiterer Gegenbeleg zur Annahme, `const char *` bedeute immer einen geliehenen Puffer: `Transaction::getResponseBody()` liefert im gelesenen Code einen mit `strdup()` angelegten Puffer, während `msc_who_am_i()` einen `c_str()`-Zeiger auf den Engine-String zurückgibt. Diese beiden Rückgaben haben verschiedene Speicherherkunft. Der Response-Getter ist außerdem im Kommentar als noch nicht verwendetes Gerüst beschrieben; er darf nicht als Beleg für eine umfassende Response-Umschreibfunktion oder beliebige Binärdatenverarbeitung dienen. Quellen: [Response-Getter][response-getter], [who_am_i][c-engine].

## 8. Callbacks: Typ und Lebensdauer gehören zusammen

`ModSecLogCb` hat die Form `void (*)(void *, const void *)`. Die Engine übergibt benutzerdefinierte Daten und entweder Text oder eine `RuleMessage`. Die C++-Überladung `setServerLogCb(cb, properties)` bestimmt die Darstellung. Die Überladung ohne Properties wählt `TextLogProperty`; `msc_set_log_cb()` verwendet genau diese Variante. Einen `void *`-Zeiger daher nur entsprechend der tatsächlich gewählten Property interpretieren. Quellen: [Callback-Typ und Properties][callback-api], [Property-Auswahl und C-Wrapper][callback-setup].

Im Textpfad erzeugt `ModSecurity::serverLog()` eine lokale Zeichenkette und übergibt deren `c_str()` während des Callback-Aufrufs. Im Objektpfad übergibt es die Adresse einer `RuleMessage`; die Regelverarbeitung dokumentiert deren begrenzte Lebensdauer ausdrücklich und kopiert Meldungen für spätere Audit-Nutzung. **Abgeleitete Aufruferregel:** Benötigte Daten im Callback kopieren, wenn sie nach dem Aufruf noch gebraucht werden; diese Zeiger nicht ungeprüft dauerhaft speichern. Quellen: [Callback-Ausführung][callback-call], [RuleMessage-Lebensdauer][message-life].

`logCbData` wird beim Erzeugen der Transaktion übergeben. Der Testkontext benutzt dafür die Adresse seines `std::stringstream`; der Callback wandelt sie wieder in den passenden Typ zurück und schreibt die Meldung hinein. Dieses konkrete Beispiel zeigt, warum Callback-Funktion, Kontexttyp und Kontextlebensdauer zusammenpassen müssen. Quellen: [Testkontext][test-context], [Transaktionskonstruktor][transaction-life].

## 9. Präprozessor und Build-Varianten

Im Code bestimmen Makros, welche Implementierungen enthalten sind. Beispiele sind `WITH_LMDB` für die Auswahl der Collection-Backends, `WITH_LIBXML2` und `WITH_YAJL` für Body-Prozessoren sowie `NO_LOGS` für Debug-Makros. Ein erfolgreich gelesener Quellpfad beweist nicht, dass er in einem konkreten Binary enthalten ist. Bei plattform- oder funktionsabhängigen Fragen gehören Build-Konfiguration und tatsächliche Compilerdefinitionen zur Untersuchung. Quellen: [Engine-Auswahl][engine-life], [Prozessor-Auswahl][transaction-life], [Debug-Makros][c-types].

Die SecRules-Grammatik ist ebenfalls nicht bloß eine Liste gewünschter Funktionen. Sie enthält auch ausdrücklich nicht unterstützte Aktionen, beispielsweise `ACTION_APPEND` mit `ACTION_NOT_SUPPORTED("Append", ...)`. Einen Token oder einen Namen im Repository zu finden reicht daher nicht als Nachweis, dass diese Funktion verfügbar ist. Quelle: [Grammatikfall][unsupported-action].

## 10. Vorhandene Beispiele sinnvoll nutzen

| Quelle | Wofür sie sich eignet | Beim Lesen beachten |
| --- | --- | --- |
| `examples/using_bodies_in_chunks/simple_request.cc` | API-Reihenfolge, Chunk-Zufuhr, Interventionsabfrage, Cleanup-Reihenfolge. | Ein Demonstrationsprogramm, dessen Helfer Ausgaben erzeugt; kein Nachweis der HTTP-Wirkung eines produktiven Connectors. [Quelle][example-flow] |
| `test/common/modsecurity_test_context.h` | Engine und Ruleset als Member, Transaktionskonstruktion und Text-Callback. | Die zurückgegebene Transaktion referenziert den Kontext. [Quelle][test-context] |
| `examples/simple_example_using_c/test.c` | C-Namen, Regeldatei-Laden und Fehlertext. | Der gezeigte Cleanup-Pfad enthält kein `msc_transaction_cleanup(transaction)`; nicht als vollständiges Ressourcenmuster kopieren. [Quelle][c-example] |
| README-C++-Ausschnitt | Historischer Einstieg in die Idee der API. | Namen und Argumentlisten gegen die aktuellen Header abgleichen; sie stimmen hier nicht überall überein. [Quelle][readme-example] |

## 11. Leseweg vor einer konkreten Änderung

Die folgende Reihenfolge ist eine Empfehlung für dieses Arbeitsrepository:

1. Die öffentliche Signatur und ihre tatsächlichen Aufrufer finden; Namen und Parametertypen abgleichen.
2. Die konkrete Implementierung einschließlich früher Rückgaben, Build-Makros und Fehlerzweige lesen.
3. Für jeden betroffenen Zeiger Erzeugung, Besitzübergang und Freigabe verfolgen.
4. Den Datenfluss bis zur Regelphase, Intervention oder Logausgabe nachvollziehen; dabei die beiden Phasennummerierungen aus [ARCHITECTURE.md](ARCHITECTURE.md) beachten.
5. Bestehende passende Tests und Build-Anweisungen lesen und den tatsächlich überprüften Umfang protokollieren.
6. Offene Punkte als offen dokumentieren. Ein gefundener Symbolname, ein Kommentar oder ein Beispiel ersetzt keinen Funktionsnachweis.

Die Empfehlung folgt aus den oben belegten Unterschieden zwischen Headern, Implementierungen und Beispielen. Sie schreibt keine ungelesenen Sprachregeln, unbewiesene Thread-Sicherheit oder automatisch erfolgreiche Tests fest.

[cxx17]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L49-L63
[cxx17-win]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/CMakeLists.txt#L102-L103
[engine-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/modsecurity.h#L275-L331
[rules-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/rules_set.h#L49-L107
[transaction-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L335-L419
[c-types]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L20-L80
[c-trans-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L677-L767
[c-construct]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1804-L1829
[c-engine]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L411-L482
[c-rules]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rules_set.cc#L277-L345
[c-cleanup]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L2153-L2195
[load]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rules_set.cc#L50-L109
[recommended]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/modsecurity.conf-recommended#L1-L76
[parser-build]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/Makefile.am#L1-L43
[driver-rules]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/driver.cc#L46-L124
[chain-fields]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/rule_with_actions.h#L38-L109
[readme-example]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L233-L255
[example-setup]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/examples/using_bodies_in_chunks/simple_request.cc#L131-L170
[example-flow]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/examples/using_bodies_in_chunks/simple_request.cc#L95-L282
[installed-headers]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/Makefile.am#L37-L67
[abi]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/anchored_set_variable.cc#L124-L139
[engine-life]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L62-L109
[transaction-life]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L113-L182
[collections]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/collection/collections.cc#L37-L56
[test-context]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/common/modsecurity_test_context.h#L13-L39
[rules-storage]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/rules.h#L41-L87
[operator-life]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_operator.cc#L54-L79
[actions-life]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_actions.cc#L141-L178
[resolve]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/anchored_set_variable.cc#L70-L111
[resolve-first]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/anchored_set_variable.cc#L114-L122
[evaluate]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_operator.cc#L199-L353
[variable-value]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/variable_value.h#L38-L125
[uri]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L337-L443
[header-lengths]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L618-L654
[request-append]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L929-L971
[rule-base]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/rule.h#L65-L105
[rules-evaluate]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rules_set.cc#L121-L232
[request-phase]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L461-L471
[intervention]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1350-L1389
[response-getter]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1234-L1249
[callback-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/modsecurity.h#L205-L267
[callback-setup]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L389-L413
[callback-call]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L193-L212
[message-life]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_actions.cc#L495-L525
[unsupported-action]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L2726-L2729
[c-example]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/examples/simple_example_using_c/test.c#L26-L76
