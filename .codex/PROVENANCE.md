# Herkunft, Quellenstand und Belege

## 1. Festgehaltener Quellstand

| Feld | Geprüfter Wert |
| --- | --- |
| Offizielles Repository | `https://github.com/owasp-modsecurity/ModSecurity` |
| Untersuchte Entwicklungslinie | `v3/master` |
| Fixierter Commit | `effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729` |
| Git-Tree | `d626de814f96ceea7fd717d6176b30a17a86f08e` |
| Commit-Zeit laut Git | `2026-10-24T18:06:05+02:00` |
| Prüfung | 25.10.2026, Datumsbezug Europe/Berlin |
| Inventar des Hauptrepositorys | 850 Einträge: 846 Blobs und 4 Gitlinks |

Der [Commit](https://github.com/owasp-modsecurity/ModSecurity/commit/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729)
und sein [Quellbaum](https://github.com/owasp-modsecurity/ModSecurity/tree/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729)
sind die Grundlage. Die Werte wurden aus `git show`, `git rev-parse` und
`git ls-tree -r` gelesen. Das vollständige maschinenlesbare Inventar steht in
[SOURCE_INDEX.tsv](SOURCE_INDEX.tsv), die Metadaten in
[upstream-lock.json](upstream-lock.json).

„850 Einträge“ bezeichnet den Git-Tree, nicht 850 einzeln vollständig erklärte
Funktionen oder eine vollumfängliche Codeprüfung. Ein Blob kann beispielsweise
auch eine Bilddatei sein. Ein Gitlink referenziert einen eigenen Repository-Commit.

## 2. Verhältnis zum angegebenen Benutzer-Fork

Am Prüftag zeigte `Easton97-Jens/ModSecurity`, ebenfalls Branch `v3/master`,
auf `2e08f23dd50e7c0aa7c969b244b9d3a6dce4d641`. Dieser Commit ist ein Vorfahr
des oben genannten offiziellen Stands. Der Vergleich ergab **22 zusätzliche
Upstream-Commits und 0 nur auf dem verglichenen Fork-Branch vorhandene Commits**.
Das ist eine Aussage über diese zwei Branchspitzen zu diesem Zeitpunkt,
nicht über sämtliche Branches oder den zukünftigen Zustand des Forks.

Belege: [Fork-Commit](https://github.com/Easton97-Jens/ModSecurity/commit/2e08f23dd50e7c0aa7c969b244b9d3a6dce4d641),
[Vergleich der beiden Commits](https://github.com/owasp-modsecurity/ModSecurity/compare/2e08f23dd50e7c0aa7c969b244b9d3a6dce4d641...effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729).
Zusätzlich wurde nach dem Abruf der v3-Historie lokal geprüft:

```sh
git merge-base --is-ancestor \
  2e08f23dd50e7c0aa7c969b244b9d3a6dce4d641 \
  effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729
git rev-list --count \
  2e08f23dd50e7c0aa7c969b244b9d3a6dce4d641..effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729
git rev-list --count \
  effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729..2e08f23dd50e7c0aa7c969b244b9d3a6dce4d641
```

Ergebnisse in dieser Reihenfolge: Exitcode `0`, Anzahl `22`, Anzahl `0`.
Für die Arbeitsgrundlage im vorhandenen Fork wurde der offizielle Stand
gewählt. Der Arbeitsbranch `codex/kontext-v3` basiert darauf. Ein Pull Request
auf den zu Beginn beobachteten Fork-Branch umfasst daher auch die 22
Upstream-Commits; die Dokumentations-Ergänzung enthält keine eigenen
Änderungen am ursprünglichen Produktcode.

## 3. Submodule sind eigenständige Quellen

Die vier direkten Gitlinks des Hauptcommits sind:

| Pfad | Fixierter Submodul-Commit |
| --- | --- |
| `bindings/python` | `bc625d5bb0bac6a64bcce8dc9902208612399348` |
| `others/libinjection` | `211782219663f889f471650150df12b623c5766e` |
| `others/mbedtls` | `0fe989b6b514192783c469039edd325fd0989806` |
| `test/test-cases/secrules-language-tests` | `f73c73027ef49ccf99c7911744929a71d15ff419` |

Quelle für Zuordnung und URLs: [.gitmodules](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.gitmodules).
Quelle für die Commit-IDs: Gitlinks im
[fixierten Quellbaum](https://github.com/owasp-modsecurity/ModSecurity/tree/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729),
reproduzierbar mit `git ls-tree -r` und im Inventar festgehalten.

Die innere Implementierung dieser Submodule wurde für diese Dokumentation
nicht vollständig untersucht. Aussagen über ihre Verwendung stützen sich auf
die Aufrufer und Builddefinitionen im Hauptrepository. Für eine eigene Änderung
innerhalb eines Submoduls muss dessen tatsächlicher Commit separat gelesen
und geprüft werden. Das gilt auch für verschachtelte Submodule.

## 4. Welche Quelle beantwortet welche Frage?

| Frage | Bevorzugte Grundlage |
| --- | --- |
| Öffentliche Signatur | Deklaration im passenden Header und Definition der Funktion |
| Tatsächliches Verhalten | Implementierung, Aufrufer, Zustände und Fehlerpfade |
| Zulässige Regelsyntax | Scanner, Parsergrammatik und konkrete Parseraktionen |
| Ein-/ausgeschaltete Funktion | Builddefinitionen, Präprozessorschalter und Laufzeitpfad |
| Erwartetes Verhalten | Vorhandene Tests, nachvollziehbare Anforderungen |
| In dieser Umgebung erfolgreich? | Tatsächlich protokollierter Build-/Testlauf |
| Historischer Zusammenhang | Git-Commit und Diff, klar vom aktuellen Code getrennt |

README, Kommentare und Doxygen helfen beim Einstieg, können aber abweichen.
Konkrete Beispiele dafür stehen in [FACT_CHECK.md](FACT_CHECK.md). Ein
eingetragener Test und ein erfolgreicher Testlauf sind unterschiedliche Belege.

## 5. Eigene Ergänzungen und externe Produktdokumentation

`AGENTS.md`, `CODEX_START.md` und `.codex/` wurden für diesen Auftrag ergänzt.
Arbeitsabläufe, Übergabevorlagen und der Kontextprüfer sind eigene
Projektkonventionen. Sie werden nicht als vorhandene Upstream-Funktionen
oder als Anweisungen der ModSecurity-Maintainer ausgegeben.

Für die Bedeutung und Erkennung von `AGENTS.md` wurde zusätzlich die
[offizielle OpenAI-Dokumentation](https://developers.openai.com/codex/agent-configuration/agents-md)
gelesen. Sie ist ausschließlich Quelle für dieses Codex-Verhalten.
**ModSecurity-Fakten werden daraus nicht abgeleitet.**

Der Dokumentationsansatz verlangt keine bestimmte Modellversion, keinen
API-Schlüssel und keine Übernahme persönlicher Einstellungen. Die Dateien
müssen dem jeweiligen Chat oder Code-Assistenten tatsächlich zugänglich sein.

## 6. Grenzen der Konsistenzprüfung

`verify_context.py` prüft Git-Objekte, Inventar, lokale Linkziele und
commitgebundene Quellpfade einschließlich Zeilenbereichen. Aktive Fachkapitel
müssen zum fixierten Basiscommit passen. Historische Sitzungs- und
Entscheidungsbelege dürfen ihren eigenen vollständigen Commit nennen und
werden gegen dessen lokal vorhandene Git-Objekte geprüft.

Der Prüfer lädt nichts aus dem Netz nach und führt keine Buildprogramme aus.
Fehlt ein benötigtes Git-Objekt, wird das als Fehler gemeldet. Die Prüfung
erfordert eine Git-Version mit `--no-lazy-fetch`. Sie prüft keine Erreichbarkeit
der externen Websites und nicht, ob jeder Satz semantisch von seinem Link
getragen wird. Diese letzte Aufgabe bleibt eine Code- und Dokumentprüfung.

Der Ausgangsstand und alle tatsächlich ausgeführten Prüfungen stehen in
[CURRENT_STATE.md](CURRENT_STATE.md). Die Pflege bei einer neuen
Upstream-Version beschreibt [UPSTREAM_SYNC.md](UPSTREAM_SYNC.md).
