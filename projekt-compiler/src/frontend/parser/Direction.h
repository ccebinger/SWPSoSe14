/*
 * Direction.h
 *
 *  Created on: May 28, 2014
 *      Author: asd
 */

#ifndef DIRECTION_H_
#define DIRECTION_H_


//#include <iostream>
//#include <string.h>
#include <map>


/**
 * used to store the offsets on where to look(concerning the current position),
 * first in offsets is for looking to the left, second looks straight ahead, third for looking right
 */
struct offsetValues {
	int offsets[3];
};


/**
 * used to store the acceptable chars for looking left, straight or right
 */
struct allowedChars {
	std::list<char> left;
	std::list<char> straight;
	std::list<char> right;
};


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



struct DirConfig {
	static const std::map<Direction,char> leftDirChangeMap;
	static const std::map<Direction,char> rightDirChangeMap;
	static const std::map<Direction, offsetValues> rowOffsetMap;
	static const std::map<Direction, offsetValues> colOffsetMap;
	static const std::map<Direction, allowedChars> validRailMap;


	static void turnLeft(Direction& dir) {
		switch(dir) {
			case E:  dir = NE; break;
			case SE: dir = E;  break;
			case S:  dir = SE; break;
			case SW: dir = S;  break;
			case W:  dir = SW; break;
			case NW: dir = W;  break;
			case N:  dir = NW; break;
			case NE: dir = N;  break;
			default: break;
		}
	}

	static void turnRight(Direction& dir) {
		switch(dir) {
			case E:  dir = SE; break;
			case SE: dir = S;  break;
			case S:  dir = SW; break;
			case SW: dir = W;  break;
			case W:  dir = NW; break;
			case NW: dir = N;  break;
			case N:  dir = NE; break;
			case NE: dir = E;  break;
			default: break;
		}
	}

	static void reverse(Direction& dir) {
		switch(dir) {
			case E:  dir = W;  break;
			case SE: dir = NW; break;
			case S:  dir = N;  break;
			case SW: dir = NE; break;
			case W:  dir = E;  break;
			case NW: dir = SE; break;
			case N:  dir = S;  break;
			case NE: dir = SW; break;
			default: break;
		}
	}

};


/*
 * The leftDirChangeMap/rightDirChangeMap are needed for the following case:
 * The train did go straight and now it needs to be decided, if the direction needs to be changed
 * For Example if your Direction is East and you go straight by reading '/' you need to change the direction (left turn)
 */
//if the train moves straight but reads a specific character the direction needs to be changesd
//when to turn left 45 deg
const std::map<Direction,char> DirConfig::leftDirChangeMap = {
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
const std::map<Direction,char> DirConfig::rightDirChangeMap = {
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
const std::map<Direction, offsetValues> DirConfig::rowOffsetMap = {
	{ E,  offsetValues{{ -1,  0, +1 }} },
	{ SE, offsetValues{{  0, +1, +1 }} },
	{ S,  offsetValues{{ +1, +1, +1 }} },
	{ SW, offsetValues{{ +1, +1,  0 }} },
	{ W,  offsetValues{{ +1,  0, -1 }} },
	{ NW, offsetValues{{  0, -1, -1 }} },
	{ N,  offsetValues{{ -1, -1, -1 }} },
	{ NE, offsetValues{{ -1, -1,  0 }} },
};;
//y offsets:
const std::map<Direction, offsetValues> DirConfig::colOffsetMap = {
	{ E,  offsetValues{{ +1, +1, +1 }} },
	{ SE, offsetValues{{ +1, +1,  0 }} },
	{ S,  offsetValues{{ +1,  0, -1 }} },
	{ SW, offsetValues{{  0, -1, -1 }} },
	{ W,  offsetValues{{ -1, -1, -1 }} },
	{ NW, offsetValues{{ -1, -1,  0 }} },
	{ N,  offsetValues{{ -1,  0, +1 }} },
	{ NE, offsetValues{{  0, +1, +1 }} },
};





const std::map<Direction, allowedChars> DirConfig::validRailMap = {
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















#endif /* DIRECTION_H_ */
