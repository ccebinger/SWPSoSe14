#include "ast.h"

Graph* Graph::deserialize(std::istream& in) {
	(void) in;
	return nullptr;
}

Graph::~Graph() {}

void Graph::addNode(Node* node) {
	(void) node;
}

void addEdge(Node *source, Node *dist, bool path) {
	(void) source;
	(void) dist;
	(void) path;
}

Node* start() {
	return nullptr;
}

std::size_t nodeCount() {
	return 0;
}
