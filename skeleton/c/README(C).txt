Dieser Ordner beinhaltet ein Skelett für einen SPL-Compiler in C.
Im Laufe der 6 Praktikumsaufgaben stellen Sie diesen SPL-Compiler fertig.
Dafür enthält dieses Dokument einige wichtige Hinweise!
Lesen Sie auch das nicht sprachspezifische README, das oberhalb dieses Ordners liegt.


1. Die Ordnerstruktur

Dieser Ordner enthält einen Unterordner 'src'.
Er enthält sowohl die Header- als auch die Quelldateien des Skeletts in einzelnen Unterordnern.
Beachten Sie, dass Sie in der Regel nur die Dateien des 'phases' Unterordners ergänzen müssen.
Alle anderen Dateien beinhalten ausschließlich Librarycode, den Sie benutzen werden.
Die Dateien und Ordner, und ab welcher Aufgabe sie relevant werden, sind im unteren Diagramm kurz beschrieben.
Für Details zu den einzelnen Quelldateien schauen Sie sich bitte die Dokumentation innerhalb der Dateien an.

src/
    phases/                     -- Alle Dateien, die von Ihnen ergänzt werden müssen. Die Namen der Unterordner geben das zugehörige Aufgabenblatt an.
        /_01_scanner/           -- Die Definition des Scanners (PA1)
        /_02_03_parser          -- Die Definition des Parsers (PA2 und PA3)
        /_04a_tablebuild        -- Der Code für die Konstruktion der Symboltabellen (PA4 Teil A)
        /_04b_semant            -- Der Code für die semantische Prüfung der Prozedurrümpfe (PA4 Teil B)
        /_05_varalloc           -- Der Code für die Festlegung von Speicherplätzen in Stackframes (PA5)
        /_06_codegen            -- Der Code für die Eco32-Assemblercodegenerierung (PA6)
    /absyn                      -- Die Definitionen des abstrakten Syntaxbaums (AST). Ab PA3
    /table                      -- Die Definitionen der Symboltabelle und ihrer Einträge. Ab PA4 Teil A
    /types                      -- Die Definitionen von SPL-Typgraphen. Ab PA4 Teil A
    /util
        /ascii_table.[c|h]      -- Enthält Definitionen, die die bereits implementierte Textausgabe von PA5 (--vars) unterstützen. Für Ihre Implementierung nicht relevant.
        /string_ops.[c|h]       -- Hilffunktionen für Textausgaben. Für Sie i.d.R. nicht relevant.
        /errors.[c|h]           -- Wichtig. Enthält die Definitionen verschiedener Fehlerfälle, die während des Compileprozesses auftreten können, mit ihren Exit-Codes. Immer relevant, besonders in PA4.
        /memory.[c|h]           -- Kleine Hilfsfunktionen zur sichereren Verwaltung von Speicher. Für Sie i.A. nicht relevant.
        /list.h                 -- Enthält einige Makros, die bei der Definition der vielen verschiedenen Listenstrukturen nützlich sind. Für die Aufgaben nicht wirklich relevant.
    /command_line_options.[c|h] -- Enthält das Parsing der Kommandozeilenoptionen des Compilers.
    /main.c                 -- Der Einstiegspunkt des Compilers. Übernimmt den Aufruf der Compilerphasen.
.gitignore      -- Eine Datei, die der Versionsverwaltung die Dateien auflistet, die ignoriert werden, also nicht dem Repository hinzugefügt werden sollen (z.B. IDE Dateien, Builddateien).
CMakeLists.txt  -- Die Projektdatei für das Buildsystem CMake. 


2. Softwarevoraussetzungen

Das Projekt nutzt das CMake-Buildsystem in der Version 3.5. CMake ist ein plattformunabhängiger "Buildfile-Generator". Von unserer Seite aus wird allerdings nur Linux unterstützt.
Für die Windowsnutzer: Wir empfehlen, das 'Windows Subsystem for Linux' zu installieren und einzurichten! Mehr dazu erfahren Sie im README, das oberhalb dieses Ordners liegt.

CMake nutzt eine CMakeLists.txt-Datei, die die Inhalte des Projekts beschreibt. Diese ist bereits in diesem Ordner enthalten.
Ein Aufruf des CMake Tools baut nicht direkt das Projekt, sondern generiert Builddateien für das native Buildsystem der aktuellen Plattform.
Unter Linux wird zum Beispiel eine Makefile generiert, unter Windows Projektdateien für das MSBuild-System.
Mehr über CMake erfahren Sie auf cmake.org.

Um das Projekt bauen zu können benötigen Sie neben CMake einen C11-fähigen Compiler, sowie den Scannergenerator flex und den Parsergenerator bison.
Die allermeisten Linux Distributionen werden mit gcc in einer C11-kompatiblen Version ausgeliefert. Die erwähnten Generatoren müssen sie manuell nachinstallieren.

Unter Ubuntu können Sie alle Voraussetzungen mit dem folgenden Befehl installieren:
    sudo apt install cmake flex bison

MacOS: Um das Projekt über die Kommandozeile bauen zu können benötigen sie die Xcode Kommandozeilentools.
Darüber hinaus benötigen auch Sie CMake, Flex und Bison.
Dies können Sie über
    sudo brew install cmake flex bison

XCode enthält bereits Versionen von Flex und Bison, die jedoch nicht aktuell genug sind.
Die CMake-Builddatei (CMakeLists.txt) enthält Code, der dafür sorgt, dass die Versionen verwendet werden, die manuell über Brew installiert wurde.
Achten Sie darauf, dass im Make-Log (siehe Punkt 4) eine Bisonversion neuer als 3.0 aufgeführt wird.
Falls sie einen Syntaxfehler im Zusammenhang mit dem Codefragment '%code' angezeigt bekommen, nutzen Sie eine veraltete Bisonversion.


3. IDE-Nutzung

Wir empfehlen Ihnen eine IDE zu nutzen. Dies ist insbesondere für das Debugging Ihrer Lösung hilfreich.
Dafür empfehlen wir CLion von Jetbrains. CLion basiert auf der IntelliJ Plattform.
Wie für IntelliJ können Sie auch für CLion als Student eine kostenlose Lizenz bekommen.

Wichtig: Öffnen Sie mit CLion den Ordner, in dem dieses README und die CMakeLists.txt liegen, es sei denn Sie wissen genau was Sie tun.
         Öffnen Sie NICHT den Überordner oder den src-Unterordner. Nur so versteht CLion Ihr Projekt und kann normal genutzt werden!

Nachdem CLion das CMake Projekt importiert hat müssen Sie unter Umständen noch zwei Ordner als Quellverzeichnisse markieren.
Machen Sie dafür einen Rechtsklick auf die Ordner "src" und "cmake-build-debug/generated-sources" und wählen Sie "Mark Directory as -> Project Sources and Headers".
Damit der letztere Ordner existiert, müssen Sie das Projekt zunächst einmal bauen.
Der "generated-sources" Ordner wird die Quelldateien enthalten, die von Scanner- und Parsergenerator generiert werden.

Windows 10: CLion funktioniert unter Windows 10 am einfachsten mit der WSL Toolchain. Dafür müssen Sie einen ssh-Zugang einrichten, mit dem CLion
            auf das Subsystem zugreifen kann. Eine Anleitung von Jetbrains dazu finden Sie hier: https://www.jetbrains.com/help/clion/how-to-use-wsl-development-environment-in-clion.html


4. Manuelles Bauen und Ausführen.

Wenn Sie das Projekt manuell bauen wollen, erstellen Sie einen zusätzlichen Unterordner 'build'.
Öffnen Sie in diesem Ordner ein Terminal und führen Sie die folgenden Befehle aus:
    cmake ..
    make

Dieser Prozess wird auch 'Out of source build' genannt, da die Builddateien nicht im gleichen Ordner wie das restliche Projekt liegen.
Der CMake Aufruf generiert die Dateien für das Buildsystem 'make'. Der anschließende Aufruf baut dann tatsächlich das Projekt.
Anschließend sollte, wenn der Kompilierprozess nicht fehlschlägt, im Buildordner eine Datei 'spl' entstanden sein.
Dies ist die fertige ausführbare Datei, die Sie wie jedes andere Programm über ein Terminal starten können.

CMake unterstützt keine eingebaute Funktion, die Builddateien aufzuräumen ('clean').
Wenn Sie alle Dateien des Buildprozesses loswerden wollen, löschen Sie einfach den 'build'-Ordner oder dessen Inhalte.