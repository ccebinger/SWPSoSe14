#include <fstream>
#include <iostream>

#include <backend/backend.h>


#ifdef STANDALONE_BACKEND

/**
 * Standalone-App für das Backend. Liest einen serialisierten Graphen und gibt
 * den generierten Bytecode aus.
 * Parameter:
 * -i <graphfile>: Liest den Graphen aus der Datei graphfile. Pflicht.
 * -o <outfile>: Gibt den Bytecode auf outfile aus. Pflicht.
 * Returns:
 * 0 bei Erfolg.
 * 1 bei Fehler in Kommandozeilenparametern.
 * 2 falls die Ein- oder Ausgabedateien nicht beide angegeben wurden (s. -i/-o).
 * 3 bei Übersetzungsfehler.
 * 4 falls die Ausgabedatei nicht geöffnet werden konnte.
 * 99 bei einem unbekannten Fehler.
 */
int main(int argc, char** argv) {
  /* Ein-/Ausgabestreams nach Argumenten setzen */
  std::string inFile;
  std::string outFile;

  for (int i = 1; i != argc; ++i) {
    std::string current(argv[i]);
    if (current == "-i") {
      if (i == argc-1) {
        std::cerr << "Fehler im -i Parameter, bitte Manual lesen." << std::endl;
        return 1;
      } else {
        inFile = std::string(argv[++i]);
      }
    } else if (current == "-o") {
      if (i == argc-1) {
        std::cerr << "Fehler im -o Parameter, bitte Manual lesen." << std::endl;
        return 1;
      } else {
        outFile = std::string(argv[++i]);
      }
    } else {
      std::cerr << "Fehler, unbekannter Parameter, bitte Manual lesen." << std::endl;
      return 1;
    }
  }

  if(outFile == "" || inFile == "") {
    std::cerr << "Fehler, bitte Ein- und Ausgabedatei angeben, s. Manual." << std::endl;
    return 1;
  }

  std::ofstream out;
  out.open(outFile, std::ios::out | std::ios::binary);
  if(!out.is_open()) {
    std::cerr << "Fehler, konnte Ausgabedatei nicht öffnen" << std::endl;
    return 4;
  }

  /* Bytecode generieren */
  Backend::Status status = Backend::Generate(inFile, out);
  /* Erfolg prüfen */
  if (status != Backend::Status::SUCCESS) {
    std::cerr << Backend::ErrorMessage(status) << std::endl;
    return 3;
  }
}

#endif  // STANDALONE_BACKEND
