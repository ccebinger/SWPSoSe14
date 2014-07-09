
#ifndef Parser_h
#define Parser_h

#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>


#include <common/Encoding.h>
#include <frontend/adjacency_list.h>
#include <frontend/Graphs.h>
#include <common/Env.h>
#include <frontend/lexer/Lexer.h>


using namespace std;


/**
 * All 8 possible Directions (compass directions) in which the train can move.
 * Assignment of the enum values must not be changed without adapting the functions
 * void turnLeft45Deg() and
 * void turnRight45Deg()
 * in Parser.cpp since they rely on this order
*/
enum Direction {
	E = 0,
	SE = 1,
	S = 2,
	SW = 3,
	W = 4,
	NW = 5,
	N = 6,
	NE = 7
};

/**
 * 	Used to store the offsets on where to look(concerning the current position).
 * 	First in offsets is for looking to the left, second looks straight ahead, third for looking right
 */
struct offsetvalues {
	int offsets[3];
};

/**
 * Struct used to store the acceptable chars for looking left, straight or right.
 */
struct allowedChars {
	list<uint32_t> left;
	list<uint32_t> straight;
	list<uint32_t> right;
};

/**
* Struct used to unmistakenly identify rail-command-nodes in the graph in order to avoid creating them multiple times.
* Nodes are identified by their position in the file and the Direction they were entered through while parsing)
* this is relevant so that nodes in a loop will not be parsed an infinite number of times but instead are reused(because they already exist)
*/
struct NodeIdentifier{
	int32_t posRow;
	int32_t posCol;
	Direction dir;
};

/**
* Operator must be overloaded so that NodeIdentifier can serve as a map key.
*/
bool operator<( const NodeIdentifier&, const NodeIdentifier&);

/**
* Operator must be overloaded so that NodeIdentifier can be found in a map.
*/
bool operator==(const NodeIdentifier&,const NodeIdentifier&);

/**
* struct used as key for the juncDirChangeMap in the Parser.
*/
struct Djp{
	Direction dir;
	string junction;
	bool path;
};

/**
* Operator must be overloaded so that Djp can serve as a map key.
*/
bool operator<( const Djp &l, const Djp &r);

/**
* The class responsible for parsing rail functions.
*/
class Parser {

	private:

		/**
		 * Informs you when to turn left 45 deg after going straight. The leftDirChangeMap/rightDirChangeMap are needed for the following case:
		 * The train did go straight and now it needs to be decided, if the direction needs to be changed
		 * For Example if your Direction is East and you go straight by reading '/' you need to change the direction (left turn)
		*/
		const map<Direction, uint32_t> leftDirChangeMap = {
			{ E,  '/'  },
			{ SE, '-'  },
			{ S,  '\\' },
			{ SW, '|'  },
			{ W,  '/'  },
			{ NW, '-'  },
			{ N,  '\\' },
			{ NE, '|'  },
		};

		/**Informs you when to turn right 45 deg after going straight. Has the same semantics as leftDirChangeMap
		 *
		 */
		const map<Direction,uint32_t> rightDirChangeMap = {
			{ E,  '\\' },
			{ SE, '|'  },
			{ S,  '/'  },
			{ SW, '-'  },
			{ W,  '\\' },
			{ NW, '|'  },
			{ N,  '/'  },
			{ NE, '-'  },
		};

		/**
		 * The juncDirChangeMap is used to store the correct direction changes after passing a junction. Keys consist of:
		 * -the current direction(the one the train was driving in while passing the junction
		 * -the junction itsself as a string (either "v","^","<",">")
		 * -the path that shall be taken (true (same as right) or false (same as left) )
		 * Entrys are the new direction based on the three key elements -
		 * not all combinations of keys elements are possible - for example for each junction there are (naturally) only 3 possible directions through which the train can enter!
		 * before accessing the map it should be ensured that the input key is valid
		 */
		const map<Djp,Direction> juncDirChangeMap = {
				//junction "<"
				{{E,"<",true},SE},
				{{E,"<",false},NE},
				{{SW,"<",true},W},
				{{SW,"<",false},SE},
				{{NW,"<",true},NE},
				{{NW,"<",false},W},
				//junction ">"
				{{W,">",true},NW},
				{{W,">",false},SW},
				{{NE,">",true},E},
				{{NE,">",false},NW},
				{{SE,">",true},SW},
				{{SE,">",false},E},
				//junction "^"
				{{S,"^",true},SW},
				{{S,"^",false},SE},
				{{NW,"^",true},N},
				{{NW,"^",false},SW},
				{{NE,"^",true},SE},
				{{NE,"^",false},N},
				//junction "v"
				{{N,"v",true},NE},
				{{N,"v",false},NW},
				{{SE,"v",true},S},
				{{SE,"v",false},NE},
				{{SW,"v",true},NW},
				{{SW,"v",false},S},
		};

		/**
		 * The rowOffsetMap is responsible for providing the offsets(based on the current Direction) that tell you in which row to look for the next character(based on your current row).
		 * For Example if you are going east and you want to look left, the row needs to be lowered by 1, if you are going straight the row stays the same(0), if you are going right the row needs to be increased by 1 (+1)
		 * Thus the offset-values are saved as a triple of ints, first one being for left, second for straight, third right
		 */
		const map<Direction, offsetvalues> rowOffsetMap = {
			{ E,  offsetvalues{{ -1,  0, +1 }} },
			{ SE, offsetvalues{{  0, +1, +1 }} },
			{ S,  offsetvalues{{ +1, +1, +1 }} },
			{ SW, offsetvalues{{ +1, +1,  0 }} },
			{ W,  offsetvalues{{ +1,  0, -1 }} },
			{ NW, offsetvalues{{  0, -1, -1 }} },
			{ N,  offsetvalues{{ -1, -1, -1 }} },
			{ NE, offsetvalues{{ -1, -1,  0 }} },
		};
		/**
		 * The colOffsetMap is responsible for providing the offsets(based on the current Direction) that tell you in which column to look for the next character(based on your current column).
		 * Works the same way as rowOffsetMap
		 */
		const map<Direction, offsetvalues> colOffsetMap = {
			{ E,  offsetvalues{{ +1, +1, +1 }} },
			{ SE, offsetvalues{{ +1, +1,  0 }} },
			{ S,  offsetvalues{{ +1,  0, -1 }} },
			{ SW, offsetvalues{{  0, -1, -1 }} },
			{ W,  offsetvalues{{ -1, -1, -1 }} },
			{ NW, offsetvalues{{ -1, -1,  0 }} },
			{ N,  offsetvalues{{ -1,  0, +1 }} },
			{ NE, offsetvalues{{  0, +1, +1 }} },
		};

		/**
		 * The validRailMap is responsible for identifying valid rails(based on the current Direction that can be read by going left, straight or right.
		 * For Example if you are going east and you want to go left, you can only read '/' if you want to go straight any character of {'-','/','\\','+','*'} is allowed
		 * Thus the valid rails are saved as triples of character arrays, first array being for left, second for straight, third for right.
		 */
		const map<Direction, allowedChars> validRailMap = {
			{ E, allowedChars {
				{'/'},				// left
				{'-','/','\\','+','*'},		// straight
				{'\\'},				// right
			}},
			{ SE, allowedChars {
				{'-'},
				{'-','\\','|','*','x'},
				{'|'},
			}},
			{ S, allowedChars {
				{'\\'},
				{'|','\\','/','*','+'},
				{'/'}
			}},
			{ SW, allowedChars {
				{'|'},
				{'-','/','|','*','x'},
				{'-'}
			}},
			{ W, allowedChars {
				{'/'},
				{'-','/','\\','+','*'},
				{'\\'},
			}},
			{ NW, allowedChars {
				{'-'},
				{'-','\\','|','*','x'},
				{'|'},
			}},
			{ N, allowedChars {
				{'\\'},
				{'|','\\','/','*','+'},
				{'/'},
			}},
			{ NE, allowedChars {
				{'|'},
				{'-','/','|','*','x'},
				{'-'},
			}},
		};


	private:
		const int LEFT = 0;
		const int STRAIGHT = 1;
		const int RIGHT = 2;

		int32_t moveCount = 0;
		int32_t posRow = 0;
		int32_t posCol = 0;
		Direction dir = SE;
		shared_ptr<RailFunction> board;
		map<string,shared_ptr<RailFunction>> lexedFunctions;
		set<string> allCurrentlyUsedFunctionNames;
		Graphs graphs;

		map<NodeIdentifier,std::shared_ptr<Node>> allNodes;
		std::shared_ptr<Adjacency_list> currentAbstractSyntaxGraph;
		std::shared_ptr<Node> currentNode;
		bool addNextNodeAsTruePathOfPreviousNode = true;


		bool parsingNotFinished = true;
		void move();
		bool addToAbstractSyntaxGraph(string,Command::Type,NodeIdentifier);
		void turnLeft45Deg();
		void turnRight45Deg();
		void reverseDirection();
		bool currentCharIsNoCrossing();
		Direction getOutPathOfJunction(string,Direction,bool);
		void parseVariable(string data,NodeIdentifier);
		bool parseJunctions(list<Direction>,int,int,string,Command::Type,NodeIdentifier);
		bool checkForValidCommandsInStraightDir(int,int);
		void setRowCol(int,int);
		void stepStraight();
		string readCharsUntil(uint32_t);
		string readConstantStringUntil(uint32_t);
		std::shared_ptr<Adjacency_list> parseGraph(int,int,Direction);
		/**
		* Parses a rail function and puts its contents (adjacency-list) into the graphs-data structure
		* @ param fname the name of the rail function to be parsed.
		* @returns An Adjacency_list object, which is basically a graph consisting of the rail-commands used in the function.
		*/
		void parseCompleteBoard(string fname);
		string findUnusedFunctionName();
		void parseLambdaFunction(string,int,int,Direction);
		void parseLambda(NodeIdentifier id);
	public:
		/**
		* Creates a Parser object.
		* The object should only be used for parsing one Rail-Function at the Moment. For a subsequent function, a new Parser-object should be created
		* @param railFunction a RailFunction object containing the rail function to be parsed.
		* @param allCurrentlyUsedNames contains all function names that are currently being used
		*/
		Parser(map<string,std::shared_ptr<RailFunction>> lexedFunctions);

		/**
		 * Parses all Rail-Functions that were passed in the constructor and inserts their adjacency-lists into the graphs object.
		 * @param graphs an empty Graphs object that will be filled after this function-call
		 */
		Graphs parseGraphs(Graphs graphs);
};

#endif
