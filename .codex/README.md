# Dokumentationsindex und Lesepfade

Dieser Bereich ist eine eigene Ergänzung für die Arbeit mit Codex und ChatGPT.
Er ist auf den in [upstream-lock.json](upstream-lock.json) festgehaltenen
ModSecurity-Commit bezogen. Inhaltliche Aussagen über ModSecurity sind in den
Fachkapiteln mit dem offiziellen Quellcode verknüpft.

## Pflichtlektüre am Sitzungsanfang

1. [AGENTS.md](../AGENTS.md): Arbeitsanweisungen und Quellenregeln.
2. [CURRENT_STATE.md](CURRENT_STATE.md): tatsächlicher Arbeits- und Prüfstand.
3. [DECISIONS.md](DECISIONS.md): begründete, bereits getroffene Entscheidungen.
4. [upstream-lock.json](upstream-lock.json): maschinenlesbarer Quellenstand.

Danach die zur Aufgabe passenden Kapitel und die darin referenzierten
Implementierungen lesen. Der gespeicherte Kontext ist ein Einstieg, keine
Erlaubnis, auf eine frische Codeprüfung zu verzichten.

## Fachwissen

| Datei | Leitfrage |
| --- | --- |
| [PROJECT_CONTEXT.md](PROJECT_CONTEXT.md) | Was ist ModSecurity v3 in diesem Repository? |
| [REPOSITORY_MAP.md](REPOSITORY_MAP.md) | Wo liegen Code, Header, Buildsystem, Tests und Hilfsmittel? |
| [ARCHITECTURE.md](ARCHITECTURE.md) | Wie verläuft eine Transaktion durch Parser, Regeln und Interventionen? |
| [CPP_GUIDE.md](CPP_GUIDE.md) | Welche C++-Konstrukte und Schnittstellen muss ich hier verstehen? |
| [FEATURE_MAP.md](FEATURE_MAP.md) | Welche Dateien und Tests gehören zu einer konkreten Funktion? |
| [CONFIG_AND_RULES.md](CONFIG_AND_RULES.md) | Wie hängen Konfiguration, SecRules und Implementierung zusammen? |
| [BUILD_AND_TEST.md](BUILD_AND_TEST.md) | Wie wird vorbereitet, gebaut und aussagekräftig geprüft? |
| [FACT_CHECK.md](FACT_CHECK.md) | Welche bekannten Fallen und Widersprüche müssen geprüft werden? |

## Zusammenarbeit und dauerhafter Kontext

| Datei | Zweck |
| --- | --- |
| [WORKFLOW.md](WORKFLOW.md) | Vorgehen beim Erklären, Ändern, Prüfen und Übergeben |
| [CURRENT_STATE.md](CURRENT_STATE.md) | Kurzer aktueller Stand; bei jeder relevanten Übergabe aktualisieren |
| [DECISIONS.md](DECISIONS.md) | Entscheidung, Begründung, Quelle und Auswirkung festhalten |
| [BACKLOG.md](BACKLOG.md) | Echte offene Aufgaben, Unsicherheiten und Blockaden |
| [HANDOFF_TEMPLATE.md](HANDOFF_TEMPLATE.md) | Vorlage für eine nachvollziehbare Sitzungsübergabe |
| [sessions/README.md](sessions/README.md) | Ablagekonvention für spätere Sitzungsprotokolle |
| [prompts/START_NEW_CHAT.md](prompts/START_NEW_CHAT.md) | Starttext für Codex oder ChatGPT |
| [prompts/END_SESSION.md](prompts/END_SESSION.md) | Abschluss mit aktualisierten Kontextdateien |
| [UPSTREAM_SYNC.md](UPSTREAM_SYNC.md) | Einen neueren offiziellen Stand kontrolliert übernehmen |
| [USING_THIS_FORK.md](USING_THIS_FORK.md) | Den Arbeitsbranch im vorhandenen Fork öffnen und für neue Chats verwenden |

## Herkunft und automatische Konsistenzprüfung

| Datei | Bedeutung |
| --- | --- |
| [PROVENANCE.md](PROVENANCE.md) | Herkunft, Quellenpriorität, Forkvergleich und Grenzen |
| [upstream-lock.json](upstream-lock.json) | Commit, Git-Tree, Beobachtungsdatum und Submodule |
| [SOURCE_INDEX.tsv](SOURCE_INDEX.tsv) | Vollständiges Inventar des fixierten Hauptrepository-Git-Trees |
| [scripts/verify_context.py](scripts/verify_context.py) | Eigene lokale Prüfung von Inventar, Verweisen und Quellabweichungen |

`SOURCE_INDEX.tsv` ist ein Inventar, keine Aussage darüber, dass jede Datei
inhaltlich vollständig untersucht wurde. Ein Submodul erscheint als Gitlink
auf einen Commit; seine Dateien sind nicht Teil dieses Hauptrepository-Inventars.

## Lesepfade nach Ziel

**Ich lerne das Projekt kennen:** PROJECT_CONTEXT → REPOSITORY_MAP → ARCHITECTURE
→ CPP_GUIDE. Anschließend ein konkretes Beispiel und die referenzierte
Implementierung nebeneinander lesen.

**Ich untersuche einen Fehler:** CURRENT_STATE → FEATURE_MAP → betroffene
Implementierung und Aufrufer → vorhandene Tests → BUILD_AND_TEST. Erst nach
Reproduktion die Ursache und eine Änderung festlegen.

**Ich ändere eine Regel oder Konfiguration:** CONFIG_AND_RULES → Parsergrammatik
→ konkrete Aktion/Operator/Variable → passende Regression. Die Existenz eines
Schlüsselworts oder einer Klasse genügt nicht als Beleg für Unterstützung.

**Ich aktualisiere Upstream:** PROVENANCE → UPSTREAM_SYNC → betroffener Diff →
Fachkapitel und Belege aktualisieren → Prüfungen → CURRENT_STATE.

**Ich wechsle den Chat:** CURRENT_STATE und DECISIONS aktualisieren, bei Bedarf
Sitzungsübergabe schreiben, Änderungen speichern. Im neuen Chat den Starttext
verwenden und den tatsächlichen Zugriff auf die Dateien prüfen lassen.
