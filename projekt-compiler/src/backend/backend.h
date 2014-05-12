#ifndef BACKEND_H_
#define BACKEND_H_

#include <iostream>

/**
 * Die Klasse stellt statische Methoden für das Übersetzen des Graphen in
 * Target-Code zur Verfügung.
 */
class Backend {
public:
	/**
	 * Die möglichen 'Exit Codes' des Backends.
	 */
	enum Status {
		/**
		 * Erfolgreiche Übersetzung.
		 */
		SUCCESS
	// TODO other statuses...
	};

	/**
	 * Übersetzt den serialisierten Graphen aus graphIn in Target-Code, der
	 * auf codeOut geschrieben wird. Gibt je nach Ergebnis einen
	 * Backend::Status zurück.
	 */
	static Backend::Status Generate(std::istream& graphIn,
			std::ostream& codeOut);

	/**
	 * Gibt eine beschreibende Fehlernachricht zu einem Backend::Status
	 * zurück. Ist der status Backend::Status::SUCCESS, wird ein leerer String
	 * zurückgegeben.
	 * Typische Nutzung dieser Methode:
	 * if (status != Backend::Status::SUCCESS) {
	 * 		std::cerr << Backend::ErrorMessage(status) << std::endl;
	 * }
	 */
	static std::string ErrorMessage(Backend::Status status);

private:
	/**
	 * Nur statische Methoden.
	 */
	Backend();
};

#endif /* BACKEND_H_ */
