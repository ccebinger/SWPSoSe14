#ifndef AST_H_
#define AST_H_

#include <string>
#include <vector>
#include <memory>
#include <iostream>

/**
 * Represents an Rail command, consits of an enum Command::Type and an argument.
 * For each command type in Rail there exists a corresponding enum Command::Type.
 *
 * @author Christopher Zell <Zelldon91@googlemail.com>, Hanin Halawani
 */
struct Command {
  /**
   * Represents an command type of an Rail progamm.
   * Each value corresponds to the ascii value of the corresponding Rail command.
   * E.g. o -> 111 or a -> 97, thats makes the mapping much easier.
   */
	enum Type {
		PUSH_CONST = 0, /**< 0-9 or [...] */
		CALL = 1, /**< {FUNC} */
		OUTPUT = 111, /**< o */
		ADD = 97, /**< a */
		SUB = 115,  /**< s */
		MULT = 109, /**< m */
		DIV = 100, /**<  d */
		MOD = 114, /**<  r */
		CUT = 99, /**<  c */
		APPEND = 112,/**< p */
		SIZE = 122,/**< z */
		NIL = 110,/**< n */
		LIST_CONS = 58,/**< : */
		LIST_BREAKUP = 126, /**< ~ */
		FALSE = 102,/**< f */
		GREATER = 103,/**< g */
		EQUAL = 113, /**< q */
		TRUE = 116,/**< t */
		REFLECTOR = 64,/**< @ */
		START =  36,/**< $ */
		FINISH = 35,/**< \# */
		LAMBDA = 38,/**< &amp; */
		BOOM = 98,/**< b */
		EOF_CHECK = 101,/**< e */
		INPUT = 105,/**< i */
		UNDERFLOW_CHECK = 117,/**< u */
		TYPE_CHECK = 63,/**< ? */
	};
 /**
  * The type of the Rail command.
  */
	Command::Type type;
 /**
  * The argument of the Rail command.
  */
	std::string arg;
};

/**
 * Represents a node in the [a]bstract [s]yntax [g]raph (ASG), each node
 * also represents a command in the corresponding Rail programm.
 * That means each node has a command to execute.
 *
 * @author Christopher Zell <Zelldon91@googlemail.com>, Hanin Halawani
 */
struct Node {

 /**
  * The default ctor of the node, initialize the id of the node with 0.
  */
  Node() {id = 0;}
 /**
  * The copy ctor of the node, copies the values of another node in the
  *  corresponding node.
  * @param n    the node which will be copied
  */
  Node(const Node& n)
  {
    id = n.id;
    command = n.command;
    successor1 = n.successor1;
    successor2 = n.successor2;
  }
 /**
  * The copy ctor of the node, copies the values of another node in the
  *  corresponding node.
  * @param n    the node which will be copied
  */
  Node(Node* n)
  {
    id = n->id;
    command = n->command;
    successor1 = n->successor1;
    successor2 = n->successor2;
  }

 /**
  * The destructor of the node, resets all existing smart pointers inside the node.
  */
  ~Node()
  {
    successor1.reset();
    successor1.reset();
  }
 /**
  * The ID of the node, which identifies the node.
  */
	int id;
 /**
	* The corresponding command of the node.
	*/
	Command command;
 /**
  * The first successor of the current node.
  * <B>This successor is always the true path and is set if a successor exists.</B>
  * That means if the node (command) is not #, what means end, a successor must exists
  * and these is placed in the successor1.
  */
	std::shared_ptr<Node> successor1;
 /**
	* The second successor of the current node.
	* This successor is only not null if the current node is a junction, like <, >, V or ^.
	* <B>The successor2 contains the false path of the junction.</B>
	*/
	std::shared_ptr<Node> successor2;
 /**
	* The equals operator which copares the id of the current with another node.
	* If the ids are equal it returns true, otherwise false.
	* @param n      the node which will be used for the comparison
	* @return       true if the ids are equal, false otherwise
	*/
	bool operator ==(const Node& n) const{
		return id == n.id;
	}
};

/**
 * The interface of the [a]bstract [s]yntax [g]raph (ASG), which consists
 * of different nodes these are representing the commands of the Rail programm.
 * An ASG represents a function in a Rail programm, that means for each
 * function in a Rail programm there exists a Graph.
 *
 * @author Christopher Zell <Zelldon91@googlemail.com>, Hanin Halawani
 */
class Graph {
public:
 /**
  * The destructor of the ASG.
  */
	virtual ~Graph(){};
 /**
  * Adds a node (command) to the corresponding ASG.
  *
  * @param node     the node which will be added
  */
	virtual void addNode(std::shared_ptr<Node> node) = 0;

 /**
  * Replaces the successor of a certain node.
  * If the path is true the successor1 of the source will be replaced, successor2 if the path is false.
  *
  * @param source     the node which gets a new succesor
  * @param dist       the new successor of the source
  * @param path       specifies whether the successor1 or 2 are been replaced
  */
	virtual void addEdge(std::shared_ptr<Node> source, std::shared_ptr<Node> dist, bool path) = 0;

 /**
	* Returns the start node of the ASG, respectively of the Rail function.
	* @return       the start node
	*/
	virtual std::shared_ptr<Node> start() = 0;
 /**
	* Returns the current size of the graph (node count).
	* @return       the size of the ASG
	*/
	virtual std::size_t nodeCount() const = 0 ;
 /**
  * Finds for the given id the node in the ASG.
  * @param id       the id of the node, which should be founded
  * @return         the node with the searched id
	*/
	virtual std::shared_ptr<Node> find(int id) const = 0 ;
	/**
	 * Returns the name of the ASG.
	 * The name of the graph is equal to the name of the Rail function.
	 * (Each function has his own graph)
	 * @return      the name of the ASG
   */
	virtual std::string name() const = 0;
};

#endif /* AST_H_ */
