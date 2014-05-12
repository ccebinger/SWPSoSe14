#include "backend.h"

Backend::Status Backend::Generate(std::istream& graphIn,
		std::ostream& codeOut) {
	(void) graphIn;
	(void) codeOut;
	// TODO implementieren
	return Backend::Status::SUCCESS;
}

std::string Backend::ErrorMessage(Backend::Status status) {
	switch (status) {
	case Backend::Status::SUCCESS:
		return "";
		break;
		// TODO handle other cases
	default:
		return "";
	}
}
