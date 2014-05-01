#include <fstream>
#include <iostream>

#include "backend.h"

#ifdef STANDALONE_BACKEND

/**
 * Standalone-App für das Backend. Liest einen serialisierten Graphen und gibt
 * den generierten Bytecode aus.
 * Parameter:
 * -i <graphfile>: Liest den Graphen aus der Datei graphfile statt von stdin.
 * -o <outfile>: Gibt den Bytecode auf outfile statt auf stdout aus.
 * Returns:
 * 0 bei Erfolg.
 * 1 bei Fehler in Kommandozeilenparametern.
 * 2 falls die Ein- oder Ausgabedateien nicht geöffnet werden können (s. -i/-o).
 * 3 bei Übersetzungsfehler.
 * 99 bei einem unbekannten Fehler.
 */
int main(int argc, char** argv) {
	/* Ein-/Ausgabestreams nach Argumenten setzen */
	std::ifstream inFile;
	std::ofstream outFile;

	std::istream* in = &std::cin;
	std::ostream* out = &std::cout;

	for (int i = 1; i != argc; ++i) {
		std::string current(argv[i]);
		if (current == "-i") {
			if (i == argc-1) {
				std::cerr << "Fehler im -i Parameter, bitte Manual lesen." << std::endl;
				return 1;
			} else {
				inFile.open(argv[++i]);
				if(inFile.fail()) {
					std::cerr << "Fehler: Eingabedatei kann nicht geöffnet werden" << std::endl;
					return 2;
				}
				in = &inFile;
			}
		} else if (current == "-o") {
			if (i == argc-1) {
				std::cerr << "Fehler im -o Parameter, bitte Manual lesen." << std::endl;
				return 1;
			} else {
				outFile.open(argv[++i]);
				if(outFile.fail()) {
					std::cerr << "Fehler: Ausgabedatei kann nicht geöffnet werden" << std::endl;
					return 2;
				}
				out = &outFile;
			}
		} else {
			std::cerr << "Fehler, unbekannter Parameter, bitte Manual lesen." << std::endl;
			return 1;
		}
	}

	/* Bytecode generieren */
	Backend::Status status = Backend::Generate(*in, *out);
	/* Erfolg prüfen */
	if (status != Backend::Status::SUCCESS) {
		std::cerr << Backend::ErrorMessage(status) << std::endl;
		return 3;
	}
}

#endif // STANDALONE_BACKEND
