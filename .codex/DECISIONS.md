# Entscheidungen

Dies sind die Entscheidungen für die am 25.10.2026 erstellte Arbeitsgrundlage.
Es wird keine Vorgeschichte aus früheren Chats angenommen. Spätere Einträge
sollen Datum, Entscheidung, Begründung, betroffenen Stand und Folgen nennen.

## D001 – Offizielles v3/master als technische Grundlage

**Entscheidung:** Die Dokumentation bezieht sich zunächst auf den offiziellen
Commit `effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`.

**Begründung:** Der Nutzer verlangt Fakten aus dem offiziellen ModSecurity-v3-
Master. Der angegebene Fork liegt beim geprüften Branchvergleich 22 Commits
dahinter. Der Vergleich und seine Grenze stehen in [PROVENANCE.md](PROVENANCE.md).

**Folge:** Eine spätere Änderung des Quellstands benötigt einen erneuten
Abgleich der Erklärungen und Quellen. Der bereits vorhandene Fork wird durch
das Anlegen der neuen Arbeitsgrundlage nicht automatisch aktualisiert.

## D002 – Arbeitsanweisung kurz, Fachwissen in eigenen Kapiteln

**Entscheidung:** `AGENTS.md` legt Vorgehen und Leseauftrag fest. Die
ausführlichen Erklärungen liegen in `.codex/` und werden nach Aufgabe gelesen.

**Begründung:** Wiederverwendbarer Kontext muss auffindbar bleiben. Eine
einzige sehr lange Startanweisung würde Orientierung und fachliche Details
vermischen. Der [Index](README.md) führt gezielt zu den relevanten Kapiteln.

**Folge:** Neue Chats müssen den aktuellen Arbeitsstand lesen und bei
Detailfragen den Code erneut öffnen. Vollständiges automatisches Laden aller
Dokumente oder Erinnern aller vorherigen Gespräche wird nicht behauptet.

## D003 – Deutsch erklären, echte Codebezeichner erhalten

**Entscheidung:** Prosa und Arbeitsanweisungen sind deutsch; API-Namen,
Dateipfade, Befehle und Fehlermeldungen behalten ihre tatsächliche Schreibweise.

**Begründung:** Der Nutzer fragt auf Deutsch. Unveränderte Symbole ermöglichen
die direkte Suche und Prüfung im Repository.

## D004 – Quellbelege und Empfehlungen sichtbar unterscheiden

**Entscheidung:** ModSecurity-Erklärungen verweisen auf konkrete Quellstellen
des fixierten Commits. Eigene Arbeitsabläufe und Lernhinweise werden als
solche ausgewiesen. Bekannte Konflikte stehen in [FACT_CHECK.md](FACT_CHECK.md).

**Begründung:** Der Nutzer verlangt ausdrücklich, nichts zu erfinden oder
ungeprüft zu behaupten. Eine plausibel klingende README-Zusammenfassung allein
würde diese Anforderung nicht erfüllen.

## D005 – Originalquellcode für die Dokumentationsgrundlage erhalten

**Entscheidung:** Diese Aufgabe ergänzt `AGENTS.md`, `CODEX_START.md` und
`.codex/`. Die bestehende Implementierung, Builddefinitionen, Tests und
Originaldokumente des Basiscommits bleiben für diesen Auftrag unverändert.

**Begründung:** Der Auftrag ist eine verständliche, weiterverwendbare
Arbeitsgrundlage. Korrekturen am Produktverhalten wären eigene Änderungen
mit eigenen Anforderungen und Prüfungen.

**Folge:** Die strikte Kontextprüfung kontrolliert diese Grenze. Dokumentierte
Upstream-Unstimmigkeiten werden damit nicht automatisch im Produkt behoben.

## D006 – Keine erfundenen Prüferfolge

**Entscheidung:** Quellen-/Dokumentprüfungen und ModSecurity-Build/-Tests werden
separat benannt. Nur tatsächlich ausgeführte Befehle erscheinen als
ausgeführt; nicht ausgeführte Prüfungen bleiben erkennbar.

**Begründung:** Quellenlektüre belegt bestimmte Codeeigenschaften, aber keinen
erfolgreichen Lauf auf der späteren Zielplattform.

## D007 – Den vorhandenen Fork verwenden

**Entscheidung:** Nach der ausdrücklichen Präzisierung des Nutzers gehören
die Dateien in `Easton97-Jens/ModSecurity`. Arbeitsbranch ist
`codex/kontext-v3`; ein Pull Request richtet sich an `v3/master` dieses Forks.

**Begründung:** Der Nutzer möchte seinen bestehenden Fork als Arbeitsort
verwenden. Die dokumentierte offizielle v3-Basis liegt 22 Commits vor dessen
zu Beginn beobachtetem Stand. Diese Upstream-Differenz wird im Pull Request
zusammen mit den Kontextdateien sichtbar gemacht.

**Folge:** Vor der Übernahme in `v3/master` kann auf dem Arbeitsbranch gelesen
und gearbeitet werden. Ein Merge ist ein gesonderter Schritt. Der tatsächliche
Branch-/PR-Stand wird in [CURRENT_STATE.md](CURRENT_STATE.md) gepflegt.
