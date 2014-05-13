#ifndef AST_H_
#define AST_H_

#include <string>
#include <vector>
#include <memory>
#include <iostream>

struct Command {
	enum Type {
		PUSH_CONST = 0, //0-9 or [...]
		CALL = 1,
		OUTPUT = 111, //o
		ADD = 97, //a
		SUB = 115,  //s
		MULT = 109, //m
		DIV = 100, // d
		MOD = 114, // r
		CUT = 99, // c
		APPEND = 112,//p
		SIZE = 122,//z
		NIL = 110,//n
		LIST_CONS = 58,//:
		LIST_BREAKUP = 126, //~
		FALSE = 102,//f
		GREATER = 103,//g
		EQUAL = 113, //q
		TRUE = 116,//t
		REFLECTOR = 64,//@
		START =  36,//$
		FINISH = 35,//#
		LAMBDA = 38,//&
		BOOM = 98,//b
		EOF_CHECK = 101,//e
		INPUT = 105,//i
		UNDERFLOW_CHECK = 117,//u
		TYPE_CHECK = 63,//?
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
