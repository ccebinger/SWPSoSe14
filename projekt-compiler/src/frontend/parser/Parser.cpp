/*
 * Parser.cpp
 *
 *  Created on: 30.04.2014
 *      Author: LeonBornemann
 */


#include <frontend/lexer/Lexer.h>
#include <frontend/parser/Parser.h>
#include <vector>

bool operator<( const Djp &l, const Djp &r ){
	if(l.path == r.path){
			if(l.junction==r.junction){
				//cols and rows are equal, dir is decisive for comparison
				return l.dir < r.dir;
			} else{
				//rows are equal, col is decisive for comparison
				return l.junction<r.junction;
			}
		} else{
			//row is decisive for comparison
			return l.path<r.path;
		}
}

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

Parser::Parser(map<string,shared_ptr<RailFunction>> lexedFunctions) {
	this->lexedFunctions = lexedFunctions;
	currentAbstractSyntaxGraph = nullptr;
}

void Parser::setRowCol(int newRow, int newCol){
	posRow = newRow;
	posCol = newCol;
}

Graphs Parser::parseGraphs(Graphs graphs) {
	this->graphs = graphs;
	//mark all already existing function names as used so they cant get overwritten when lambda function names are generated!
	for (std::map<string, std::shared_ptr<RailFunction>>::iterator it = lexedFunctions.begin(); it != lexedFunctions.end(); ++it){
		allCurrentlyUsedFunctionNames.insert(it->first);
	}
	for (std::map<string, std::shared_ptr<RailFunction>>::iterator it = lexedFunctions.begin(); it != lexedFunctions.end(); ++it)
	{
	  this->board = it->second;
	  Parser::parseCompleteBoard(it->first);
	}
	return this->graphs;
	//this->board = railFunction;
}

void Parser::parseLambdaFunction(string fname,int startPosRow, int startPosCol, Direction startDir){
	currentAbstractSyntaxGraph = nullptr;
	addNextNodeAsTruePathOfPreviousNode = true;
	std::shared_ptr<Adjacency_list> graph = parseGraph(startPosRow,startPosCol,startDir);
	if(graph == nullptr){
		throw EnvException(FRONTEND_PARSER, "Anonymous Function undefined in " + Env::getSrcFile());
	}
	graphs.put(fname,graph);
}

void Parser::parseCompleteBoard(string fname) {
	currentAbstractSyntaxGraph = nullptr;
	addNextNodeAsTruePathOfPreviousNode = true;
	int startPosCol=-1;
	//find $, initiate pos
	for(int32_t i=0; i<board->getWidth(); ++i) {
		if(board->get(0, i) == '$') {
			startPosCol = i;
			break;
		}
	}
	if(startPosCol == -1) {
		throw new EnvException(FRONTEND_PARSER, "$ not found");
	}
	std::shared_ptr<Adjacency_list> graph = parseGraph(0,startPosCol,SE);
	if(graph == nullptr){
		throw EnvException(FRONTEND_PARSER, "Function undefined in " + Env::getSrcFile());
	}
	graphs.put(fname,graph);
}

std::shared_ptr<Adjacency_list> Parser::parseGraph(int startPosRow, int startPosCol, Direction startDir) {
	if(Env::verbose()) {
		cout << "Begin parsing..." << endl;
	}
	setRowCol(startPosRow, startPosCol);
	dir = startDir;
	parsingNotFinished = true;
	while(parsingNotFinished) {
		if(Env::verbose()) {
			cout << "\t@(" << board->funcStartLine + posRow << ", " << posCol+1 << ", " << Encoding::unicodeToUtf8(board->get(posRow, posCol)) << ")" << endl;
		}
		move();
		moveCount++;
		if(moveCount >= board->getWidth()*board->getHeight()*8){
			// we have detected an inifinite loop.
			//TODO: add infinite noop loop to ast and terminate
			cout << "warning: empty infinite loop detected!";
			//dummy default values for the nodeIdentifier since they are not relevant
			NodeIdentifier id{0,0,E};
			addToAbstractSyntaxGraph(".",Command::Type::NOOP,id);
			addToAbstractSyntaxGraph(".",Command::Type::NOOP,id);
			parsingNotFinished = false;
		}
	}
	if(Env::verbose()) {
		cout << "Finished parsing" << endl;
	}
	return currentAbstractSyntaxGraph;
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
		throw EnvException(FRONTEND_PARSER, board->getName() + ": ambiguous move", board->funcStartLine + posRow, posCol + 1);
	}
	if(!leftIsValidRail && !rightIsValidRail) {
		throw EnvException(FRONTEND_PARSER, board->getName() + ": no valid move possible", board->funcStartLine + posRow, posCol + 1);
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

	throw EnvException(FRONTEND_PARSER, board->getName() + ": end of move-function reached - this should never happen and is an internal error", board->funcStartLine + posRow, posCol + 1);
}

bool Parser::currentCharIsNoCrossing(){
	int32_t curChar = board->get(posRow, posCol);
	return curChar != '*' && curChar != '+' && curChar != 'x';
}

bool Parser::checkForValidCommandsInStraightDir(int straightRow, int straightCol) {
	uint32_t charAtStraight = board->get(straightRow, straightCol);

//	if(Env::verbose()) {
//		cout << "\tcheckForValidCommandsInStraightDir(" << straightRow << ", " << straightCol << ") " << Encoding::unicodeToUtf8(charAtStraight) << endl;
//	}

	//
	bool didGoStraight = true;
	NodeIdentifier id{posRow, posCol, dir};
	switch(charAtStraight) {

		//Delimiter
		case '[': // Reading Constant
			setRowCol(straightRow, straightCol);
			//TODO: ueberpruefen ob notwendig: list<char> invalidCharList = {'['};
			//'\[\' it will be parsed as '\t\' , '\\' , '\n\' within the String(Constant)
			//but (,{,},) are as any other chars .
			parsingNotFinished = addToAbstractSyntaxGraph(readConstantStringUntil(']'), Command::Type::PUSH_CONST, id);
			//TODO: create pushNode in graph
			break;
		case ']': // Reading Constant
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph(readConstantStringUntil('['), Command::Type::PUSH_CONST, id);
			break;
		case '{': // Function call
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph(readCharsUntil('}'), Command::Type::CALL, id);
			break;
		case '}': // Function call
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph(readCharsUntil('{'), Command::Type::CALL, id);
			break;
		case '(': // Variable push / pop
			setRowCol(straightRow, straightCol);
			parseVariable(readCharsUntil(')'), id);
			break;
		case ')': // Variable push / pop
			setRowCol(straightRow, straightCol);
			parseVariable(readCharsUntil('('), id);
			break;

		// Misc
		case '@': // Reflector
			setRowCol(straightRow, straightCol);
			reverseDirection();
			break;
		case '#': // Finish
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("#", Command::Type::FINISH, id);
			parsingNotFinished = false;
			break;

		// Juctions
		case '<':
			didGoStraight = parseJunctions({E,SW,NW}, straightRow, straightCol, "<", Command::Type::EASTJUNC, id);
			break;
		case '>':
			didGoStraight = parseJunctions({W,NE,SE}, straightRow, straightCol, ">", Command::Type::WESTJUNC, id);
			break;
		case '^':
			didGoStraight = parseJunctions({S,NW,NE}, straightRow, straightCol, "^", Command::Type::SOUTHJUNC, id);
			break;
		case 'v':
			didGoStraight = parseJunctions({N,SW,SE}, straightRow, straightCol, "v", Command::Type::NORTHJUNC, id);
			break;

		//Constant Numbers
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
			parsingNotFinished = addToAbstractSyntaxGraph(Encoding::unicodeToUtf8(charAtStraight), Command::Type::PUSH_CONST, id);
			break;

		// System operation
		case 'b': //Boom
			setRowCol(straightRow,straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("b", Command::Type::BOOM, id);
			break;
		case 'e': //EOF
			setRowCol(straightRow,straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("e", Command::Type::EOF_CHECK, id);
			break;
		case 'i': //Input
			setRowCol(straightRow,straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("i", Command::Type::INPUT, id);
			break;
		case 'o': //Output
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("o", Command::Type::OUTPUT, id);
			break;
		case 'u': //Underflowcheck
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("u", Command::Type::UNDERFLOW_CHECK, id);
			break;
		case '?': // Typecheck
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("?", Command::Type::TYPE_CHECK, id);
			break;

		// Arithmetic Operations
		case 'a': // Add
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("a", Command::Type::ADD, id);
			break;
		case 'd': // Divide
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("d", Command::Type::DIV, id);
			break;
		case 'm': // Multiply
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("m", Command::Type::MULT, id);
			break;
		case 'r': // Remainder
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("r", Command::Type::MOD, id);
			break;
		case 's': // Subtract
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("s", Command::Type::SUB, id);
			break;

		// String Operations
		case 'c': // Cut
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("c", Command::Type::CUT, id);
			break;
		case 'z': // Size
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("z", Command::Type::SIZE, id);
			break;
		case 'p': // Append
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("p", Command::Type::APPEND, id);
			break;

		// Conditionals
		case 'g': // Greater than
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("g", Command::Type::GREATER, id);
			break;
		case 'q': // Is Equal
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("q", Command::Type::EQUAL, id);
			break;
		case 't': // True
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("1", Command::Type::TRUE, id);
			break;
		case 'f': // False
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("0", Command::Type::FALSE, id);
			break;

		// List Operation
		case 'n': //Nil
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("n", Command::Type::NIL, id);
			break;
		case ':': // Concat
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph(":", Command::Type::LIST_CONS, id);
			break;
		case '~': // List breakup
			setRowCol(straightRow, straightCol);
			parsingNotFinished = addToAbstractSyntaxGraph("~", Command::Type::LIST_BREAKUP, id);
			break;
		case '&': // anonymous function
		{
			setRowCol(straightRow, straightCol);
			parseLambda(id);
			break;
		}
		default:
			didGoStraight = false;
			break;
	}
	return didGoStraight;
}

void Parser::parseLambda(NodeIdentifier id){
	string lambdaName = findUnusedFunctionName();
	parsingNotFinished = addToAbstractSyntaxGraph(lambdaName, Command::Type::LAMBDA, id);
	//save status before parsing lambda function, so it can be restored afterwards.
	int prePosRow = posRow;
	int prePosCol = posCol;
	Direction preDir = dir;
	bool preParsingNotFinished = parsingNotFinished;
	std::shared_ptr<Adjacency_list> preASG = currentAbstractSyntaxGraph;
	std::shared_ptr<Node> preCurrentNode = currentNode;
	//parse lambda function
	parseLambdaFunction(lambdaName,posRow,posCol,dir);
	//reset status after parsing lambda function:
	posRow = prePosRow;
	posCol = prePosCol;
	dir = preDir;
	currentAbstractSyntaxGraph = preASG;
	currentNode = preCurrentNode;
	parsingNotFinished = preParsingNotFinished;
	reverseDirection();
}

string Parser::findUnusedFunctionName(){
	int count = 0;
	string name;
	while(true){
		std::ostringstream s;
		s << "&" << count;
		name = s.str();
		if(allCurrentlyUsedFunctionNames.find(name)==allCurrentlyUsedFunctionNames.end()){
			allCurrentlyUsedFunctionNames.insert(name);
			break;
		}
		count++;
	}
	return name;
}


void Parser::parseVariable(string data, NodeIdentifier id) {

	// Filter invalid Variable names
	if(data.length() < 3) {
		// Too short
		throw EnvException(FRONTEND_PARSER, board->getName() + ": Syntax Error: Invalid variable action " + data + ": too short", board->funcStartLine + posRow, posCol + 1);
	}

	// Filter {, } within name
	if(data.find('{') != string::npos || data.find('}') != string::npos) {
		throw EnvException(FRONTEND_PARSER, board->getName() + ": Syntax Error: Invalid variable action " + data + ": must not contain { or }", board->funcStartLine + posRow, posCol + 1);
	}

	// Filter () within name
	string noBraces = data.substr(1, data.length()-2);
	if(noBraces.find('(') != string::npos || noBraces.find(')') != string::npos) {
		throw EnvException(FRONTEND_PARSER, board->getName() + ": Syntax Error: Invalid variable action " + data + ": Variable name must not contain ( or )", board->funcStartLine + posRow, posCol + 1);
	}


	if(data.find('!') != string::npos) {

		if(data[1] != '!' || data[data.length()-2] != '!') {
			throw EnvException(FRONTEND_PARSER, board->getName() + ": Syntax Error: Invalid variable action " + data + ": (!name!) required", board->funcStartLine + posRow, posCol + 1);
		}

		// Filter ! within name
		if(data.substr(2, data.length()-4).find('!') != string::npos) {
			throw EnvException(FRONTEND_PARSER, board->getName() + ": Syntax Error: Invalid variable action " + data + ": Variable name must not contain '!'", board->funcStartLine + posRow, posCol + 1);
		}

		// Variable pop action
		addToAbstractSyntaxGraph(data, Command::Type::VAR_PUSH, id);
	}
	else {
		// Variable push action
		addToAbstractSyntaxGraph(data, Command::Type::VAR_POP, id);
	}
}



/*
 * requiredDir: The direction the train has to have at the moment for this being a valid junction (for dir must be E for < junction)
 * juncX/juncY are the coordinates of the junction
 * truePathDir/falsePathDir are the starting directions of the true/false paths
 * command name: the junction symbol as a string
 * juncType: ast.h Command::type enum value of the junction
*/
bool Parser::parseJunctions(list<Direction> requiredDir, int juncRow, int juncCol, string commandName, Command::Type juncType, NodeIdentifier id) {
	if(std::find(requiredDir.begin(),requiredDir.end(),dir) != requiredDir.end()){
		parsingNotFinished = addToAbstractSyntaxGraph(commandName, juncType,id);
		if(parsingNotFinished){
			//if the junction was not already parsed recursively call self:
			std::shared_ptr<Node> ifNode = currentNode;
			Direction truePathDir = getOutPathOfJunction(commandName,dir,true);
			Direction falsePathDir = getOutPathOfJunction(commandName,dir,false);
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

Direction Parser::getOutPathOfJunction(string junction,Direction curDir,bool path){
	return juncDirChangeMap.at({curDir,junction,path});
}

bool Parser::addToAbstractSyntaxGraph(string commandName, Command::Type type, NodeIdentifier id) {
	bool nodeWasNew;
	if(allNodes.find(id)!=allNodes.end()){
		std::shared_ptr<Node> node;
		//node already exists
		if(Env::verbose()) {
			cout << "\tReached Node that was already parsed once: " << commandName << endl;
		}
		node = allNodes.at(id);
		currentAbstractSyntaxGraph->addEdge(currentNode, node, addNextNodeAsTruePathOfPreviousNode);
		currentNode = node;
		nodeWasNew = false;
	} else{
		//create a new node
		if(Env::verbose()) {
			cout << "\tNode creation: " << commandName << endl;
		}
		std::shared_ptr<Node> node(new Node());
		node->command = {type,commandName};
		//TODO:an Graph Schnittstelle anpassen
		if(currentAbstractSyntaxGraph == NULL) {
			//this is the first node that we meet, create a new one
			node->id = 1;
			currentAbstractSyntaxGraph.reset(new Adjacency_list(board->getName(), node));
		} else {
			node->id = currentNode->id+1;
			currentAbstractSyntaxGraph->addNode(node);
			currentAbstractSyntaxGraph->addEdge(currentNode, node, addNextNodeAsTruePathOfPreviousNode);
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

string Parser::readConstantStringUntil(uint32_t until) {
	//doubly escaped symbols, i.e "\\n\\" for example
	list<string> escapableSymbols{"n","t","]","["};
	list<string> mustEscapeSymbols{"]","["};
	string result = "";
	result += Encoding::unicodeToUtf8(board->get(posRow, posCol));
	while(true) {
		stepStraight();
		string curChar = Encoding::unicodeToUtf8(board->get(posRow, posCol));
		if(curChar == "\\"){
			//we need to read an escaped character
			stepStraight();
			string secondChar = Encoding::unicodeToUtf8(board->get(posRow, posCol));
			if(secondChar == "\\"){
				//the only allowed single-escaped character was found: continue
				result += curChar += secondChar;
			} else if(std::find(escapableSymbols.begin(),escapableSymbols.end(),secondChar) != escapableSymbols.end()){
				//now another '\\' needs to follow
				stepStraight();
				string thirdChar =  Encoding::unicodeToUtf8(board->get(posRow, posCol));
				if(thirdChar == "\\"){
					result += curChar += secondChar += thirdChar;
				} else{
					throw EnvException(FRONTEND_PARSER, board->getName() + ": Invalid escape sequence in constant string: All escapable characters except for \\ must be escaped by \\ on both sides", board->funcStartLine + posRow, posCol + 1);
				}
			} else{
				throw EnvException(FRONTEND_PARSER, board->getName() + ": Invalid escape sequence in constant string: \\ must be followed by '\\','n','t',']' or '['", board->funcStartLine + posRow, posCol + 1);
			}
		} else if (std::find(mustEscapeSymbols.begin(),mustEscapeSymbols.end(),curChar)!=mustEscapeSymbols.end()){
			if(curChar == Encoding::unicodeToUtf8(until)){
				//we reached the end
				result +=curChar;
				break;
			} else{
				throw EnvException(FRONTEND_PARSER, board->getName() + ": Invalid character in constant string: '" + curChar + "' needs to be escaped", board->funcStartLine + posRow, posCol + 1);
			}
		} else{
			//it was a normal char just append it
			result +=curChar;
		}
	}
	return result;
}

void Parser::stepStraight(){
	int32_t nextRow = posRow + rowOffsetMap.at(dir).offsets[STRAIGHT];
	int32_t nextCol = posCol + colOffsetMap.at(dir).offsets[STRAIGHT];
	if(posRow >= board->getHeight() || nextCol >= board->getWidth()) {
		throw EnvException(FRONTEND_PARSER, board->getName() + ": Parsing ran out of valid function space", board->funcStartLine + posRow, posCol + 1);
	}
	posRow = nextRow;
	posCol = nextCol;
}

//set the position to 'until' if it exists then return the return the string ,which was read ,including the starts- and end-symbol
//just if we didi not return an empty string and the error message is already set.
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
			throw EnvException(FRONTEND_PARSER, board->getName() + ": Parsing ran out of valid function space", board->funcStartLine + posRow, posCol + 1);
		}
		posRow = nextRow;
		posCol = nextCol;

		//result += board->get(posRow, posCol);
		result += Encoding::unicodeToUtf8(board->get(posRow, posCol));

		if(board->get(posRow, posCol) == until) {
			break;
		}
	}

//	if(Env::verbose()) {
//		cout << "readCharsUntil: " << result << endl;
//	}
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






