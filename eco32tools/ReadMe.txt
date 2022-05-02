Dieser Ordner enthält einige Tools, die für die Benutzung von SPL mit dem Eco32 Simulator notwendig sind.
Im Speziellen sind das:
   - bin/
      - as: 
         Der Eco32-Assembler. Übersetzt Eco32-Assemblercode zu einer Eco32 Objektcodedatei.    
      - ld: 
         Der Eco32-Linker. Verknüpft mehrere Objektcodedateien zu einer einzigen.
         Linker werden in der Regel benutzt um ein Programm, dass aus mehreren Modulen besteht, zusammenzuführen.
         In SPL wird der Linker nur verwendet, um den sogenannten Startupcode dazuzubinden.
         Der Startupcode ist dafür verantwortlich den Stack zu initialisieren und die Main-Prozedur aufzurufen.
      - load:
         Der Eco32-Loader. Übersetzt eine Objektcodedatei zu einer ausführbaren Datei.
      - refspl: 
         Eine Referenzimplementierung des SPL-Compilers. Übersetzt SPL-Code in Eco32-Assemblercode.    
      - sim:
         Der Eco32-Simulator. Kann genutzt werden um SPL-Programme, die mit der Toolchain bestehend aus refspl, as, ld, load übersetzt worden sind, auszuführen
         Der Simulator wird aufgerufen mit 'sim -x -l <executable_file> -s 1 -t 0 -g'.
         Er öffnet ein xterm Fenster und benötigt daher eine Grafikausgabe. Falls Sie keine zur Verfügung haben (z.B. in der WSL) können Sie das Tool 'xvfb-run' aus dem 'xvfb' package nutzen.
         In diesem Fall sind leider auch keine Texteingaben über die Kommandozeile möglich, beachten Sie dies daher bei der Auswahl der SPL-Programme.
         WICHTIG: Auch wenn Sie keine Grafikausgabe nutzen müssen Sie das Paket 'xterm' installieren! Auf Ubuntu geht das z.B. mit 'sudo apt install xterm'.
   - lib/
      Dieser Ordner enthält ein Script für den Linker, die Objektcodedatei des Startupcodes, sowie die Standardbibliothek von SPL.

Informationen über die Benutzung der einzelner Tools kann jeweils mit der '--help' Option abgerufen werden.

Außerdem sind zwei Scripts enthalten, die bei der Benutzung der Toolchain helfen können:
   - compile.sh:
      Führt die Toolchain (refspl, as, ld, load) für eine .spl-Datei aus.
      Als Argument muss der Pfad zur .spl-Datei angegeben werden.
      Zwischenergebnisse werden mit den Endungen .s, .o und .x abgelegt. 
      Die ausführbare Datei ist dann mit der Endung .bin verfügbar.

      Beispiel: "./compile.sh bigtest.spl".
         - Assemblercode: bigtest.s
         - Objektcode: bigtest.o
         - Gelinkter Objektcode: bigtest.x
         - Ausführbarer Code: bigtest.bin
      
      Falls Sie die Tools manuell ausführen wollen finden Sie in diesem Script Verwendungsbeispiele für die Tools.

   - run.sh:
      Führt den Simulator mit einer bereits gebauten (z.B. mit compile.sh) ausführbaren Datei aus.
      Der Pfad zu dieser Datei (i.d.R. .bin) muss als Argument angegeben werden.
      Falls kein Display zur Verfügung steht versucht dieses Script automatisch xvfb-run (falls installiert) zu verwenden.
      Die Ausgabe des Simulators können Sie in jedem Fall im Terminal sehen.

   Tipp: Sobald sie gegen Ende des Semesters Ihren SPL-Compiler testen wollen, legen Sie sich eine Kopie von compile.sh an,
      die mit Ihrem Compiler arbeitet, statt die Referenzimplementierung zu verwenden. Das erleichtert Ihnen das Testen immens.