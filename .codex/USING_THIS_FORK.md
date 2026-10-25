# Mit dem vorhandenen ModSecurity-Fork arbeiten

Ziel dieser Arbeitsgrundlage ist der vorhandene Fork
[`Easton97-Jens/ModSecurity`](https://github.com/Easton97-Jens/ModSecurity).
Die Kontextdateien werden auf dem Arbeitsbranch `codex/kontext-v3`
bereitgestellt. Der Pull Request auf `v3/master` macht die vorgeschlagenen
Änderungen sichtbar; sein tatsächlicher Stand steht in
[CURRENT_STATE.md](CURRENT_STATE.md).

## 1. Den richtigen Branch auswählen

Für einen neuen lokalen Checkout:

```sh
git clone --branch codex/kontext-v3 https://github.com/Easton97-Jens/ModSecurity.git ModSecurity
cd ModSecurity
git status --short
git log -1 --oneline
python3 .codex/scripts/verify_context.py --strict-source
```

Bei einer bestehenden Arbeitskopie zunächst deren Änderungen und Remotes
prüfen. Wenn `origin` tatsächlich auf diesen Fork zeigt und der Arbeitsbaum
einen Branchwechsel zulässt:

```sh
git fetch origin
git switch codex/kontext-v3
```

Diese Schritte setzen einen bereits vorhandenen entfernten Arbeitsbranch
voraus. Die Kontextprüfung benötigt Python 3 und Git mit `--no-lazy-fetch`.
Lies danach [CODEX_START.md](../CODEX_START.md) und verwende den
[Starttext](prompts/START_NEW_CHAT.md) für einen neuen Chat.

## 2. Quellenstand und Pull Request verstehen

Die technische Dokumentation beschreibt den offiziellen v3-Commit
`effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`. Der zu Beginn beobachtete
`v3/master`-Branch des Forks lag bei
`2e08f23dd50e7c0aa7c969b244b9d3a6dce4d641` und damit 22 Upstream-Commits
dahinter. Der Arbeitsbranch enthält deshalb den dokumentierten offiziellen
Stand und die zusätzlichen Kontextdateien.

Der Pull Request auf den ursprünglichen Fork-Stand umfasst sowohl diese
Upstream-Differenz als auch die Dokumentationsgrundlage. Das ist bei der
Übernahme zu berücksichtigen. [PROVENANCE.md](PROVENANCE.md) enthält die
genauen Quellen und den Vergleich. Eigene Produktcodeänderungen sind nicht
Bestandteil der Kontext-Ergänzung.

Solange der Pull Request nicht übernommen ist, muss der neue Chat den
Arbeitsbranch auswählen, um `AGENTS.md` und `.codex/` zu lesen. Nach einer
Übernahme sind die Dateien auch im dann aktualisierten Zielbranch verfügbar.
Ein bloßer Link auf den Fork garantiert nicht, dass ein Chat den richtigen
Branch geöffnet hat; deshalb Branch und Commit ausdrücklich prüfen.

## 3. Build und Submodule

Vor einem Build die in [BUILD_AND_TEST.md](BUILD_AND_TEST.md) beschriebenen
Werkzeuge und Bibliotheken für die tatsächliche Plattform bereitstellen.
Die vom Hauptrepository festgelegten Submodule werden mit
`git submodule update --init --recursive` initialisiert. Dieser Schritt ist im
offiziellen
[Git-Checkout-Ablauf](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L65-L98)
beschrieben. Die konkreten Gitlinks stehen in
[upstream-lock.json](upstream-lock.json).

Die enthaltenen Buildanweisungen wurden aus den Quellen abgeleitet. Reale
Build- und Testergebnisse der Zielumgebung müssen als neuer Prüfstand
festgehalten werden. Das Vorhandensein dieses Branches ist kein Nachweis
eines ausgeführten C++-Builds.

## 4. Neue Chats und spätere Änderungen

Im Chat zuerst den ausgewählten Branch sowie `AGENTS.md`, den Index und
`CURRENT_STATE.md` nennen beziehungsweise bereitstellen. Danach die konkrete
Aufgabe formulieren. Nach einer Sitzung den tatsächlichen Arbeitsstand,
Entscheidungen und offenen Punkte speichern. Das Verfahren steht in
[WORKFLOW.md](WORKFLOW.md); für eine Upstream-Aktualisierung gilt
[UPSTREAM_SYNC.md](UPSTREAM_SYNC.md).
