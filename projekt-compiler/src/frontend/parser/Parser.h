
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


using namespace std;


/*assignment of the enum values must not be changed without adapting the functions
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


struct offsetvalues {
	//used to store the offsets on where to look(concerning the current position), first in offsets is for looking to the left, second looks straight ahead, third for looking right
	int offsets[3];
};


struct allowedChars {
	//used to store the acceptable chars for looking left, straight or right
	list<uint32_t> left;
	list<uint32_t> straight;
	list<uint32_t> right;
};

struct NodeIdentifier{
	//used to unmistakenly identify nodes(by their position in the file and the Direction they were entered through while parsing)
	//this is relevant so that nodes in a loop will not be parsed multiple times
	//but instead are reused(because they already exist)
	uint32_t posRow;
	uint32_t posCol;
	Direction dir;
	//operator necessary so NodeIdentifier can be used in a map.
//	bool operator<( const NodeIdentifier &r){
//		if(posRow == r.posRow){
//			if(posCol==r.posCol){
//				//cols and rows are equal, dir is decisive for comparison
//				return dir < r.dir;
//			} else{
//				//rows are equal, col is decisive for comparison
//				return posCol<r.posCol;
//			}
//		} else{
//			//row is decisive for comparison
//			return posRow<r.posRow;
//		}
//	}
//	bool operator==(const NodeIdentifier &r){
//		return posRow==r.posRow && posCol==r.posCol && dir==r.dir;
//	}
};


bool operator<( const NodeIdentifier&, const NodeIdentifier&);

bool operator==(const NodeIdentifier&,const NodeIdentifier&);

//TODO: x hinzufügen als valid rail und @ implementieren
class Parser {

	private:
		/*
		 * The leftDirChangeMap/rightDirChangeMap are needed for the following case:
		 * The train did go straight and now it needs to be decided, if the direction needs to be changed
		 * For Example if your Direction is East and you go straight by reading '/' you need to change the direction (left turn)
		 */
		//if the train moves straight but reads a specific character the direction needs to be changesd
		//when to turn left 45 deg
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

		//when to turn right 45 deg
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



		/*
		 * The xOffsetMap/yOffsetMap are responsible for providing the offsets(based on the current position and Direction) that tell you where to look for the character
		 * For Example if you are going east and you want to look left, x(rowNumber) needs to be lowered by 1, if you are going straight x stays the same(0), if you are going right x needs to be increased by 1 (+1)
		 * Thus the offset-values are saved as a triple of ints, first one being for left, second for straight, third right
		 */
		//Order in offsetvalues: [0] - Left, [1] - straight, [2] - right
		//x offsets
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
		//y offsets:
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





		const map<Direction, allowedChars> validRailMap = {
			{ E, allowedChars {
				{'/','*','x'},				// left
				{'-','/','\\','+','*'},		// straight
				{'\\','*','x'},				// right
			}},
			{ SE, allowedChars {
				{'-','*','+'},
				{'-','\\','|','*','x'},
				{'|','*','+'},
			}},
			{ S, allowedChars {
				{'\\','*','x'},
				{'|','\\','/','*','+'},
				{'/','*','x'}
			}},
			{ SW, allowedChars {
				{'|','*','+'},
				{'-','/','|','*','x'},
				{'-','*','+'}
			}},
			{ W, allowedChars {
				{'/','*','x'},
				{'-','/','\\','+','*'},
				{'\\','*','x'},
			}},
			{ NW, allowedChars {
				{'-','*','+'},
				{'-','\\','|','*','x'},
				{'|','*','+'},
			}},
			{ N, allowedChars {
				{'\\','*','x'},
				{'|','\\','/','*','+'},
				{'/','*','x'},
			}},
			{ NE, allowedChars {
				{'|','*','+'},
				{'-','/','|','*','x'},
				{'-','*','+'},
			}},
		};


	private:
		const int LEFT = 0;
		const int STRAIGHT = 1;
		const int RIGHT = 2;

		int32_t posRow, posCol;
		Direction dir;
		shared_ptr<RailFunction> board;

		map<NodeIdentifier,std::shared_ptr<Node>> allNodes;
		std::shared_ptr<Adjacency_list> abstractSyntaxGraph;
		std::shared_ptr<Node> currentNode;
		int lastUsedId;
		bool addNextNodeAsTruePathOfPreviousNode;



		//string graphName;
		bool parsingNotFinished = true;
		void move();
		bool addToAbstractSyntaxGraph(string,Command::Type,NodeIdentifier);
		void turnLeft45Deg();
		void turnRight45Deg();
		void reverseDirection();
		bool parseVariable(string data,NodeIdentifier);
		bool parseJunctions(Direction,int,int,Direction,Direction,string,Command::Type,NodeIdentifier);
		bool checkForValidCommandsInStraightDir(int,int);
		int getNextUnusedId();
		void setRowCol(int,int);
		string readCharsUntil(uint32_t);
	public:
		string errorMessage;
		Parser(shared_ptr<RailFunction> railFunction);
		shared_ptr<Adjacency_list> parseGraph();
		shared_ptr<Adjacency_list> parseGraph(int,int,Direction);
};

#endif
