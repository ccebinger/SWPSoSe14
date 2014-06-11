/*
 * Parser.cpp
 *
 *  Created on: 30.04.2014
 *      Author: LeonBornemann
 */


#include <frontend/lexer/Lexer.h>
#include <frontend/parser/Parser.h>


bool operator<( const NodeIdentifier &l, const NodeIdentifier &r){
	if(l.posRow == r.posRow){
		if(l.posCol==r.posCol){
			//cols and rows are equal, dir is decisive for comparison
			return l.dir < r.dir;
		} else{
			//rows are equal, col is decisive for comparison
			return l.posCol<r.posCol;
		}
	} else{
		//row is decisive for comparison
		return l.posRow<r.posRow;
	}
}

bool operator==(const NodeIdentifier &l,const NodeIdentifier &r){
		return l.posRow==r.posRow && l.posCol==r.posCol && l.dir==r.dir;
}

Parser::Parser(std::shared_ptr<RailFunction> railFunction) {
	this->board = railFunction;
	//errorMessage - empty String: Everything is ok
	errorMessage = "";
	abstractSyntaxGraph = NULL;
}

void Parser::setRowCol(int newRow, int newCol){
	posRow = newRow;
	posCol = newCol;
}

shared_ptr<Adjacency_list> Parser::parseGraph() {
	addNextNodeAsTruePathOfPreviousNode = true;
	int startPosCol=-1;
	//find $, initiate pos
	for(int32_t i=0; i<board->getWidth(); ++i) {
		if(board->get(0, i) == '$') {
			startPosCol = i;
			break;
		}
	}
	if(startPosCol==-1){
		errorMessage ="'" + board->getName() + "': $ not found";
		return NULL;
	}
	return parseGraph(0,startPosCol,SE);
}

shared_ptr<Adjacency_list> Parser::parseGraph(int startPosRow, int startPosCol, Direction startDir) {
	cout << "Begin parsing..." << endl;
	setRowCol(startPosRow, startPosCol);
	dir = startDir;
	parsingNotFinished = true;
	while(parsingNotFinished) {
		cout << "\t@(" << posRow << ", " << posCol << ", " << Encoding::unicodeToUtf8(board->get(posRow, posCol)) << ")" << endl;
		move();
		if(errorMessage != ""){
			cout << "\t" << errorMessage <<endl;
			cout << "\tparsing aborted" << endl;
			break;
		}
	}
	cout << "Finished parsing" << endl;
	return abstractSyntaxGraph;
}

void Parser::move() {

	//straight
	int32_t straightRow = posRow + rowOffsetMap.at(dir).offsets[STRAIGHT];
	int32_t straightCol = posCol + colOffsetMap.at(dir).offsets[STRAIGHT];
	bool straightIsInBoardBounds = straightRow >= 0 && straightRow < board->getHeight() && straightCol >= 0 && straightCol < board->getWidth();

	//left
	int32_t leftRow = posRow + rowOffsetMap.at(dir).offsets[LEFT];
	int32_t leftCol = posCol + colOffsetMap.at(dir).offsets[LEFT];
	bool leftIsInBoardBounds =  leftRow >=0 && leftRow < board->getHeight() && leftCol >=0 && leftCol < board->getWidth();

	//right
	int32_t rightRow = posRow + rowOffsetMap.at(dir).offsets[RIGHT];
	int32_t rightCol = posCol + colOffsetMap.at(dir).offsets[RIGHT];
	bool rightIsInBoardBounds =  rightRow >=0 && rightRow < board->getHeight() && rightCol >=0 && rightCol < board->getWidth();

	//bool vars that will be checked in the end
	bool leftIsValidRail = false;
	bool rightIsValidRail = false;
	bool straightIsValidRail = false;
	if(straightIsInBoardBounds) {
		uint32_t charAtStraight = board->get(straightRow, straightCol);
		list<uint32_t> allowedRails = validRailMap.at(dir).straight;
		straightIsValidRail = std::find(allowedRails.begin(),allowedRails.end(),charAtStraight)!=allowedRails.end();
		if(straightIsValidRail) {
			//if allowedRails contains charAtStraight just move forwards
			setRowCol(straightRow,straightCol);
			if(charAtStraight == leftDirChangeMap.at(dir)){
				turnLeft45Deg();
			}
			if(charAtStraight == rightDirChangeMap.at(dir)){
				turnRight45Deg();
			}
			return;
		}
		//check for other symbols that are allowed
		bool didGoStraight = checkForValidCommandsInStraightDir(straightRow, straightCol);
		if(didGoStraight) {
			return;
		}
	}
	//if we arrive here we could not go straight
	if(currentCharIsNoCrossing()){
		//only if the current char is no crossing we are allowed to look left or right
		if(leftIsInBoardBounds) {
			uint32_t charAtLeft = board->get(leftRow, leftCol);
			list<uint32_t> allowedRailsLeft = validRailMap.at(dir).left;
			leftIsValidRail = std::find(allowedRailsLeft.begin(), allowedRailsLeft.end(), charAtLeft) != allowedRailsLeft.end();
		}
		if(rightIsInBoardBounds) {
			uint32_t charAtRight = board->get(rightRow, rightCol);
			list<uint32_t> allowedRailsRight = validRailMap.at(dir).right;
			rightIsValidRail = std::find(allowedRailsRight.begin(),allowedRailsRight.end(),charAtRight)!=allowedRailsRight.end();
		}
	}

	// ---------------------------------------------------------------------
	// Begin of error handling
	// ---------------------------------------------------------------------
	if(leftIsValidRail && rightIsValidRail) {
		std::stringstream sstm;
		sstm << "'" << board->getName() << "' (" << posRow+1 << ", " << posCol+1 << "): ambiguous move";
		errorMessage = sstm.str();
		return;
	}
	if(!leftIsValidRail && !rightIsValidRail) {
		std::stringstream sstm;
		sstm << "'" << board->getName() << "' (" << posRow+1 << ", " << posCol+1 << "): no valid move possible";
		errorMessage = sstm.str();
		return;
	}
	//error handling end
	if(leftIsValidRail) {
		setRowCol(leftRow,leftCol);
		turnLeft45Deg();
		return;
	}
	if(rightIsValidRail) {
		setRowCol(rightRow,rightCol);
		turnRight45Deg();
		return;
	}
	errorMessage = "'" + board->getName() + "' : end of move-function reached - this should never happen and is an internal error";
	return;
}

bool Parser::currentCharIsNoCrossing(){
	int32_t curChar = board->get(posRow, posCol);
	return curChar != '*' && curChar != '+' && curChar != 'x';
}

bool Parser::checkForValidCommandsInStraightDir(int straightRow, int straightCol) {
	uint32_t charAtStraight = board->get(straightRow, straightCol);
	//cout << "\tcheckForValidCommandsInStraightDir(" << straightRow << ", " << straightCol << ") " << Encoding::unicodeToUtf8(charAtStraight) << endl;
	bool didGoStraight = true;
	NodeIdentifier id{posRow,posCol,dir};
	switch(charAtStraight) {
		case '[':
			setRowCol(straightRow, straightCol);
			//TODO: ueberpruefen ob notwendig: list<char> invalidCharList = {'[','{','(',},;
			parsingNotFinished = addToAbstractSyntaxGraph(readCharsUntil(']'), Command::Type::PUSH_CONST,id);
			//TODO: create pushNode in graph
			break;
		case ']':
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph(readCharsUntil('['), Command::Type::PUSH_CONST,id);
			break;
		case '@':
			setRowCol(straightRow, straightCol);
			reverseDirection();
			break;
		case '#':
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("#", Command::Type::FINISH,id);
			parsingNotFinished = false;
			break;
		case '<':
			didGoStraight = parseJunctions(E, straightRow, straightCol, SE, NE, "<", Command::Type::EASTJUNC,id);
			break;
		case '>':
			didGoStraight = parseJunctions(W, straightRow, straightCol, NW, SW, ">", Command::Type::WESTJUNC,id);
			break;
		case '^':
			didGoStraight = parseJunctions(S, straightRow, straightCol, SW, SE, "^", Command::Type::SOUTHJUNC,id);
			break;
		case 'v':
			didGoStraight = parseJunctions(N, straightRow, straightCol, NE, NW, "v", Command::Type::NORTHJUNC,id);
			break;
		case '{':
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph(readCharsUntil('}'), Command::Type::CALL,id);
			break;
		case '}':
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph(readCharsUntil('{'), Command::Type::CALL,id);
			break;
		case 't':
		case 'f':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph(Encoding::unicodeToUtf8(charAtStraight), Command::Type::PUSH_CONST,id);
			break;

		// System operation
		case 'b': //boom
			setRowCol(straightRow,straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("b", Command::Type::BOOM,id);
			break;
		case 'e': //EOF
			setRowCol(straightRow,straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("e", Command::Type::EOF_CHECK,id);
			break;
		case 'i': //EOF
			setRowCol(straightRow,straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("i", Command::Type::INPUT,id);
			break;
		case 'o':
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("o", Command::Type::OUTPUT,id);
			break;
		case 'u': //Underflowcheck
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("u", Command::Type::UNDERFLOW_CHECK,id);
			break;
		case '?':
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("?", Command::Type::TYPE_CHECK,id);
			break;
		// Arithmetic Operations
		case 'a': // Add
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("a", Command::Type::ADD,id);
			break;
		case 'd': // Divide
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("d", Command::Type::DIV,id);
			break;
		case 'm': // Multiply
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("m", Command::Type::MULT,id);
			break;
		case 'r': // Remainder
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("r", Command::Type::MOD,id);
			break;
		case 's': // Subtract
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("s", Command::Type::SUB,id);
			break;

		// String Operations
		case 'c': // Cut
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("c", Command::Type::CUT,id);
			break;
		case 'z': // Size
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("z", Command::Type::SIZE,id);
			break;
		case 'p': // Append
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("p", Command::Type::APPEND,id);
			break;

		// Conditionals
		case 'g': // Greater than
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("g", Command::Type::GREATER,id);
			break;
		case 'q': // Is Equal
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("q", Command::Type::EQUAL,id);
			break;

		// List Operation
		case 'n': //Nil
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("n", Command::Type::NIL,id);
			break;
		/*case 'c': // Concat
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("c", Command::Type::CONCAT);
			break; */
		/*case '~': // List breakup
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("~", Command::Type::LIST_BREAKUP);
			break;*/

		// Variables
		case '(':
			setRowCol(straightRow, straightCol);
			parsingNotFinished = parseVariable(readCharsUntil(')'),id);
			break;
		case ')':
			setRowCol(straightRow, straightCol);
			parsingNotFinished = parseVariable(readCharsUntil('('),id);
			break;
		default:
			didGoStraight = false;
			break;
	}
	if(errorMessage != "") {
		//TODO:Error stuff?
	}
	return didGoStraight;
}


bool Parser::parseVariable(string data, NodeIdentifier id) {

	// Filter invalid Variable names
	if(data.length() < 3) {
		// Too short
		errorMessage = "Syntax Error: Invalid variable action " + data + ": too short";
		return false;
	}

	// Filter {, } within name
	if(data.find('{') != string::npos || data.find('}') != string::npos) {
		errorMessage = "Syntax Error: Invalid variable action " + data + ": must not contain { or }";
		return false;
	}

	// Filter () within name
	string noBraces = data.substr(1, data.length()-2);
	if(noBraces.find('(') != string::npos || noBraces.find(')') != string::npos) {
		errorMessage = "Syntax Error: Invalid variable action " + data + ": Variable name must not contain ( or )";
		return false;
	}


	if(data.find('!') != string::npos) {

		if(data[1] != '!' || data[data.length()-2] != '!') {
			errorMessage = "Syntax Error: Invalid variable action " + data + ": (!name!) required";
			return false;
		}

		// Filter ! within name
		if(data.substr(2, data.length()-4).find('!') != string::npos) {
			errorMessage = "Syntax Error: Invalid variable action " + data + ": Variable name must not contain '!'";
			return false;
		}

		// Variable pop action
		return addToAbstractSyntaxGraph(data, Command::Type::VAR_POP, id);
	}
	else {
		// Variable push action
		return addToAbstractSyntaxGraph(data, Command::Type::VAR_PUSH, id);
	}
}



/*
 * requiredDir: The direction the train has to have at the moment for this being a valid junction (for dir must be E for < junction)
 * juncX/juncY are the coordinates of the junction
 * truePathDir/falsePathDir are the starting directions of the true/false paths
 * command name: the junction symbol as a string
 * juncType: ast.h Command::type enum value of the junction
*/
bool Parser::parseJunctions(Direction requiredDir, int juncRow, int juncCol, Direction truePathDir, Direction falsePathDir, string commandName, Command::Type juncType, NodeIdentifier id) {
	if(dir==requiredDir){
		parsingNotFinished = addToAbstractSyntaxGraph(commandName, juncType,id);
		if(parsingNotFinished){
			//if the junction was not already parsed recursively call self:
			std::shared_ptr<Node> ifNode = currentNode;
			parseGraph(juncRow, juncCol, truePathDir);
			parsingNotFinished = true;
			currentNode = ifNode;
			addNextNodeAsTruePathOfPreviousNode = false;
			parseGraph(juncRow, juncCol, falsePathDir);
			parsingNotFinished = false;
		}
		return true;
	} else{
		return false;
	}
}

bool Parser::addToAbstractSyntaxGraph(string commandName, Command::Type type, NodeIdentifier id) {
	bool nodeWasNew;
	if(allNodes.find(id)!=allNodes.end()){
		std::shared_ptr<Node> node;
		//node already exists
		cout << "\tReached Node that was already parsed once: " << commandName << endl;
		node = allNodes.at(id);
		abstractSyntaxGraph->addEdge(currentNode, node, addNextNodeAsTruePathOfPreviousNode);
		currentNode = node;
		nodeWasNew = false;
	} else{
		//create a new node
		cout << "\tNode creation: " << commandName << endl;
		std::shared_ptr<Node> node(new Node());
		node->command = {type,commandName};
		//TODO:an Graph Schnittstelle anpassen
		if(abstractSyntaxGraph == NULL) {
			//this is the first node that we meet, create a new one
			node->id = 1;
			lastUsedId = 1;
			abstractSyntaxGraph.reset(new Adjacency_list(board->getName(), node));
		} else {
			node->id = ++lastUsedId;
			abstractSyntaxGraph->addNode(node);
			abstractSyntaxGraph->addEdge(currentNode, node, addNextNodeAsTruePathOfPreviousNode);
		}
		allNodes[id] = node;
		nodeWasNew = true;
		currentNode = node;
	}
	if(!addNextNodeAsTruePathOfPreviousNode) {
		//restore default behavior: always add new nodes to the 'true' path of their predecessor
		//unless it was set before the call of this method (in case an 'IF' was read and we parse the first node of the 'false' branch
		addNextNodeAsTruePathOfPreviousNode = true;
	}
	return nodeWasNew;
}

//FIXME translate -> english
//setzt position auf until falls er existiert, und gibt den gelesenen string inklusive anfangs und endzeichen zurueck
//falls nicht wir ein leerer string zurueckgegeben und die fehlermeldung gesetzt
string Parser::readCharsUntil(uint32_t until) {
	string result = "";

	//result += board->get(posRow, posCol);

	// Can be decoded anyway - doesn't matter
	result += Encoding::unicodeToUtf8(board->get(posRow, posCol));
	while(true) {
		int32_t nextRow = posRow + rowOffsetMap.at(dir).offsets[STRAIGHT];
		int32_t nextCol = posCol + colOffsetMap.at(dir).offsets[STRAIGHT];
		if(posRow >= board->getHeight() || nextCol >= board->getWidth()) {
			//TODO:Dir auch ausgeben
			std::stringstream sstm;
			sstm << "'" << board->getName() << "' (" << posRow+1 << ", " << posCol+1 << "): Parsing ran out of valid space";
			errorMessage = sstm.str();
			return "";
		}
		posRow = nextRow;
		posCol = nextCol;

		//result += board->get(posRow, posCol);
		result += Encoding::unicodeToUtf8(board->get(posRow, posCol));

		if(board->get(posRow, posCol) == until) {
			break;
		}
	}
	//cout << "readCharsUntil: " << result << endl;
	return result;
}

void Parser::turnLeft45Deg() {
	switch(dir) {
		case E: dir = NE; break;
		case SE: dir = E; break;
		case S: dir = SE; break;
		case SW: dir = S; break;
		case W: dir = SW; break;
		case NW: dir = W; break;
		case N: dir = NW; break;
		case NE: dir = N; break;
	}
}

void Parser::turnRight45Deg() {
	switch(dir) {
		case E: dir = SE; break;
		case SE: dir = S; break;
		case S: dir = SW; break;
		case SW: dir = W; break;
		case W: dir = NW; break;
		case NW: dir = N; break;
		case N: dir = NE; break;
		case NE: dir = E; break;
	}
}

void Parser::reverseDirection() {
	switch(dir) {
		case E: dir = W; break;
		case SE: dir = NW; break;
		case S: dir = N; break;
		case SW: dir = NE; break;
		case W: dir = E; break;
		case NW: dir = SE; break;
		case N: dir = S; break;
		case NE: dir = SW; break;
	}
}






