# Konfiguration und SecRules aus dem Code verstehen

## Gültigkeit

Diese Erklärung gilt für den untersuchten offiziellen Quellstand
`effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729` von `v3/master`.
Sie ist eine Orientierung zu den vorhandenen Implementierungen und
Testfällen, keine vollständige Referenz sämtlicher Direktiven und keine
Behauptung über einen erfolgreich betriebenen Webserver.

## 1. Drei Ebenen auseinanderhalten

| Ebene | Aufgabe im Repository | Einstieg in den Quellcode |
| --- | --- | --- |
| Bibliothekskonfiguration | Direktiven setzen Eigenschaften eines Regelsatzes, etwa den Engine-Modus, den Zugriff auf Bodies und Größenlimits. | [Parseraktionen für Engine/Body-Zugriff][engine-grammar], [Limits][limits-grammar] |
| SecRules | Regeln verbinden eine Variablenauswahl, einen Operator und Aktionen. Der Parser erzeugt dafür `RuleWithOperator`-Objekte. | [SecRule-Grammatik][rule-grammar] |
| Aufrufende Anwendung/Connector | Übergibt HTTP-Daten an die Bibliothek und entscheidet anhand ihrer Interventionen über die weitere Verarbeitung. Das v3-Hauptrepository enthält die Bibliothek; Connectors sind getrennte Projekte. | [README: Connectors][connectors], [Transaktions-API][transaction-header] |

SecLang ist die von diesem Projekt eingelesene Regelsprache. C++ ist die
Sprache der hier beschriebenen Engine-Implementierung. Eine Textregel wird
also nicht zu einem eigenen C++-Programm übersetzt: Die Grammatik legt
Variablen-, Operator- und Aktionsobjekte an, die später gegen eine
`Transaction` ausgewertet werden. Quellen: [Parser: Objekterzeugung][rule-grammar],
[RuleWithOperator::evaluate][rule-evaluation].

## 2. Der Engine-Modus ist Teil des tatsächlichen Verhaltens

Die Datei `modsecurity.conf-recommended` beginnt mit
`SecRuleEngine DetectionOnly`. Das ist der konkrete Inhalt dieser
Beispielkonfiguration, keine Aussage darüber, welche Konfiguration eine
Anwendung tatsächlich geladen hat. Der Parser unterscheidet `Off`, `On`
und `DetectionOnly`; `RuleWithActions::executeAction` lässt disruptive
Aktionen nur bei `EnabledRuleEngine` laufen. Quellen:
[Beispielkonfiguration][recommended-engine], [Engine-Grammatik][engine-grammar],
[Aktionsausführung][actions].

| Wert | Aussage, die sich hier belegen lässt |
| --- | --- |
| `On` | Der Parser setzt `EnabledRuleEngine`; die Engine-Zustandsprüfung lässt disruptive Regelaktionen zu. |
| `Off` | Der Parser setzt `DisabledRuleEngine`; vorhandene Engine-Regressionen erwarten bei ihrem ausgeschalteten Szenario keine Blockierung. |
| `DetectionOnly` | Der Parser setzt `DetectionOnlyRuleEngine`; disruptive Regelaktionen werden an der zentralen Aktionsprüfung nicht ausgeführt. |

Quellen: [Parserwerte][engine-grammar], [Aktionsprüfung][actions],
[Engine-Regressionen][engine-tests]. Die JSON-Erwartungen sind belegte
Testfälle, aber durch dieses Dokument noch kein frisch ausgeführter Test.

Die Aktion `ctl:ruleEngine=...` kann einen transaktionsbezogenen Zustand
setzen. `Transaction::getRuleEngineState()` verwendet diesen Zustand,
sofern er gesetzt ist, sonst den des Regelsatzes. Bei einer Erklärung eines
konkreten Requests reicht deshalb nicht immer der Blick auf eine einzelne
globale Direktive. Quellen: [ctl-Implementierung][ctl-engine],
[getRuleEngineState][engine-state].

## 3. Eine vorhandene Regel vollständig lesen

Die folgende Regel steht so in `modsecurity.conf-recommended`:

```apache
SecRule REQUEST_HEADERS:Content-Type "^application/json" \
     "id:'200001',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
```

Quelle: [JSON-Regel der Beispielkonfiguration][recommended-json].

| Teil | Bedeutung in dieser Regel | Codebeleg |
| --- | --- | --- |
| `SecRule` | Erzeugt eine Regel mit Variablen und Operator. | [Grammatikzweig][rule-grammar] |
| `REQUEST_HEADERS:Content-Type` | Wählt das benannte Element der Request-Header-Collection. | [Header-Variablen in der Grammatik][header-variables] |
| `"^application/json"` | In der Beispielkonfiguration verwendeter regulärer Ausdruck. Die Operatorenauflösung und Regex-Ausführung liegen im Parser und in `Rx`. | [Operatorgrammatik][operators-grammar], [Rx][rx] |
| `id:'200001'` | Regelkennzeichen. `Driver::addSecRule` prüft bei nicht angehängten Regeln auf fehlende und doppelte Operatorregel-IDs. | [Driver::addSecRule][driver-rules] |
| `phase:1` | Ordnet die Regel der Request-Header-Phase zu. | [Phase::init][phases] |
| `t:none,t:lowercase` | Unterdrückt frühere Transformationen für diese Auswertung und wendet anschließend die angegebene Kleinschreibung an. | [Transformationsablauf][transformations], [LowerCase-Implementierung][lowercase] |
| `pass` | Die Aktionsimplementierung gibt den bisherigen Interventionsinhalt frei und setzt die Intervention zurück. Ihre Ausführung unterliegt der Prüfung für disruptive Aktionen. | [Pass][pass], [Aktionsprüfung][actions] |
| `nolog` | Setzt `ruleMessage.m_saveMessage` auf `false`. | [NoLog][nolog] |
| `ctl:requestBodyProcessor=JSON` | Setzt den angeforderten Body-Prozessor für die Transaktion. | [Body-Prozessor-Aktion][body-ctl] |

Der Lernschritt hier ist: Zuerst die selektierte Eingabe finden, dann die
Transformationen, danach den Operator und schließlich die Aktionen und
den Engine-Modus. Das entspricht dem Ablauf in
`RuleWithOperator::evaluate`: Variablen auswerten, Transformationen
anwenden, Operator ausführen und bei Treffern weitere Aktionen ausführen.
Quelle: [RuleWithOperator::evaluate][rule-evaluation].

### Variablenlisten und Collections

Die Grammatik enthält Variablenlisten mit `PIPE` sowie Modifikatoren für
Ausschluss und Zählung. Außerdem unterscheidet sie beispielsweise
`ARGS`, `ARGS:<Element>` und eine Auswahl per regulärem Ausdruck.
Das ist der Einstieg, wenn eine Regel mehrere Eingaben untersucht,
einzelne Ziele ausnimmt oder eine Anzahl vergleichen soll. Ein vorhandenes
konkretes Zählbeispiel ist `SecRule &ARGS "@ge 1000"` in der empfohlenen
Konfiguration. Quellen: [Variablengrammatik][variable-grammar],
[Argumentanzahl-Regel][arguments-rule].

`TX` hat pro `Collections`-Instanz ein eigenes
`InMemoryPerProcess`-Objekt. Die weiteren Collections `GLOBAL`, `IP`,
`SESSION`, `USER` und `RESOURCE` werden dem Transaktions-Collections-Objekt
übergeben; der `ModSecurity`-Konstruktor wählt dafür abhängig von
`WITH_LMDB` das LMDB- oder In-Memory-Backend. Deshalb nie pauschal von
persistenter oder prozessübergreifender Speicherung aller Variablen
ausgehen. Quellen: [Collections-Konstruktor][tx-collection],
[Backendauswahl][collections].

## 4. Phasen und Zeitpunkt der Daten

Die Regelsprache ordnet die üblichen numerischen Angaben wie folgt zu:

| SecRule-Angabe | Im Parser gewählter Phasenname |
| --- | --- |
| `phase:1` | `RequestHeadersPhase` |
| `phase:2` | `RequestBodyPhase` |
| `phase:3` | `ResponseHeadersPhase` |
| `phase:4` | `ResponseBodyPhase` |
| `phase:5` | `LoggingPhase` |

Quelle: [Phase::init, Zuordnung][phases]. Dieselbe Funktion akzeptiert
auch die Namen `request`, `response` und `logging` und ordnet sie
den Regelsprache-Phasen 2, 4 und 5 zu. Sie kennt zusätzlich `phase:0`
als `ConnectionPhase`; das allein beweist jedoch noch keine Ausführung
dieser Phase an einem bestimmten Connector-Aufruf. Interne Enum-Werte
und Regelsprache-Zahlen nicht ungeprüft gleichsetzen. Für den Ablauf
der API siehe [ARCHITECTURE.md](ARCHITECTURE.md).

In der Beispielkonfiguration wird der JSON-Prozessor bereits in Phase 1
ausgewählt. `processRequestBody()` verarbeitet später je nach Auswahl und
eingebauten Bibliotheken XML, JSON oder Multipart und setzt unter anderem
Fehlervariablen. Eine Body-Regel kann nur die Daten beurteilen, die dieser
Verarbeitungspfad tatsächlich bereitgestellt hat. Quellen:
[JSON-Prozessorwahl][recommended-json], [Body-Verarbeitung][body-processing].

## 5. Transformationen, Aktionen, Verkettung

### Transformationen

`executeTransformations()` beginnt mit einer Kopie des zu prüfenden
Wertes. Es berücksichtigt Standardtransformationen des Regelsatzes,
die Transformationen der Regel und Regelupdates. `t:none` beeinflusst,
welche früheren Transformationen ausgeführt werden. Mit `multiMatch`
werden auch Zwischenwerte für Operatorprüfungen gesammelt. Ohne
`multiMatch` wird am Ende der resultierende Wert eingetragen. Daher
darf eine Erklärung nicht die Reihenfolge der Transformationen
weglassen. Quelle: [executeTransformations][transformations].

### Aktionen und `SecDefaultAction`

Nicht jede Aktion wartet auf dieselbe Bedingung: Der Code unterscheidet
unter anderem Aktionen, die unabhängig vom Ergebnis einer folgenden
verketteten Regel laufen, und Aktionen nach einem vollständigen Treffer.
`SetVar` gehört im untersuchten Ablauf zur ersten Gruppe. Die endgültige
disruptive Aktion wird an anderer Stelle behandelt. Quellen:
[executeActionsIndependentOfChainedRuleResult und executeActionsAfterFullMatch][actions],
[Aufrufreihenfolge im Regelauswerter][rule-evaluation].

`SecDefaultAction` ist im Parser kein beliebiger Textanhang. Der Parser
prüft unter anderem, ob eine disruptive Aktion vorhanden ist, untersagt
`t:none` innerhalb der Standardaktionen und lässt pro Phase und
Konfigurationskontext nur einen solchen Standardaktionssatz zu.
Die Details dieser Regeln gehören bei Ladefehlern zur Diagnose.
Quelle: [SecDefaultAction-Grammatik][default-actions].

### `chain`

Beim Anhängen einer verketteten Regel übernimmt der Driver die Phase
des Vorgängers und verwirft disruptive Aktionen in einem angehängten
Kettenglied. Zur Laufzeit ruft `RuleWithOperator::evaluate` die
Kindregel rekursiv auf; erst ein erfolgreicher weiterer Treffer führt
im gezeigten Pfad zu den Aktionen nach vollständigem Match.
Das bedeutet nicht, dass vorher keinerlei Aktion gelaufen sein kann:
Die unabhängigen Aktionen werden bereits vor der Kettenprüfung
aufgerufen. Quellen: [Verkettung im Driver][driver-rules],
[Reihenfolge der Auswertung][rule-evaluation], [Aktionsgruppen][actions].

## 6. Bodies, Limits und Fehlerzustände

Die Werte in `modsecurity.conf-recommended` sind konkrete Beispiele:

| Einstellung | Wert in der Datei |
| --- | --- |
| `SecRequestBodyAccess` | `On` |
| `SecRequestBodyLimit` | `13107200` |
| `SecRequestBodyNoFilesLimit` | `131072` |
| `SecRequestBodyLimitAction` | `Reject` |
| `SecRequestBodyJsonDepthLimit` | `512` |
| `SecArgumentsLimit` | `1000` |
| `SecResponseBodyAccess` | `On` |
| `SecResponseBodyMimeType` | `text/plain text/html text/xml` |
| `SecResponseBodyLimit` | `524288` |
| `SecResponseBodyLimitAction` | `ProcessPartial` |

Quellen: [Request-Einstellungen][recommended-request],
[Response-Einstellungen][recommended-response]. Die Tabelle ist eine
Bestandsaufnahme der Datei, keine Empfehlung für die Größen deiner
Anwendung und keine Aussage über alle internen Standardwerte.

Bei einem überschrittenen Request-Body-Limit setzt
`appendRequestBody()` `INBOUND_DATA_ERROR`. Für die konfigurierte Aktion
`ProcessPartial` schreibt der gezeigte Zweig nur den verbleibenden Platz
und gibt `false` zurück. Für `Reject` setzt er eine disruptive
Intervention mit Status 403 nur dann, wenn die Rule-Engine eingeschaltet
ist. Der Response-Pfad prüft entsprechend Größe und Aktion und setzt
`OUTBOUND_DATA_ERROR`. Quellen: [Request-Pufferung][request-limit],
[Response-Pufferung][response-limit].

**Kommentar und Implementierung unterscheiden:** Der Kommentar der
Beispielkonfiguration beschreibt einen automatischen Wechsel zu
`ProcessPartial` in `DetectionOnly`. Aus dem eben genannten
`appendRequestBody()`-Zweig lässt sich jedoch direkt nur die Prüfung der
konfigurierten Limitaktion und das Unterdrücken der Ablehnung außerhalb
von `EnabledRuleEngine` ableiten. Diese Dokumentation behauptet keinen
allgemeinen automatischen Wechsel, der an dieser Stelle nicht
implementiert ist. Quellen: [Konfigurationskommentar][recommended-request],
[implementierter Zweig][request-limit].

Die Body-Verarbeitung setzt bei XML-/JSON-Fehlern Variablen wie
`REQBODY_ERROR`, `REQBODY_ERROR_MSG` und
`REQBODY_PROCESSOR_ERROR`. Die Beispielkonfiguration enthält dazu
ausdrückliche Regeln. Solche Fehlervariablen und die dazu geladenen
Regeln sind bei einer Fehlersuche gemeinsam zu untersuchen; eine
fehlgeschlagene Parseroperation ist nicht automatisch dieselbe Sache
wie eine vom Connector ausgeführte HTTP-Ablehnung. Quellen:
[Fehlerbehandlung der Body-Prozessoren][body-processing],
[Regeln für Parserfehler][recommended-errors], [disruptive Aktionsprüfung][actions].

## 7. Regex-Verhalten ist auch eine Buildentscheidung

`Rx::init()` erzeugt bei einem statischen Muster bereits einen
`Regex`; bei Parametern mit Makro wird in `Rx::evaluate()` ein
transaktionsbezogenes Muster erzeugt. Ein ungültiges statisches Muster
kann daher schon beim Laden der Regel auffallen. Die Auswertung
berücksichtigt den gesetzten `m_pcreMatchLimit` und meldet
Regex-Fehler über Transaktionsvariablen. Quelle: [Rx-Implementierung][rx].

Der Standardwert von `kRxMultiLine` ist `true` und wird mit
`MODSEC_REGEX_DOLLAR_ENDONLY` auf `false` gesetzt. Der
`Regex`-Konstruktor verwendet dann entweder
`DOTALL | MULTILINE` oder `DOTALL | DOLLAR_ENDONLY` für PCRE2
beziehungsweise PCRE. Der Buildschalter
`--enable-regex-dollar-endonly=yes` setzt das entsprechende Makro.
Für diesen Sonderbuild existieren Regressionen mit der Ressource
`regex-dollar-endonly`; ein übersprungener Fall in einem anderen
Build bestätigt seine Semantik nicht. Quellen: [Rx-Schalter][rx],
[Regex-Konstruktor][regex-constructor], [configure-Schalter][regex-option],
[vorhandene Regressionen][regex-test].

## 8. Manche bekannten Direktiven sind nicht implementiert

Ein in Beispielen vorhandener Direktivenname beweist keine vollständig
implementierte Wirkung. Im geprüften Grammatikstand gibt es unter
anderem folgende Unterschiede:

| Direktive | Was die Grammatik tatsächlich tut | Quelle |
| --- | --- | --- |
| `SecRequestBodyInMemoryLimit` | Erzeugt einen Parserfehler mit dem Hinweis auf Webserver-Konfiguration. | [Grammatik, 1655–1662][limits-grammar] |
| `SecPcreMatchLimitRecursion` | Akzeptiert den Zweig ohne Wirkung im gezeigten Parsercode; die Meldung zur fehlenden Unterstützung ist auskommentiert. Nicht mit dem implementierten `SecPcreMatchLimit` gleichsetzen. | [Grammatik, 1696–1707][limits-grammar] |
| `SecTmpDir` | Der gezeigte Zweig setzt keinen Wert; die Fehlermeldung zur fehlenden Unterstützung ist auskommentiert. | [Grammatik, 1748–1758][compatibility] |
| `SecDataDir` | Ebenfalls ohne implementierte Zuweisung an dieser Stelle; Kommentar verweist auf Backendverhalten. | [Grammatik, 1759–1767][compatibility] |
| `SecStatusEngine` | Keine implementierte Aktion im gezeigten Grammatikzweig; die Fehlermeldung ist auskommentiert. | [Grammatik, 1781–1785][compatibility] |

Diese Tabelle beschreibt überprüfte Stellen. Sie ist keine vollständige
Liste aller Unterschiede zu ModSecurity v2. Für eine zusätzliche
Direktive zuerst Scanner, Grammatikaktion, verwendete Eigenschaft und
Laufzeitzugriff verfolgen.

## 9. Konfiguration laden, Verhalten belegen

`Include` wird im Scanner behandelt: Der Code sucht Ressourcen relativ
zum jeweiligen Dateikontext, expandiert die gefundene Angabe und öffnet
weitere Scannerpuffer. Bei einem Problem meldet er den betroffenen
Dateipfad. Ein nicht gefundener Include kann daher die Erklärung für
einen Ladefehler sein, auch wenn die einzelne Regel syntaktisch korrekt
aussieht. Quelle: [Include-Behandlung][include].

Der Regelprüfer lädt Dateien oder SecLang-Text über `RulesSet`, erzeugt
aber selbst keine HTTP-Transaktion. Eine erfolgreiche Ladeprüfung
belegt folglich einen anderen Teil des Verhaltens als ein
Regressionstest. Die vorhandenen Regressionen kombinieren Regeln,
Request-/Response-Daten und erwartete Ergebnisse; beispielsweise
enthält `secruleengine.json` unterschiedliche Engine-Zustände und
erwartete HTTP-Codes. Quellen: [Regelprüfer][rules-check],
[Engine-Regressionen][engine-tests].

Für Änderungen ergibt sich daraus diese Arbeitsweise:

1. Die betroffene Direktive oder Regel auf konkrete Implementierungen
   zurückführen.
2. Erforderliche Buildfähigkeiten und den verwendeten Engine-Modus
   festhalten.
3. Eine vorhandene passende Regression lesen; bei einer Verhaltensänderung
   den konkreten beobachtbaren Fall ergänzen.
4. Ladeprüfung und Verhaltenstest getrennt ausweisen.
5. Ergebnisse und verbleibende Unsicherheiten im Arbeitsstand sichern.

Diese Schritte sind Projektarbeitsregeln. Die dazu passenden,
quellgeprüften Befehle stehen in
[BUILD_AND_TEST.md](BUILD_AND_TEST.md).

## Quellen

[engine-grammar]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1281-L1308
[limits-grammar]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1628-L1708
[rule-grammar]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1116-L1166
[connectors]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L35-L49
[transaction-header]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h
[rule-evaluation]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_operator.cc#L199-L353
[recommended-engine]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/modsecurity.conf-recommended#L1-L7
[actions]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_actions.cc#L202-L324
[engine-tests]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/test-cases/regression/secruleengine.json#L1-L123
[ctl-engine]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/ctl/rule_engine.cc#L25-L59
[engine-state]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1770-L1777
[recommended-json]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/modsecurity.conf-recommended#L25-L37
[header-variables]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L2111-L2122
[operators-grammar]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L914-L950
[rx]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/operators/rx.cc#L29-L100
[driver-rules]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/driver.cc#L76-L124
[phases]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/phase.cc#L25-L72
[transformations]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/rule_with_actions.cc#L327-L441
[lowercase]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/transformations/lower_case.cc
[pass]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/disruptive/pass.cc#L32-L40
[nolog]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/no_log.cc
[body-ctl]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/ctl/request_body_processor_json.cc#L28-L35
[variable-grammar]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1854-L1978
[arguments-rule]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/modsecurity.conf-recommended#L60-L68
[tx-collection]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/collection/collections.cc#L37-L56
[collections]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L62-L78
[body-processing]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L694-L790
[default-actions]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1217-L1273
[recommended-request]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/modsecurity.conf-recommended#L10-L68
[recommended-response]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/modsecurity.conf-recommended#L163-L187
[request-limit]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L929-L971
[response-limit]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1181-L1230
[recommended-errors]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/modsecurity.conf-recommended#L70-L98
[regex-constructor]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/utils/regex.cc#L90-L117
[regex-option]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L284-L292
[regex-test]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/test-cases/regression/operator-rx-dollar-endonly.json
[compatibility]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1748-L1785
[include]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-scanner.ll#L1285-L1342
[rules-check]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/tools/rules-check/rules-check.cc#L32-L93
