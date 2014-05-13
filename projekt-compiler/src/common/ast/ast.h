#ifndef AST_H_
#define AST_H_

#include <string>
#include <vector>
#include <memory>
#include <iostream>

struct Command {
	enum Type {
		PUSH_CONST, OUTPUT,
		ADD, SUB, MULT, DIV,
		CALL
		// TODO others...
	};

	Command::Type type;
	std::string arg;
};

struct Node {

  Node() {id = 0;}
  Node(const Node& n)
  {
    id = n.id;
    command = n.command;
    successor1 = n.successor1;
    successor2 = n.successor2;
  }
  Node(Node* n)
  {
    id = n->id;
    command = n->command;
    successor1 = n->successor1;
    successor2 = n->successor2;
  }
  ~Node()
  {
    successor1.reset();
    successor1.reset();
  }
	int id;
	Command command;
	std::shared_ptr<Node> successor1; //true Pfad
	std::shared_ptr<Node> successor2; //false Pfad
	bool operator ==(const Node& n) const{
		return id == n.id;
	}
};

class Graph {
public:
	virtual ~Graph(){};
	virtual void addNode(std::shared_ptr<Node> node) = 0;
	virtual void addEdge(std::shared_ptr<Node> source, std::shared_ptr<Node> dist, bool path) = 0;
	virtual std::shared_ptr<Node> start() = 0;
	virtual std::size_t nodeCount() const = 0 ;
	virtual std::shared_ptr<Node> find(int id) const = 0 ;
	/**
	 * Returns the name of the graph.
	 * The name of the graph is equal to the name of the function.
	 * (Each function has his own graph)
   */
	virtual std::string name() const = 0;
};

#endif /* AST_H_ */
