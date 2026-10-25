# Bauen, prüfen und Fehler nachvollziehen

## Gültigkeit und Leseregel

Diese Anleitung beschreibt den gelesenen Upstream-Quellstand
`effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729` aus `owasp-modsecurity/ModSecurity`,
Branch `v3/master`. Die Quellenlinks zeigen auf diesen unveränderlichen Commit.
Die folgenden Befehle sind aus Builddateien, Testprogrammen und CI-Konfiguration
abgeleitet. Ihre Aufnahme in dieses Dokument bedeutet **nicht**, dass sie in
dieser Sitzung ausgeführt wurden oder auf deinem Rechner bereits funktionieren.
Den tatsächlich ausgeführten Prüfstand dokumentiert der Arbeitsstand des Projekts.

## 1. Was gebaut wird

Unter Unix erzeugt das Projekt über Autotools die Bibliothek
`libmodsecurity.la` und die zugehörige dynamische Bibliothek. Der oberste
Build durchläuft `others`, `src`, `doc`, `tools` sowie abhängig von den
Konfigurationsschaltern `examples` und `test`. Ein Webserver-Connector wird
dadurch nicht automatisch mitgebaut. Die Beispiele und der Regelprüfer sind
eigene Programme, die die Bibliothek benutzen.
Quellen: [oberstes Makefile][root-make], [Bibliotheksziel und öffentliche Header][src-make],
[Abgrenzung zu Connectors][readme-connectors].

| Bestandteil | Tatsächliche Anforderung oder Aufgabe | Quelle |
| --- | --- | --- |
| C- und C++-Compiler | `AC_PROG_CC` und `AC_PROG_CXX`; C++17 ist mit `AX_CXX_COMPILE_STDCXX(17, noext, mandatory)` verpflichtend. | [configure.ac, 49–63][compiler] |
| Autotools | `build.sh` ruft `libtoolize` beziehungsweise auf Darwin `glibtoolize`, `autoreconf`, `autoheader`, `automake` und `autoconf` auf. | [build.sh][bootstrap] |
| `make`, Archivierungswerkzeug, `pkg-config` | Werden im Konfigurationsablauf gesucht; `pkg-config` unterstützt die Erkennung externer Bibliotheken. | [configure.ac, 49–63][compiler], [Bibliothekserkennung][find-lib] |
| Flex und Bison | Erforderlich, wenn der Parser mit `--enable-parser-generation=yes` neu erzeugt werden soll. Standardmäßig ist diese Option aus; vorerzeugte Parserdateien liegen im Repository. | [configure.ac, 284–310][parser-option], [src/Makefile.am, 17–28][src-make] |
| Git-Submodule | `configure.ac` prüft konkrete Dateien aus libinjection und dem verschachtelten mbedTLS-Quellbaum. Fehlende Dateien beenden die Konfiguration. | [configure.ac, 65–107][submodule-checks] |

`build.sh` ist keine schreibgeschützte Bestandsprüfung: Es entfernt unter
anderem Autotools-Zwischendateien und erzeugt `src/headers.mk` erneut.
Deshalb vor dem Start vorhandene Änderungen mit `git status --short` ansehen.
Das ist eine Arbeitsregel dieser Dokumentation; die Dateieingriffe selbst
stehen in [build.sh][bootstrap].

## 2. Submodule und externe Bibliotheken

Die vier im Hauptrepository deklarierten Submodule sind:

| Pfad | Im Quellbaum belegte Rolle |
| --- | --- |
| `others/libinjection` | Eingebundene libinjection-Quellen; die Konfiguration verlangt `src/libinjection_html5.c`. |
| `others/mbedtls` | Eingebundene Mbed-TLS-Quellen; verlangt wird insbesondere `tf-psa-crypto/utilities/base64.c`, also Inhalt eines verschachtelten Quellbaums. |
| `test/test-cases/secrules-language-tests` | Zusätzliche gemeinsame SecLang-Testfälle; Unit-Test-Standardpfade und die Testsuite verweisen hierher. |
| `bindings/python` | Eigenständiges Python-Bindings-Submodul; es erscheint nicht in der `SUBDIRS`-Liste des obersten Bibliotheksbuilds. |

Quellen: [.gitmodules][submodules], [Konfigurationsprüfungen][submodule-checks],
[Unit-Test-Standardpfad][unit-default], [Testliste][test-list], [Build-Unterverzeichnisse][root-make].

Aus dem Repository-Hauptverzeichnis:

```sh
git submodule update --init --recursive
git submodule status --recursive
```

Das rekursive Initialisieren ist im Upstream-README vorgeschrieben. Ein
führendes `-` in der Statusausgabe zeigt ein nicht initialisiertes Submodul.
Die zusätzlich rekursive Statusanzeige ist hier eine gewählte Arbeitsweise,
damit auch die verschachtelten Abhängigkeiten sichtbar werden.
Quelle: [README, Git-Checkout vorbereiten][readme-build].

### Welche Bibliothek ist wofür nötig?

Die Tabelle unterscheidet die tatsächliche Buildlogik von einer gewünschten
Testumgebung. „Optional“ bedeutet hier, dass der Unix-Konfigurationsablauf die
Bibliothek nicht in jedem Fall erzwingt; es verspricht keine gleichbleibende
Funktionalität ohne diese Bibliothek.

| Bibliothek | Einordnung im gelesenen Stand | Beleg |
| --- | --- | --- |
| PCRE2 oder ausdrücklich gewähltes älteres PCRE | Der Standardpfad sucht PCRE2; `--with-pcre` wählt den anderen Pfad. `regex.h` inkludiert ohne `WITH_PCRE` direkt `pcre2.h`. Für einen nutzbaren Build ist daher die ausgewählte Regex-Abhängigkeit bereitzustellen. | [configure.ac, 147–157][regex-selection], [regex.h, 15–20][regex-header] |
| YAJL | Das README nennt YAJL verpflichtend. Die Unix-Makros lassen den automatischen Suchlauf bei Fehlen jedoch weiterlaufen; `TEST_UTILITIES` wird nur bei `YAJL_FOUND=1` aktiviert. Für den hier beschriebenen Testbuild deshalb ausdrücklich anfordern. | [README, Dependencies][readme-deps], [YAJL-Makro][yajl], [generische Erkennung][find-lib], [Testaktivierung][test-enable] |
| libcurl | Die Konfiguration definiert bei erkanntem curl `MSC_WITH_CURL`; der Upstream ordnet curl `SecRemoteRules` zu. | [configure.ac, 130–138][curl-config], [README, Operator-Abhängigkeiten][readme-deps] |
| libxml2 | Optionaler Buildbestandteil für XML-Verarbeitung; XML-Implementierung und Transaktionspfad sind durch `WITH_LIBXML2` bedingt. | [configure.ac, 141–144][xml-config], [XML-Implementierung][xml-code] |
| LMDB | Steuert die Auswahl der Collections-Implementierung: mit `WITH_LMDB` LMDB, sonst `InMemoryPerProcess`. | [ModSecurity-Konstruktor][collections] |
| Lua, SSDEEP, GeoIP/MaxMind | Werden als gesonderte Fähigkeiten gesucht. Tests können je nach eingebauten Fähigkeiten übersprungen werden; die Runner prüfen dafür `resource` und ihre eigene Ressourcenliste. | [configure.ac, 115–128][optional-config], [Regressionstest-Ressourcen][regression-resources] |

**Konkrete Folge der YAJL-Abweichung:** Ein erfolgreicher Konfigurationslauf
allein belegt nicht, dass Unit- und Regressionstestprogramme gebaut wurden.
Für den Testbuild sollen die Zusammenfassung `Test Utilities ... enabled`
und die wirklich ausgeführten Testzahlen geprüft werden. Auch ohne
Debug-Logging hat `regression_tests` einen Pfad, der eine Meldung ausgibt und
mit `0` endet, ohne die Regressionen zu prüfen. Quellen:
[Testaktivierung][test-enable], [Konfigurationszusammenfassung][configure-summary],
[NO_LOGS-Pfad][regression-main], [Testsuite-Skip-Behandlung][test-driver].

## 3. Unix-Build für die Entwicklung

Die tatsächlichen Paketnamen hängen vom Betriebssystem ab. Dieses Dokument
behauptet keine universelle Installationszeile. Im geprüften Quellstand liefern
die GitHub-Workflows konkrete Beispiele für Ubuntu sowie macOS. Sie bauen
mit GCC beziehungsweise Clang, aktivieren Assertions und führen `make check`
aus. Die Workflowdateien belegen die konfigurierte Prüfung, nicht ihren
Erfolg auf einem beliebigen Rechner oder einen hier ausgeführten CI-Lauf.
Quellen: [Linux-CI][linux-ci], [weitere Linux-/macOS-CI][new-ci].

Nach Installation der passenden Buildwerkzeuge und Entwicklungsbibliotheken
im Hauptverzeichnis des Checkouts:

```sh
git status --short
git submodule update --init --recursive
git submodule status --recursive
./build.sh
./configure --with-yajl=yes --with-pcre2=yes --enable-assertions=yes
make -j2
make check
```

Die Reihenfolge `build.sh`, `configure`, `make` und `make check` stammt aus
Upstream-README und CI. Die ausdrücklich gesetzten YAJL-/PCRE2-Schalter und
`-j2` sind die hier gewählte Entwicklungsvariante: Die beiden Bibliotheken
sollen bei Fehlen bereits in der Konfiguration auffallen, die Anzahl
paralleler Compiler bleibt überschaubar. `MSC_CHECK_LIB` behandelt `yes` als
verpflichtende Anforderung. Quellen: [README-Build][readme-build],
[Linux-CI][linux-ci], [Bibliotheksoptionen][find-lib], [PCRE2-Makro][pcre2].

Für die meisten Arbeiten am Quellcode ist zunächst kein Installationsschritt
erforderlich: Die Testprogramme verknüpfen gegen den Build unter
`src/.libs`. Falls eine lokale Installation benötigt wird, kann der
Installationspräfix bei der Konfiguration bewusst auf einen beschreibbaren
Pfad gesetzt werden, zum Beispiel:

```sh
./configure --prefix="$HOME/.local/modsecurity" \
  --with-yajl=yes --with-pcre2=yes --enable-assertions=yes
make -j2
make check
make install
```

Dieser lokale Präfix ist eine Entscheidung dieser Anleitung. Der im
Quellstand gesetzte Standardpräfix ist `/usr/local/modsecurity`;
`make install` installiert Buildprodukte und ist keine Prüfung.
Quellen: [Standardpräfix][prefix], [Test-Linkerpfade][test-make],
[Installierbare Bibliothek/Header][src-make], [Upstream-Installationsfolge][readme-build].

### Debug-Build und Parserarbeiten

Als hier abgeleitete Debug-Variante lassen sich die C- und C++-Flags beim
Konfigurieren setzen:

```sh
CFLAGS='-g -O0' CXXFLAGS='-g -O0' ./configure \
  --with-yajl=yes --with-pcre2=yes --enable-assertions=yes
make -j2
```

Die Assertion-Option ist tatsächlich implementiert: Sie setzt `-UNDEBUG`
anstelle von `-DNDEBUG`. Die zusätzlich für C++ gesetzten Debugflags sind
eine Arbeitswahl dieser Anleitung. Ob im konkreten Build weitere Flags
hinzukommen, muss an der Compilerzeile geprüft werden.
Quelle: [Assertions und globale Flags][assertions].

Bei Änderungen an der Grammatik oder am Scanner liegen die bearbeitbaren
Eingaben unter `src/parser/seclang-parser.yy` und
`src/parser/seclang-scanner.ll`. Die Erzeugung wird mit
`--enable-parser-generation=yes` aktiviert. Das Parser-Makefile enthält
zusätzlich einen Nachbearbeitungsschritt für den erzeugten Header; daher
nicht nur eine beliebige Bison-Kommandozeile ausführen und annehmen, sie
ersetzt den Projektbuild. Quellen: [Parseroption und Werkzeugsuche][parser-option],
[Parser-Makefile][parser-make].

## 4. Tests gezielt auswählen

Nach einem erfolgreichen Build mit aktiviertem `TEST_UTILITIES`, wie in
Abschnitt 3 mit ausdrücklich angefordertem YAJL, existieren im Verzeichnis
`test` die Programme `unit_tests` und `regression_tests`. Der gemeinsame
Argumentparser akzeptiert unter anderem einen Dateipfad; bei
Regressionstests ist zusätzlich die Auswahl `Datei.json:N` möglich. Die
Regressionen werden nach ihren geladenen Schlüsseln sortiert; `N` wird im
Runner ab `1` gezählt. Quellen: [Programmnamen][test-make],
[Argumentparser][test-cli], [Regressionsauswahl][regression-main].

Das Upstream-README nennt im allgemeinen Kompilierungsabschnitt einmal
`tests/`. Die vorhandenen Builddefinitionen und Dateien verwenden
`test/` ohne abschließendes `s`; für Befehle gilt dieser tatsächliche Pfad.
Quellen: [abweichende README-Stelle][readme-test-path],
[Testintegration][test-integration].

Aus dem Hauptverzeichnis für die gesamte konfigurierte Testsuite:

```sh
make check
```

Für die folgenden direkten Aufrufe zuerst in `test` wechseln. Die
Arbeitsverzeichnisse sind wichtig, weil Fixtures und Standardpfade relativ
dazu angegeben werden; genau diesen Wechsel macht auch der Shell-Testtreiber.
Quelle: [test/test-suite.sh][test-driver].

```sh
cd test
./unit_tests test-cases/unit/transformation-hash-base64.json
./regression_tests test-cases/regression/secruleengine.json
./regression_tests test-cases/regression/secruleengine.json:1
```

Das erste Beispiel wählt eine vorhandene lokale Transformations-Testdatei.
Das zweite prüft die vorhandenen Fälle zur Rule-Engine; das dritte wählt
einen Eintrag daraus. Diese Beispiele sind dokumentierte Aufrufe, kein
Testprotokoll. Quellen: [vorhandene Unit-Testdatei][hash-test],
[vorhandene Engine-Regressionen][engine-test], [Argumentauswertung][test-cli].

Ohne Dateiangabe lädt `unit_tests` standardmäßig Operatorfälle aus
`test-cases/secrules-language-tests/operators` und ergänzt bei diesem
Standardziel `transformations`. Die zusätzlichen lokalen Dateien unter
`test-cases/unit` sind damit nicht automatisch durch diesen direkten
Standardaufruf beschrieben. Für die gesamte vom Projekt aufgelistete
Suite ist `make check` mit `test/test-suite.in` maßgeblich. Quellen:
[Unit-Standardpfad][unit-default], [Zusatzladepfad][unit-main],
[Testliste][test-list], [Testintegration im Makefile][test-integration].

### Was ein Testergebnis tatsächlich aussagt

- Exitcode, Anzahl ausgeführter Fälle, übersprungene und deaktivierte Fälle
  zusammen festhalten. Die Programme ziehen übersprungene Fälle von ihren
  Fehlerzahlen ab; ein Exitcode `0` beweist daher nicht die Ausführung aller
  denkbaren Fähigkeiten. [Unit-Ergebnisbildung][unit-main],
  [Regressionsergebnisbildung][regression-main].
- Bei einem neuen Regressionstest zusätzlich prüfen, ob er in
  `test/test-suite.in` aufgenommen ist. Die Suite wird darüber eingebunden.
  [Makefile-Einbindung][test-integration], [Testliste][test-list].
- `format` ist ein schreibender Modus des Regressionstestprogramms: Er
  öffnet JSON-Dateien als Ausgabedateien. Er ist kein alternativer
  schreibgeschützter Testaufruf. [Formatierungszweig][regression-main].
- Eine vorhandene JSON-Erwartung ist ein Testvertrag; erst ein dokumentierter
  Lauf belegt, ob der aktuelle Build ihn erfüllt. Diese Unterscheidung ist
  eine Arbeitsregel für dieses Repository.

## 5. Regeldateien laden und statisch prüfen

Der Unix-Build definiert das Programm
`tools/rules-check/modsec-rules-check`. Es behandelt ein vorhandenes
Dateisystemobjekt als Eingabedatei (`RulesSet::loadFromUri`), sonst das
Argument als SecLang-Text (`RulesSet::load`). Ein negativer Laderückgabewert
wird als Fehler ausgegeben. Quellen: [Programmziel][rules-check-make],
[Implementierung][rules-check].

Beispiel aus dem Repository-Hauptverzeichnis nach dem Build:

```sh
./tools/rules-check/modsec-rules-check 'SecRuleEngine DetectionOnly'
```

Der Prüfer lädt Regeln und meldet Ladefehler; er simuliert in seinem
`main` keine HTTP-Transaktion. Erfolgreiches Laden ersetzt deshalb keine
Regression mit passenden Request-/Response-Daten. Quelle:
[vollständiger Kontrollfluss des Prüfers][rules-check].

`make check-static` verweist auf `cppcheck`; die Optionen und Unterdrückungen
stehen im obersten Makefile. `make check-style` verweist auf
`check-coding-style`, verwendet `cpplint.py` und schreibt `coding-style.txt`.
Für diese Zusatzprüfungen müssen die betreffenden Werkzeuge installiert sein.
Quelle: [statische und Stilprüfungen][static-checks].

## 6. Windows hat einen eigenen Buildweg

Der geprüfte Windows-Weg verwendet Visual-Studio-2022-Buildwerkzeuge, Conan
und CMake. `vcbuild.bat` führt `conan install` mit
`compiler.cppstd=17` aus, erzeugt mit dem Generator
`Visual Studio 17 2022` ein Projekt und baut die gewählte Konfiguration.
`build/win32/CMakeLists.txt` verlangt mindestens CMake 3.24 und C++17.
Quellen: [Windows-README][windows-readme], [vcbuild.bat][vcbuild],
[Windows-CMake][windows-cmake].

Belegte Beispielaufrufe aus einer passend vorbereiteten Windows-Umgebung
im Repository-Hauptverzeichnis:

```bat
vcbuild.bat Release x86_64 NO_ASAN
vcbuild.bat Debug x86_64 USE_ASAN
```

Die Ergebnisse liegen laut Windows-Anleitung unter
`build\win32\build\<Konfiguration>`. Zusätzliche CMake-Schalter werden
ab dem vierten Argument weitergereicht; die CI benutzt zum Beispiel
`-DWITH_LMDB=ON`. Quellen: [vcbuild.bat][vcbuild],
[Windows-README][windows-readme], [Windows-CI][windows-ci].

**Quellstand statt verallgemeinerter Anleitung:** Die Windows-README nennt
LMDB in einer Liste standardmäßig aktivierter Features. Die tatsächliche
CMake-Option `WITH_LMDB` steht jedoch auf `OFF`; Lua, libxml2, MaxMind und
curl stehen auf `ON`. Für diesen Stand gilt diese CMake-Tabelle. Die
Conan-Abhängigkeiten haben im Repository feste Versionsangaben; ihre
Auflösbarkeit wurde durch die Erstellung dieser Dokumentation nicht geprüft.
Quellen: [CMake-Optionen][windows-cmake], [abweichende README-Passage][windows-readme],
[conanfile.txt][conan].

Die Windows-CI bereitet vor `ctest -C Release --output-on-failure` unter
anderem DLLs, Testprogramme und Fixtures vor und deaktiviert einige benannte
Fälle. Daher nicht nur den CTest-Aufruf kopieren und daraus Gleichheit mit
der gesamten Linux-Prüfung ableiten. Quelle: [Windows-Testvorbereitung][windows-ci].

## 7. Arbeitsprotokoll bei Fehlern

Für eine reproduzierbare Fortsetzung sollen neue Chats diese Angaben sichern:

1. Commit, Branch, Betriebssystem und Compiler.
2. Ausgabe von `git submodule status --recursive`.
3. Exakter `configure`- oder `vcbuild.bat`-Aufruf und erkannte Fähigkeiten.
4. Erster fehlgeschlagener Befehl, Exitcode und relevante Fehlermeldung.
5. Konkrete Testdatei, Testauswahl, Fallzahlen und Skip-/Disabled-Zahlen.
6. Bestätigte Ursache oder ausdrücklich noch offene Vermutung.

Diese Liste ist die hier eingeführte Arbeitsweise. Sie ist keine Behauptung
über bereits erfolgte Builds. Die Befehle und Merkmale, auf die sie verweist,
sind in den oben verlinkten Build- und Testquellen überprüfbar.

## Quellen

[root-make]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/Makefile.am#L1-L17
[src-make]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/Makefile.am#L1-L67
[readme-connectors]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L35-L49
[compiler]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L49-L63
[bootstrap]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build.sh#L1-L33
[find-lib]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/msc_find_lib.m4#L21-L99
[parser-option]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L284-L310
[submodule-checks]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L65-L107
[submodules]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.gitmodules#L1-L12
[unit-default]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/unit/unit.cc#L52-L53
[test-list]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/test-suite.in
[readme-build]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L58-L95
[regex-selection]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L147-L157
[regex-header]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/utils/regex.h#L15-L20
[readme-deps]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L101-L134
[yajl]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/yajl.m4#L10-L11
[test-enable]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L313-L335
[curl-config]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L130-L138
[xml-config]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L141-L144
[xml-code]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/request_body_processor/xml.cc
[collections]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L62-L78
[optional-config]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L115-L128
[regression-resources]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/regression/regression.cc#L160-L185
[configure-summary]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L479-L498
[regression-main]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/regression/regression.cc#L395-L561
[test-driver]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/test-suite.sh#L1-L40
[linux-ci]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.github/workflows/ci.yml#L8-L106
[new-ci]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.github/workflows/ci_new.yml#L75-L171
[pcre2]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/pcre2.m4#L11-L13
[prefix]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L40-L42
[test-make]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/Makefile.am#L29-L145
[assertions]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L350-L358
[parser-make]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/Makefile.am#L1-L43
[test-cli]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/common/modsecurity_test.cc#L95-L173
[hash-test]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/test-cases/unit/transformation-hash-base64.json
[engine-test]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/test-cases/regression/secruleengine.json#L1-L123
[unit-main]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/unit/unit.cc#L307-L379
[test-integration]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/Makefile.am#L88-L96
[readme-test-path]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L53-L54
[rules-check-make]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/tools/rules-check/Makefile.am#L3-L9
[rules-check]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/tools/rules-check/rules-check.cc#L32-L93
[static-checks]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/Makefile.am#L58-L85
[windows-readme]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/README.md#L3-L83
[vcbuild]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/vcbuild.bat#L1-L28
[windows-cmake]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/CMakeLists.txt#L1-L149
[windows-ci]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.github/workflows/ci_new.yml#L173-L234
[conan]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/conanfile.txt#L1-L14
