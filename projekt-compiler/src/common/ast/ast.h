#ifndef AST_H_
#define AST_H_

#include <string>

struct Command {
	enum Type {
		PUSH_CONST,
		OUTPUT
		// TODO others...
	};

	Command::Type type;
	std::string arg;
};

struct Node {
	int id;
	Command command;
	Node *successor1; //true Pfad
	Node *successor2; //false Pfad
};

class Graph {
public:
	static Graph* deserialize(std::istream& in);

	virtual ~Graph();
	void addNode(Node *node);
	void addEdge(Node *source, Node *dist, bool path);
	void serialize(std::ostream& out);
	Node *start();
	std::size_t nodeCount();
};

#endif /* AST_H_ */
