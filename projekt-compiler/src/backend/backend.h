#ifndef BACKEND_H_
#define BACKEND_H_

#include <iostream>

class Backend {
public:
	enum Status {
		SUCCESS
		// TODO other statuses...
	};

	static Backend::Status generate(std::istream& graphIn, std::ostream& codeOut);
};

#endif /* BACKEND_H_ */
