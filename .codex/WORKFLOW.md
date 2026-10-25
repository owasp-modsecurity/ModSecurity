# Arbeitsablauf für nachvollziehbare Sitzungen

Dieses Kapitel beschreibt die für diese Arbeitskopie gewählte Zusammenarbeit.
Es ist keine im ModSecurity-Code vorhandene Produktfunktion.

## 1. Auftrag und Ausgangspunkt feststellen

Lies die Startdateien aus [AGENTS.md](../AGENTS.md), prüfe Branch und Status und
nenne den betroffenen Auftrag. Vergleiche den aktuellen Checkout mit
[upstream-lock.json](upstream-lock.json). Ältere Notizen gelten nur für ihren
genannten Stand. Prüfe, ob seitdem relevante Dateien geändert wurden.

Halte Unterschiede zwischen Nutzerentscheidung, Codebefund und eigener
Arbeitshypothese sichtbar. Bei einer offenen Entscheidung mit wesentlichem
Einfluss frage nach; eine Routineentscheidung innerhalb des Auftrags kann
begründet getroffen und dokumentiert werden.

## 2. Eine Erklärung aus dem Code ableiten

Suche zunächst konkrete Symbole und Dateinamen, zum Beispiel mit `rg`.
Lies nicht nur die Funktionsdeklaration, sondern auch Implementierung,
Aufrufstellen, Fehlerpfade und passende Tests. Bei optionalen Funktionen
gehören Builddefinition und Parserzulassung dazu.

Eine hilfreiche Erklärung beantwortet in dieser Reihenfolge:

1. Welchen Zweck hat die Funktion in diesem Repository?
2. Welche Daten erhält sie, und wer besitzt diese Daten?
3. Welche Zustände und Rückgaben kann sie verändern?
4. Wer ruft sie in welcher Reihenfolge auf?
5. Welche Bedingungen, Feature-Schalter oder Grenzen gelten?
6. Welcher Test beschreibt das erwartete Verhalten, und wurde er ausgeführt?

Verwende einen Link auf die vollständige Commit-ID. Bewegliche Branchlinks
sind zum Navigieren geeignet, aber keine stabile Grundlage einer Erklärung.
Eine aus mehreren Funktionen abgeleitete Schlussfolgerung wird als Ableitung
gekennzeichnet und nennt ihre Prämissen.

## 3. Änderungen durchführen

Formuliere vorab das gewünschte beobachtbare Verhalten. Begrenze Änderungen auf
die betroffenen Dateien. Prüfe vorhandene Anweisungen in deren Verzeichnissen.
Lass unbeteiligte Änderungen bestehen und führe keinen unbeauftragten Umbau aus.

Bei einem Fehler ist ein vorhandener oder gezielt ergänzter Regressionstest
sinnvoll, der den Fehler tatsächlich erkennen kann. Für reine Erklärungen und
Dokumentkorrekturen reichen entsprechende Quellen- und Dokumentprüfungen.
Ein Test nur zur Bestätigung des gerade geschriebenen Implementierungsdetails
ist kein Ersatz für eine Anforderung.

## 4. Prüfergebnisse festhalten

| Feld | Beispiel der zu erfassenden Information |
| --- | --- |
| Ziel | Welches Verhalten oder welche Dokumentkonsistenz wurde geprüft? |
| Stand | Commit, Branch und relevante uncommittete Änderungen |
| Umgebung | Tatsächlich relevante Plattform und verfügbare Abhängigkeiten |
| Befehl | Exakter ausgeführter Befehl, keine nachträglich idealisierte Variante |
| Arbeitsverzeichnis | Relativ zum Repository oder als klarer lokaler Pfad |
| Ergebnis | Exitcode, wichtige Ausgabe, ausgeführte und übersprungene Tests |
| Grenze | Was dieser Lauf nicht abdeckt oder was blockiert war |

Der Kontextprüfer ist eine selbst ergänzte Hilfe und darf nicht als
Upstream-Test ausgegeben werden. Ein erfolgreicher Testlauf ist außerdem kein
Beweis für sämtliche Plattformen, Konfigurationen oder Sicherheitsmerkmale.

## 5. Den nächsten Chat vorbereiten

`CURRENT_STATE.md` soll kurz und aktuell bleiben. Dort stehen der letzte
belastbare Stand, offene Punkte und genau der nächste sinnvolle Schritt.
Längere frühere Sitzungen gehören nach `sessions/`.

`DECISIONS.md` enthält bleibende Entscheidungen mit Begründung. `BACKLOG.md`
enthält reale Restaufgaben. Dieselbe lange Erklärung wird nicht in allen drei
Dateien dupliziert; stattdessen auf das passende Kapitel verweisen.

Verwende bei einer Übergabe [HANDOFF_TEMPLATE.md](HANDOFF_TEMPLATE.md). Speichere
den Kontext in der tatsächlichen gemeinsamen Arbeitskopie beziehungsweise im
beauftragten Repository. Eine nur im Chat ausgesprochene Anweisung ist noch
keine aktualisierte Projektdatei.

## Grenzen des Verfahrens

Dateien geben neuen Sitzungen einen nachprüfbaren Ausgangspunkt. Sie lesen sich
nicht selbst, aktualisieren sich nicht automatisch und können veralten.
Darum beginnt jede Sitzung mit Zugriffskontrolle, Standprüfung und gezieltem
Nachlesen. Das reduziert wiederholte Grundlagenarbeit, ohne fehlendes Wissen
als Erinnerung auszugeben.
