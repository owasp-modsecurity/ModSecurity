# ModSecurity mit Codex und ChatGPT bearbeiten

Hier beginnt die zusätzliche deutschsprachige Arbeitsdokumentation.

**Technische Grundlage:** offizielles
[`owasp-modsecurity/ModSecurity`, Branch `v3/master`](https://github.com/owasp-modsecurity/ModSecurity/tree/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729),
Commit `effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`, geprüft am 25.10.2026.
Die Originaldateien dieses Standes bilden die Grundlage der Erklärungen.
Neue Arbeitsanweisungen und Kontextdateien sind als eigene Ergänzungen erkennbar.

## Was du hier bekommst

| Einstieg | Inhalt |
| --- | --- |
| [AGENTS.md](AGENTS.md) | Arbeitsanweisungen für Code-Assistenten, Belegpflicht und Ablauf neuer Sitzungen |
| [.codex/README.md](.codex/README.md) | Inhaltsverzeichnis und Lesepfade |
| [Projektüberblick](.codex/PROJECT_CONTEXT.md) | Was libmodsecurity ist und wie dieses Repository abgegrenzt ist |
| [Repositorykarte](.codex/REPOSITORY_MAP.md) | Verzeichnisse, Sprachen, Builddateien und wichtige Einstiegspunkte |
| [Architektur](.codex/ARCHITECTURE.md) | Datenfluss einer HTTP-Transaktion, zentrale Klassen und Verarbeitung |
| [C++-Leitfaden](.codex/CPP_GUIDE.md) | C++17, C-Schnittstelle, Speicherbesitz und Lesen des vorhandenen Codes |
| [Funktionskarte](.codex/FEATURE_MAP.md) | Wo konkrete Funktionen implementiert und geprüft werden |
| [Regeln und Konfiguration](.codex/CONFIG_AND_RULES.md) | SecRules, Konfiguration, Operatoren und tatsächliche Grenzen |
| [Build und Tests](.codex/BUILD_AND_TEST.md) | Quellbelegte Vorbereitung, Buildwege und Testausführung |
| [Aktueller Arbeitsstand](.codex/CURRENT_STATE.md) | Was erledigt, geprüft, offen oder blockiert ist |
| [Quellen und Herkunft](.codex/PROVENANCE.md) | Upstream-Commit, Fork-Abgleich, Submodule und Belegregeln |

## Warum AGENTS.md und .codex?

`AGENTS.md` ist die kurze Anweisung am Repository-Einstieg. Codex berücksichtigt
solche Dateien entlang der Projektverzeichnisse. Die ausführlichen Kapitel
liegen hier in `.codex/`; die Startanweisung fordert gezielt zum Lesen auf.
Diese Aufteilung folgt der offiziellen
[Beschreibung von AGENTS.md](https://developers.openai.com/codex/agent-configuration/agents-md).

Der Ordner `.codex/` dient in diesem Repository als versionierte Dokumentation.
Die hier gewählten Dateinamen wie `CURRENT_STATE.md` sind eigene Konventionen.
Sie sind kein Versprechen, dass jedes Produkt alle Dateien automatisch lädt.
Es wird weder ein Modell fest eingestellt noch eine persönliche
Codex-Konfiguration überschrieben.

Ein neuer Chat kann weiterarbeiten, wenn er Zugriff auf diese Dateien hat und
sie liest. Bei normalem ChatGPT gibst du den Repository-Zugang oder die aktuellen
Dateien mit. Nicht gespeicherte Entscheidungen aus einem früheren Chat sind in
diesem System nicht vorhanden. Deshalb gehört die Aktualisierung des
Arbeitsstands zu jeder abgeschlossenen Sitzung.

## So startest du einen neuen Chat

Öffne das Repository in deiner Arbeitsumgebung beziehungsweise stelle die
Dateien im Chat bereit. Verwende dann zum Beispiel:

> Arbeite mit diesem ModSecurity-Repository. Lies zuerst AGENTS.md,
> .codex/README.md, .codex/CURRENT_STATE.md und .codex/DECISIONS.md.
> Prüfe den tatsächlichen Checkout gegen .codex/upstream-lock.json.
> Fasse den belegten Arbeitsstand und offene Punkte zusammen. Erfinde keine
> fehlenden Informationen. Meine nächste Aufgabe lautet: …

Eine ausführlichere, kopierfertige Fassung steht in
[START_NEW_CHAT.md](.codex/prompts/START_NEW_CHAT.md). Zum Ende der Arbeit gibt es
[END_SESSION.md](.codex/prompts/END_SESSION.md).

Die Dateien gehören zum vorhandenen Fork `Easton97-Jens/ModSecurity`.
[USING_THIS_FORK.md](.codex/USING_THIS_FORK.md) erklärt, wie du den Arbeitsbranch
`codex/kontext-v3` öffnest und für neue Chats verwendest.

## Kontext auf technische Konsistenz prüfen

Im Repository-Hauptverzeichnis:

```sh
python3 .codex/scripts/verify_context.py
```

Für eine unveränderte Upstream-Quellbasis zusätzlich:

```sh
python3 .codex/scripts/verify_context.py --strict-source
```

Die eigene Prüfung benötigt Python 3 und Git mit dem Schalter `--no-lazy-fetch`.
Dieser verhindert automatisches Nachladen in Partial Clones. Sie kontrolliert den fixierten
Commit, das Quellinventar, lokale Dokumentlinks und die Existenz sowie
Zeilenbereiche der commitgebundenen ModSecurity-Belege. Sie meldet Abweichungen
des Arbeitsbaums. Sie ersetzt weder das Lesen des Codes noch Build- oder Tests.

Die ehrliche Grenze dieser Dokumentation: Sie erklärt den Repositoryaufbau und
zentrale Abläufe ausführlich. Sie ist keine geprüfte Beschreibung jeder einzelnen
C++-Funktion und kein Nachweis für die Sicherheit einer Installation.
