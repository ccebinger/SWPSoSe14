#ifndef AST_H_
#define AST_H_

#include <string>

struct Command {
	std::string arg;enum type;
};

struct Node {
	int id;
	Command command;
	Node *successor1; //true Pfad
	Node *successor2; //false Pfad
};

class Graph {
public:
	virtual ~Graph() = 0;
	virtual void addNode(Node *node) = 0;
	virtual void addEdge(Node *source, Node *dist, bool path) = 0;
	//Serilize(Ostream &out)
	//Deserilize()
	virtual Node *start() = 0;
	virtual std::size_t nodeCount() = 0;
};

#endif /* AST_H_ */
