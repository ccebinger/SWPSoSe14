#include <backend/backend.h>
#include <frontend/Graphs.h>

Backend::Status Backend::Generate(std::istream& graphIn,
                                  std::ostream& codeOut) {
	//FIXME: Die deserialize-methode wurde in "src/frontend/Csv_io.cpp" verschoben. Bitte anpassen
//	Graph* graph = Graph::deserialize(graphIn);
//	Backend::Status ret = Backend::Generate(*graph, codeOut);
//	delete graph;
//	return ret;
}

Backend::Status Backend::Generate(const Graphs& graph, std::ostream& codeOut) {
  (void) graph;
  (void) codeOut;
  // TODO
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
