# Projektkontext: ModSecurity v3 verstehen und damit arbeiten

## Verlässliche Grundlage

Diese Einführung beschreibt den offiziellen ModSecurity-Quellstand **`effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`** auf `v3/master`. Die Quellen am Ende sind auf diesen Commit fixiert. Die Erklärung dient als Einstieg; für eine konkrete Änderung sind anschließend die betroffenen Implementierungen zu lesen.

`AGENTS.md`, `CODEX_START.md` und `.codex/` sind ergänzte Arbeitskonventionen dieses Repositorys. Sie erklären den Code und halten Arbeitsergebnisse fest. Sie sind keine Funktionen der Upstream-Bibliothek.

## 1. Wozu ModSecurity dient

ModSecurity v3 stellt mit **libmodsecurity** eine Bibliothek für die regelbasierte Untersuchung von HTTP-Daten bereit. Der Code kann Regeln laden, Daten einer Anfrage und Antwort entgegennehmen, Bedingungen auswerten sowie Meldungen und einen Interventionszustand erzeugen. Das zentrale Buildziel heißt `libmodsecurity.la`. Quellen: [Projektbeschreibung][scope], [Bibliotheksziel][build], [Transaktions-API][transaction].

Dabei sind drei Bestandteile auseinanderzuhalten:

| Bestandteil | Aufgabe | Wo diese Abgrenzung sichtbar wird |
| --- | --- | --- |
| **Bibliothek** | Verarbeitet die übergebenen Daten anhand des geladenen Regelwerks. | `Transaction`, `RulesSet` und ihre Implementierungen. [Quellen][transaction] [Regelauswertung][rules-evaluate] |
| **Connector oder aufrufende Anwendung** | Übergibt HTTP-Daten an die Bibliothek und reagiert auf ihre Ergebnisse im jeweiligen Server-/Anwendungskontext. | Die API nimmt Daten als Parameter entgegen; der Upstream beschreibt Connectoren als getrennte Projekte. [Quellen][scope] [API-Beispiel][example] |
| **Regelwerk und Konfiguration** | Legt fest, welche Variablen, Bedingungen, Aktionen, Phasen und Einstellungen verwendet werden. | SecLang-Grammatik und `modsecurity.conf-recommended`. [Quellen][grammar] [Konfiguration][config] |

Eine Bibliotheksänderung, eine neue Regel und eine Änderung am Webserver-Connector sind damit unterschiedliche Aufgaben. Die Beispielkonfiguration im Repository und ein externes Regelwerk müssen ebenfalls getrennt betrachtet werden; die hier beschriebenen Quellen belegen den Inhalt dieser Bibliothek und ihrer mitgelieferten Dateien. Weiterführend: [Architektur](ARCHITECTURE.md) und [Regeln und Konfiguration](CONFIG_AND_RULES.md).

## 2. Welche Sprachen hier wichtig sind

Die Bibliothek wird in **C++17** gebaut: `configure.ac` fordert diesen Standard ausdrücklich, und der Windows-CMake-Build setzt ihn ebenfalls. Klassen wie `modsecurity::Transaction` und `modsecurity::RulesSet` stehen in Headern unter `headers/modsecurity/`; ihre Implementierungen liegen überwiegend unter `src/`. Quellen: [C++17 unter Autotools][cxx17], [C++17 unter Windows][cxx17-win], [öffentliche API][transaction].

Daneben gibt es eine **C-API** mit Funktionen wie `msc_init()`, `msc_new_transaction()` und `msc_process_request_body()`. Diese Funktionen delegieren an C++-Objekte. Ein Programm kann die C-Schnittstelle verwenden, ohne die C++-Klassen direkt aufzurufen. Daraus folgt keine vollständige Gleichheit sämtlicher Möglichkeiten beider Schnittstellen. Quellen: [C-Einstieg][c-engine], [Transaktions-Wrapper][c-transaction], [C-Deklarationen][c-api].

**SecLang beziehungsweise SecRules** ist die Eingabesprache für Regeln und Direktiven. Ihre Scanner- und Grammatikquellen sind `.ll`- und `.yy`-Dateien; das ist von C++-Implementierungscode zu unterscheiden. Shell, JSON, Python, Lua und Buildformate kommen in weiteren Arbeitsbereichen vor. Die genaue Zuordnung steht in der [Repository-Landkarte](REPOSITORY_MAP.md). Quellen für die Regelsprache: [Parser-Build][parser-build], [Grammatik][grammar].

Der öffentliche Header enthält die Versionsmakros `3`, `0` und `17`, zusammen `3.0.17`. Das ist eine Angabe im untersuchten Quellcode. Sie allein beweist weder eine veröffentlichte Release-Version dieses Commits noch die Version einer tatsächlich installierten Bibliothek. Quelle: [Versionsmakros][version].

## 3. Die drei Kernklassen

| Klasse | Einfach erklärt | Konkrete Aufgaben im Code |
| --- | --- | --- |
| `ModSecurity` | Gemeinsamer Bibliothekskontext. | Erzeugt Collection-Backends, hält Connectorinformationen und den Logging-Callback. [Konstruktor][engine], [API][engine-api] |
| `RulesSet` | Geladene Regeln samt Einstellungen. | `loadFromUri()` lädt eine Datei; `load()` lädt Regeltext; `evaluate()` wertet die Regeln einer Phase aus. [Laden][load], [Auswertung][rules-evaluate] |
| `Transaction` | Zustand einer einzelnen HTTP-Transaktion. | Nimmt Verbindungsdaten, URI, Header und Bodies entgegen; hält Variablen, Meldungen und Interventionen. [API][transaction], [Lebensdauer][lifetime] |

Der Aufrufer übergibt Engine und Ruleset an die Transaktion. Diese speichert die Zeiger, löscht die beiden Objekte aber nicht selbst. Die Objekte müssen während ihrer Nutzung gültig bleiben. Das vorhandene Chunk-Beispiel beendet zuerst die Transaktion, dann das Ruleset und zuletzt die Engine. Quellen: [Konstruktor und Destruktor][lifetime], [Beispiel][example]. Die genauen Besitz- und Freigaberegeln erklärt der [C++-/C-Leitfaden](CPP_GUIDE.md).

## 4. Welche Eingaben und Ergebnisse die Bibliothek verarbeitet

| Eingabe | Typischer API-Einstieg | Bedeutung |
| --- | --- | --- |
| Regeln und Direktiven | `RulesSet::loadFromUri()` oder `load()` | Aufbau des Regelwerks und seiner Einstellungen. [Quelle][load] |
| Verbindung und Request-Zeile | `processConnection()` und `processURI()` | Adressen, Ports, URI, HTTP-Methode und Protokollversion. [Quelle][transaction] |
| Request-/Response-Header | `addRequestHeader()` und `addResponseHeader()` | Übergabe von Headernamen und -werten. [Quelle][transaction] |
| Request-/Response-Body | `appendRequestBody()` und `appendResponseBody()` | Übergabe von Datenpuffern mit expliziter Länge; Größenlimits können dabei bereits wirken. [Request][append], [Response][response-body] |

| Ergebnis | Was es aussagt | Grenze dieser Aussage |
| --- | --- | --- |
| Ladeergebnis und Parserfehler | Ob der Ladepfad Regeln übernehmen konnte und welche Fehler er meldet. | Kein Test einer späteren HTTP-Transaktion. [Quelle][load] |
| Transaktionsvariablen und Match-Daten | Welche Daten und Treffer die Auswertung ermittelt beziehungsweise speichert. | Ihre Bedeutung hängt von Phase und Auswertungspfad ab. [Quelle][rule-evaluate] |
| Intervention | Ein vom Aufrufer abzufragender Zustand mit unter anderem Status, URL, Logtext und Disruptiv-Flag. | Die tatsächliche HTTP-Reaktion muss der Aufrufer umsetzen. [Quelle][intervention] |
| Regelmeldungen, Debug- und Audit-Ausgaben | Informationen über Treffer, Verarbeitung oder ausgewählte Transaktionen. | Die Ausgabewege haben unterschiedliche Konfiguration und Bedingungen. [Logging][logging], [Callback][callback] |

Ein erfolgreicher Methodenaufruf ist somit nicht automatisch eine Freigabeentscheidung. Beispielsweise kann `appendRequestBody()` im Reject-Zweig eine Intervention setzen und dennoch `true` zurückgeben. Rückgabewert und Intervention müssen anhand ihres jeweiligen Vertrags betrachtet werden. Quelle: [Request-Body-Limitpfad][append].

## 5. Der typische Ablauf einer Transaktion

Das vorhandene Chunk-Beispiel zeigt einen vollständigen API-Ablauf. Die folgende Reihenfolge ist dessen verständliche Zusammenfassung; ein tatsächlicher Connector kann bei einer Intervention die weitere HTTP-Verarbeitung beenden. Quelle: [Beispielprogramm][example].

1. Bibliothekskontext und Ruleset anlegen; Regeln laden und Ladefehler prüfen.
2. Eine Transaktion mit diesen Objekten anlegen; Verbindung, URI und Request-Header übergeben.
3. Request-Header-Phase ausführen; den Interventionszustand prüfen.
4. Request-Body-Daten übergeben; Body-Verarbeitung und Request-Body-Phase aufrufen; Intervention prüfen.
5. Entsprechende Schritte für Response-Header und Response-Body durchführen, soweit die Anwendung fortfährt.
6. Logging abschließen und Transaktionsressourcen freigeben.

Die Methoden haben Bedingungen und frühe Rückgaben. So überspringt `processLogging()` bei deaktivierter Engine sowohl die Logging-Phase als auch die Audit-Auswahl. Request- und Response-Bodies haben eigene Zugriffs-, Format- und Limitbedingungen. Quellen: [Logging][logging], [Request-Verarbeitung][request-body], [Response-Verarbeitung][response-body].

Auch die Nummerierung braucht Aufmerksamkeit: SecRules-Phasen und das interne C++-Enum sind nicht deckungsgleich, weil das Enum zusätzlich `UriPhase` enthält. Die Aktionsklasse `Phase` übersetzt die Regelangabe; `processURI()` befüllt Daten, ohne `RulesSet::evaluate(UriPhase, ...)` aufzurufen. Die vollständige Tabelle steht in [ARCHITECTURE.md](ARCHITECTURE.md). Quellen: [Enum][phases], [Übersetzung][phase-action], [URI-Verarbeitung][uri].

## 6. Was „eine Regel prüfen“ im Code bedeutet

Eine Regel kann Daten über Variablen auswählen, diese Daten transformieren, eine Bedingung mit einem Operator prüfen und Aktionen ausführen. `RuleWithOperator::evaluate()` verbindet diese Schritte. Es berücksichtigt außerdem Ausnahmen und Regelketten. Quellen: [Regelauswertung][rule-evaluate], [Transformationen][transformations].

Ein Operator beantwortet die konkrete Prüfbedingung; eine Transformation verändert den Vergleichswert. Aktionen können zum Beispiel eine Meldung ergänzen, einen Wert setzen oder eine Intervention anfordern. Bei Regelketten sind Aktionen vor und nach vollständigem Kettentreffer getrennt zu betrachten. Quellen: [Regelauswertung][rule-evaluate], [Aktionssteuerung][actions].

Die mitgelieferte Konfiguration beginnt mit `SecRuleEngine DetectionOnly`. Disruptive Aktionen werden in `executeAction()` nur bei `EnabledRuleEngine` ausgeführt. Ein gefundener Treffer oder eine vorhandene `deny`-Aktion ist deshalb allein kein Beleg für eine tatsächlich blockierte Anfrage. Quellen: [Konfiguration][config], [Engine-Bedingung für Aktionen][actions].

## 7. Wie man sinnvoll lernt und Änderungen vorbereitet

| Ziel | Empfohlener Einstieg | Danach im Code lesen |
| --- | --- | --- |
| Überblick gewinnen | [Repository-Landkarte](REPOSITORY_MAP.md) | Root-Builddateien, öffentliche Header, passende Unterverzeichnisse. |
| HTTP-Datenfluss verstehen | [Architektur](ARCHITECTURE.md) | `src/transaction.cc` und ein vorhandenes API-Beispiel. |
| C++ und C sicher zuordnen | [C++-/C-Leitfaden](CPP_GUIDE.md) | Signatur, Implementierung, Aufrufer und Freigabepfad gemeinsam. |
| Eine Regel oder Direktive verstehen | [Konfiguration und Regeln](CONFIG_AND_RULES.md) | Scanner, Grammatik, konkrete Operator-/Aktionsimplementierung. |
| Einen Fehler oder eine Funktion finden | [Featurekarte](FEATURE_MAP.md) | Die dort zugeordneten Implementierungen und vorhandenen Tests. |
| Einen eigenen Build prüfen | [Build und Tests](BUILD_AND_TEST.md) | `configure.ac`, Plattform-Build und tatsächliche Testausgabe. |

Diese Lesereihenfolge ist eine Empfehlung für dieses Arbeitsrepository. Für eine Codeänderung zuerst das gewünschte beobachtbare Verhalten festhalten, dann die beteiligten Dateien und vorhandenen Prüfungen eingrenzen. Ein gefundenes Symbol oder ein vorhandener Testfall ersetzt keinen ausgeführten Funktionsnachweis.

## 8. Wie neue Chats an vorhandene Arbeit anschließen

Die Arbeitskonvention lautet: zuerst `AGENTS.md`, den [Dokumentationsindex](README.md) und den aktuellen festgehaltenen Arbeitsstand lesen; dann nur die für den Auftrag relevanten Kapitel und Quellen öffnen. Am Ende werden Ergebnisse, Belege, tatsächlich ausgeführte Prüfungen und offene Schritte nachvollziehbar übergeben. Der [Sitzungsablauf](WORKFLOW.md) und die [Übergabevorlage](HANDOFF_TEMPLATE.md) beschreiben dieses Verfahren.

Diese Dateien sind bewusst lesbarer Projektkontext. Sie garantieren weder ein automatisches Gedächtnis über Chats hinweg noch Zugriff auf Dateien, die einer neuen Sitzung nicht verfügbar sind. Unbekannte Sachverhalte bleiben offen, bis passende Quellen gelesen oder Prüfungen tatsächlich ausgeführt wurden.

## Quellen am fixierten Commit

[scope]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L21-L46
[build]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/Makefile.am#L11-L14
[transaction]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L335-L419
[rules-evaluate]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rules_set.cc#L121-L234
[example]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/examples/using_bodies_in_chunks/simple_request.cc#L95-L282
[grammar]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L934-L1218
[config]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/modsecurity.conf-recommended#L1-L76
[cxx17]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L49-L63
[cxx17-win]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/CMakeLists.txt#L102-L103
[c-engine]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L462-L482
[c-transaction]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1804-L1904
[c-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L677-L767
[parser-build]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/Makefile.am#L4-L43
[version]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/modsecurity.h#L191-L201
[engine-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/modsecurity.h#L275-L313
[engine]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L62-L109
[load]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rules_set.cc#L50-L110
[lifetime]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L113-L182
[append]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L929-L971
[rule-evaluate]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_operator.cc#L199-L360
[intervention]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1350-L1390
[logging]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1304-L1346
[callback]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L193-L212
[request-body]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L674-L870
[response-body]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1117-L1230
[phases]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/modsecurity.h#L100-L171
[phase-action]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/phase.cc#L25-L72
[uri]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L337-L443
[transformations]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_actions.cc#L329-L441
[actions]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_actions.cc#L202-L325
