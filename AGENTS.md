# Arbeitsanweisungen für Codex und andere Code-Assistenten

## Auftrag und Sprache

Dieses Repository enthält den offiziellen ModSecurity-v3-Quellcode sowie eine
zusätzliche, deutschsprachige Arbeitsdokumentation. Erkläre auf Deutsch; lasse
Symbole, Dateinamen und Befehle unverändert. Die Ergänzungen in `AGENTS.md`,
`CODEX_START.md` und `.codex/` sind Projektkonventionen für diese Arbeitskopie,
keine offiziellen Vorgaben des ModSecurity-Projekts.

Die wichtigste Anforderung des Eigentümers lautet: **Nichts erfinden.**
Technische Aussagen über ModSecurity müssen aus dem tatsächlich gelesenen
Quellcode des festgehaltenen Upstream-Stands oder aus einem ausdrücklich
benannten, überprüften neueren Stand hervorgehen.

## Start jeder neuen Sitzung

1. Lies diese Datei, [`.codex/README.md`](.codex/README.md),
   [`.codex/CURRENT_STATE.md`](.codex/CURRENT_STATE.md) und
   [`.codex/DECISIONS.md`](.codex/DECISIONS.md).
2. Prüfe Arbeitsverzeichnis, aktuellen Branch, `git status --short` und
   `git rev-parse HEAD`. Erhalte fremde oder noch nicht eingecheckte Änderungen.
3. Lies [`.codex/upstream-lock.json`](.codex/upstream-lock.json). Unterscheide
   den dort fixierten Upstream-Commit vom aktuellen Arbeitscommit.
4. Führe bei vorhandenem Python 3 und Git
   `python3 .codex/scripts/verify_context.py` aus. Die Prüfung belegt Pfade,
   Referenzen und Abweichungen; sie beweist keine inhaltliche Fehlerfreiheit.
5. Wähle anhand des Dokumentationsindex nur die zum Auftrag passenden Kapitel.
   Öffne anschließend die betroffenen Implementierungen, Aufrufer und Tests.
6. Fasse knapp zusammen: Auftrag, geprüfter Stand, relevante Vorarbeit,
   offene Fragen und nächster Schritt. Eine fehlende Information wird als
   fehlend benannt, nicht durch eine plausible Geschichte ersetzt.

Bei reinem Chat-Zugriff ohne Checkout: dieselben Dateien über den tatsächlich
verfügbaren Repository-Zugang lesen. Ist eine Datei nicht zugänglich, benenne
die Lücke und fordere genau diese Datei an. Behaupte niemals, sie gelesen zu
haben. Ein Link allein überträgt weder Dateien noch vorherige Chatverläufe.

## Quellen und Belegpflicht

- Maßgeblich sind Implementierung, öffentliche Header, Builddefinitionen,
  Parsergrammatik und tatsächlich vorhandene Tests am genannten Commit.
- Zitiere bei technischen Feststellungen `Pfad + Symbol` und möglichst einen
  GitHub-Link mit vollständiger Commit-ID und passenden Zeilen.
- Trenne **belegt**, **abgeleitet**, **Vorschlag** und **ungeprüft**. Ein Testfall
  beschreibt erwartetes Verhalten; erst ein dokumentierter Lauf belegt, dass
  er in der geprüften Umgebung bestanden wurde.
- Prüfe bei widersprüchlichen Kommentaren oder README-Beispielen den Code.
  Vermerke Abweichungen in [`.codex/FACT_CHECK.md`](.codex/FACT_CHECK.md).
- Aus einer vorhandenen Datei folgt keine nutzbare Funktion. Prüfe auch
  Parserzulassung, Buildschalter, Abhängigkeiten, Implementierung und Tests.
- Vermische `v2/master`, `v3/master`, externe Connectoren, das Core Rule Set
  und den eigenen Fork nicht. Untersuche externe Projekte nur, wenn der
  Auftrag sie umfasst, und kennzeichne deren eigenständigen Quellenstand.
- Erfinde weder CVEs, Sicherheitsgarantien, Benchmarks, API-Signaturen,
  Plattformunterstützung noch erfolgreiche Build- oder Testergebnisse.
- Bei jeder Upstream-Aktualisierung sind die betroffenen Dokumente und Belege
  neu zu prüfen. Eine neue Commit-ID allein macht alte Erklärungen nicht aktuell.

## Orientierung im Code

Die folgenden Einstiegspunkte sind am fixierten Stand vorhanden; ihre
detaillierten Belege stehen in den verlinkten Kapiteln.

| Aufgabe | Zuerst lesen |
| --- | --- |
| Gesamtaufbau und Dateisystem | [Repositorykarte](.codex/REPOSITORY_MAP.md), [Funktionskarte](.codex/FEATURE_MAP.md) |
| HTTP-Ablauf, Regeln, Intervention | [Architektur](.codex/ARCHITECTURE.md), `src/transaction.cc`, `src/rules_set.cc` |
| C++/C-API, Besitz und Lebensdauer | [C++-Leitfaden](.codex/CPP_GUIDE.md), `headers/modsecurity/` und passende Implementierung |
| SecRules und Konfiguration | [Regeln und Konfiguration](.codex/CONFIG_AND_RULES.md), `src/parser/seclang-parser.yy`, `src/parser/seclang-scanner.ll` |
| Kompilierung und Tests | [Build und Tests](.codex/BUILD_AND_TEST.md), `configure.ac`, `Makefile.am`, `test/Makefile.am` |
| Neue Sitzung oder Übergabe | [Sitzungsablauf](.codex/WORKFLOW.md), [Übergabevorlage](.codex/HANDOFF_TEMPLATE.md) |

## Regeln für Änderungen

Arbeite innerhalb des konkreten Auftrags eigenständig weiter. Frage nach,
wenn eine offene Entscheidung Verhalten, öffentliche Schnittstellen,
Zielrepository oder Umfang wesentlich verändern würde. Eine vorgeschlagene
Änderung ist noch keine abgeschlossene Änderung.

Halte Änderungen klein und thematisch zusammenhängend. Beachte
[`.editorconfig`](.editorconfig) und den vorhandenen Stil. Formatiere keine
unbeteiligten Dateien um. Beachte das im Build festgelegte C++17-Niveau;
prüfe Header und Implementierung zusammen. Übernimm keine alten
README-Codebeispiele ungeprüft.

Unterscheide generierten Parsercode von seinen Quellen `.ll` und `.yy`.
Ändere Submodule nicht beiläufig und führe kein `git submodule update --remote`
als gewöhnlichen Vorbereitungsschritt aus. Verwende die vom Hauptrepository
festgelegten Submodule-Commits.

Bei Codeänderungen prüfe besonders Lebensdauer von `ModSecurity`, `RulesSet`
und `Transaction`, C/C++-Schnittstellen, explizite Datenlängen, Phasenfolge,
Interventionen, Feature-Schalter und Fehlerpfade. Leite konkrete Anforderungen
aus der betroffenen Implementierung ab; unterstelle keine globale
Threadsicherheit oder vollständige C/C++-API-Gleichheit.

## Prüfung und Abschluss

Für einen vorbereiteten Unix-Checkout nennt das Projekt den Ablauf
`git submodule update --init --recursive`, `./build.sh`, `./configure`, `make`
und `make check`. Voraussetzungen, gezielte Tests und Plattformunterschiede
stehen in [Build und Tests](.codex/BUILD_AND_TEST.md). Eine Installation ins
System ist für eine Dokumentationsaufgabe nicht nötig.

Verwende die zum Auftrag passenden vorhandenen Prüfungen. Bei einer
Dokumentationsänderung sind Quellen-, Link- und Diff-Prüfungen sinnvoll;
behaupte dadurch keinen erfolgreichen C++-Build. Bei Verhaltensänderungen
prüfe gezielt das betroffene Verhalten und die relevanten Regressionen.
Beachte insbesondere deaktivierte Tests bei fehlendem YAJL.

Notiere bei jedem tatsächlich ausgeführten Prüflauf Befehl, Arbeitsverzeichnis,
Ergebnis/Exitcode und relevante Einschränkungen. Unterscheide Fehler,
übersprungene Tests und nicht ausgeführte Prüfungen.

Vor der Übergabe:

1. Aktualisiere [`.codex/CURRENT_STATE.md`](.codex/CURRENT_STATE.md) mit dem
   tatsächlichen Stand, den Änderungen und dem nächsten konkreten Schritt.
2. Trage neue begründete Entscheidungen in
   [`.codex/DECISIONS.md`](.codex/DECISIONS.md) ein; keine erfundene Vorgeschichte.
3. Pflege [`.codex/BACKLOG.md`](.codex/BACKLOG.md) nur mit echten offenen Aufgaben.
4. Dokumentiere eine Sitzung bei Bedarf anhand der Übergabevorlage unter
   `.codex/sessions/`. Halte sensible Anfragen, Zugangsdaten und private Logs
   aus der versionierten Dokumentation heraus.
5. Prüfe den Diff und melde Ergebnis, Belege, Prüfungen und verbleibende Lücken.
   Lade Änderungen nur in das vom Nutzer beauftragte Ziel hoch.
