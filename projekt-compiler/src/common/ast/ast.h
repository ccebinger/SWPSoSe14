#ifndef AST_H_
#define AST_H_

#include <string>
#include <vector>

struct Command {
	enum Type {
		PUSH_CONST, OUTPUT,
		ADD, SUB, MULT, DIV
		// TODO others...
	};

	Command::Type type;
	std::string* arg;
};

struct Node {
	int id;
	Command command;
	Node *successor1; //true Pfad
	Node *successor2; //false Pfad
	bool operator ==(const Node& n) const{
		return id == n.id;
	}
};

class Graph {
public:
	static Graph* deserialize(std::istream& in);

	virtual ~Graph();
	virtual void addNode(Node *node) = 0;
	virtual void addEdge(Node *source, Node *dist, bool path) = 0;
	virtual void serialize(std::ostream& out) = 0;
	virtual void deserialize(const std::string& file, char delimiter) = 0;
	virtual Node *start() = 0;
	virtual std::size_t nodeCount() const = 0 ;
	virtual Node* find(int id) = 0;
	/**
	 * Returns the name of the graph.
	 * The name of the graph is equal to the name of the function.
	 * (Each function has his own graph)
   */
	virtual std::string name() const = 0;
};

#endif /* AST_H_ */
