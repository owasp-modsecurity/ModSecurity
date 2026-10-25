# Den offiziellen v3-Stand kontrolliert aktualisieren

Dies ist ein gewählter Arbeitsablauf für diese Dokumentationsgrundlage.
Er aktualisiert sich nicht selbst. Der anfänglich geprüfte Stand ist in
[upstream-lock.json](upstream-lock.json) fixiert; historische Herkunft und
Branchvergleich stehen in [PROVENANCE.md](PROVENANCE.md).

## 1. Erst den bestehenden Zustand sichern und verstehen

Wenn eine Upstream-Aktualisierung beauftragt wird, zunächst `AGENTS.md`,
`CURRENT_STATE.md` und die Entscheidungen lesen. Dann Arbeitsbaum und Remotes
ansehen:

```sh
git status --short
git branch --show-current
git remote -v
git rev-parse HEAD
python3 .codex/scripts/verify_context.py
```

Vorhandene Änderungen bleiben erhalten. Nicht automatisch auf eine neue
Branchspitze zurücksetzen. Prüfen, ob `upstream` tatsächlich auf das offizielle
Repository zeigt. Den Remote-Namen nicht bloß aufgrund dieser Anleitung annehmen.

## 2. Einen neuen offiziellen Stand ermitteln

Nach Bestätigung des Remotes ist der folgende Abruf ein bewusster
Netzwerkzugriff:

```sh
git fetch upstream
git rev-parse upstream/v3/master
```

Die bisherige Commit-ID aus `upstream-lock.json` und die tatsächlich
zurückgegebene neue Commit-ID verwenden, um `git log` und `git diff` zwischen
genau diesen Ständen zu lesen. Zuerst prüfen, ob der alte Stand Vorfahr des
neuen ist. Eine Abweichung benötigt eine Erklärung und darf nicht als
gewöhnliches Fortschreiben übergangen werden.

Der Name `v3/master` ist beweglich. Die neue vollständige Commit-ID wird erst
nach ihrer Prüfung zum neuen Dokumentationsstand. Ein Versionsmakro im Header
oder eine README-Aussage allein beweist weder ein Release noch getestete
Kompatibilität.

## 3. Codeübernahme und Dokumentation zusammen prüfen

Die eigentliche Übernahme in den Arbeitsbranch hängt vom beauftragten
Git-Arbeitsablauf und vorhandenen Änderungen ab. Danach:

1. Geänderte öffentliche Header, Implementierungen, Grammatik, Builddateien,
   Submodule-Pins und Tests lesen.
2. Betroffene Fachkapitel anhand des neuen Codes überarbeiten. Besonders
   Aussagen zu Phasen, Eigentum, optionalen Funktionen und Tests kontrollieren.
3. Quellenlinks auf überprüfte Zeilen am neuen Commit setzen. Nicht lediglich
   die alte SHA-Zeichenfolge pauschal ersetzen.
4. Gelöschte oder umbenannte Dateien sowie geänderte Submodule ausdrücklich
   im Inventar und den Erklärungen berücksichtigen.
5. Historische Sitzungsnotizen auf ihrem damaligen Stand belassen. Für
   aktuelle Fakten auf die überarbeiteten Fachkapitel verweisen.

## 4. Die Herkunftsdaten neu erzeugen

`upstream-lock.json` enthält Commit, Tree, Beobachtungsdatum, Inventarzahlen
und Submodule-Pins. Diese Daten stammen aus der tatsächlichen Git-Historie,
nicht aus einer geschätzten Versionsnummer. `SOURCE_INDEX.tsv` besitzt die
Spalten `mode`, `type`, `object`, `path` und enthält alle Einträge des mit
`git ls-tree -r` ausgelesenen neuen Basiscommits.

Die untenstehende Python-Sequenz regeneriert **nur das Inventar**, nachdem der
neue geprüfte Commit bereits bewusst in `upstream-lock.json` eingetragen wurde.
Sie aktualisiert weder Erklärungen noch Metadaten oder Submodule automatisch.

```sh
python3 - <<'PY'
import json
import subprocess
from pathlib import Path

lock = json.loads(Path('.codex/upstream-lock.json').read_text(encoding='utf-8'))
commit = lock['upstream']['commit']
raw = subprocess.check_output(['git', '--no-lazy-fetch', 'ls-tree', '-r', '-z', commit])
rows = ['mode\ttype\tobject\tpath\n']
for item in raw.split(b'\0'):
    if item:
        metadata, path = item.decode('utf-8').split('\t', 1)
        mode, kind, oid = metadata.split()
        rows.append(f'{mode}\t{kind}\t{oid}\t{path}\n')
Path('.codex/SOURCE_INDEX.tsv').write_text(''.join(rows), encoding='utf-8')
PY
```

Dieser Wartungsbefehl schreibt die genannte Datei. Der gewöhnliche
`verify_context.py`-Lauf liest dagegen nur.

## 5. Abschließend prüfen und übergeben

Die Kontextprüfung muss mit den aktualisierten Daten bestehen. Bei
absichtlichen Produktänderungen meldet der normale Lauf die Abweichungen;
`--strict-source` passt nur, wenn außerhalb der Kontextdateien exakt der
fixierte offizielle Quellstand gelten soll.

Danach die zur Codeänderung passenden Build- und Regressionstests aus
[BUILD_AND_TEST.md](BUILD_AND_TEST.md) wirklich ausführen und ihren Umfang
protokollieren. `CURRENT_STATE.md`, Entscheidungen und offene Aufgaben
aktualisieren. Ein neuer Quellenstand darf nicht mit den Testergebnissen
eines alten Stands als frisch geprüft dargestellt werden.
