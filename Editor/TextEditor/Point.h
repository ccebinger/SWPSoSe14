#include <iostream>
#ifndef POINT_H
#define POINT_H
// Interne Repräsentation, wird nur von Graph_Interface verwendet
class Point{
	private:
	int muster, connections, bCons;
	char sign;
	Point* cons[8];
	

	private:
	void setMuster(void){
		switch(sign){
			case '$' :
				muster = 3;
				break;
			case '@' :
				muster = 255;
				break;
			case '#' :
				muster = 255;
				break;
			case '-' :
				muster = 24;
				break;
			case '|' :
				muster = 66;
				break;
			case '\\' :
				muster = 219;
				break;
			case '/' :
				muster = 126;
				break;
			case '^' :
				muster = 69;
				break;
			case '<' :
				muster = 49;
				break;
			case '>' :
				muster = 134;
				break;
			case 'v' :
				muster = 164;
				break;
			case '[' :
				muster = 255;
				break;
			case ']' :
				muster = 255;
				break;
			case '(' :
				muster = 255;
				break;
			case ')' :
				muster = 255;
				break;
			case '{' :
				muster = 255;
				break;
			case '}' :
				muster = 255;
				break;
			case ' ' :
				muster = 0;
				break;
			default :
				muster = 255;
				break;
		}
	}
	public:
	Point(char sign){
		this->sign = sign;
		setMuster();
		bCons = 0;
		connections = 0;
	}
	Point(){sign = ' ';}
	Point(Point *next, char sign){
		bCons = 8;
		cons[4] = next;
		this->sign = sign;
		setMuster();
		connections = 0;
	}
	Point* getPoint (int x, int y){
		if((bool)y){
			if(bCons & 2)
				return cons[6]->getPoint(x,y-1);
			else {
				bCons |= 2;
				cons[6] = new Point();
				return cons[6]->getPoint(x,y-1);
			} }
		else if((bool)x){
			if(bCons & 8)
				return cons[4]->getPoint(x-1,y);
			else {
				bCons |= 8;
				cons[4] = new Point();
				return cons[4]->getPoint(x-1,y);
			} }
		else
			return this;
	}
	char getSign(void){
		return sign;
	}
	int setSign(char sign){
		this->sign = sign;
		setMuster();
		return bCons;
	}
	char* getLine(int length){
		Point *tmp = this;
		char *line = new char[length];
		for( int z = 0 ; z < length ; z++){
			line[z] = tmp->getSign();
			tmp = tmp->getNextRight();
		}
		line[length] = '\n';
		return line;
	}
	char* getConnectionsLine(int length){
		Point *tmp = this;
		char *line = new char[length];
		for( int z = 0 ; z < length ; z++){
			line[z] = (char)(tmp->getConnections()+48);
			tmp = tmp->getNextRight();
		}
		line[length] = '\n';
		return line;
	}
	int countLine(){
		return (bCons & 8) ? ( 1 + cons[4]->countLine()) : 1;
	}
	int countRift(){
		return (bCons & 2) ? ( 1 + cons[6]->countRift()) : 1;
	}
	void clear(void);
	void clone(Point *akk);
	int getMuster(void){
		return muster;
	}
	void setConnections(int connections, bool mode){
		
		(mode)?this->connections |= connections:this->connections &= (~connections);
	}
	void testConnections(void){
		// TODO: set connections
		std::cout << (connections&muster) << " ";
	}
	int getConnections(void){
		return connections&muster ? 1 : 0;
	}
	void setCons(int pCon, Point *tmp){
		switch(pCon){
			case 128:
				cons[0] = tmp;
				break;
			case 64:
				cons[1] = tmp;
				break;
			case 32:
				cons[2] = tmp;
				break;
			case 16:
				cons[3] = tmp;
				break;
			case 8:
				cons[4] = tmp;
				break;
			case 4:
				cons[5] = tmp;
				break;
			case 2:
				cons[6] = tmp;
				break;
			case 1:
				cons[7] = tmp;
				break;
		}bCons |= pCon;
	}
	void makeCons(void){
		int tmp = bCons;
		if(tmp & 128)cons[0]->setConnections(1,(muster&128)?true:false);
		if(tmp & 64)cons[1]->setConnections(2,(muster&64)?true:false);
		if(tmp & 32)cons[2]->setConnections(4,(muster&32)?true:false);
		if(tmp & 16)cons[3]->setConnections(8,(muster&16)?true:false);
		if(tmp & 8)cons[4]->setConnections(16,(muster&8)?true:false);
		if(tmp & 4)cons[5]->setConnections(32,(muster&4)?true:false);
		if(tmp & 2)cons[6]->setConnections(64,(muster&2)?true:false);
		if(tmp & 1)cons[7]->setConnections(128,(muster&1)?true:false);
	}
	protected:
	Point* getNextRight(void){
		return (bCons & 8) ? cons[4] : NULL;
	}
	Point* getNextBottem(void){
		return (bCons & 2) ? cons[6] : NULL;
	}
	void setNextRight(Point* right){
		cons[4] = right;
		bCons |= 8;
	}
	void setNextBottem(Point *bottem){
		cons[6] = bottem;
		bCons |= 2;
	}
};

void Point::clear(void){
	if(bCons & 8)cons[4]->clear();
	if(bCons & 2)cons[6]->clear();
	delete this;
};
void Point::clone(Point *akk){
	Point *tmp;
	akk->setSign(getSign());
	if(bCons & 8){
		tmp = new Point();
		akk->setNextRight(tmp);
		cons[4]->clone(tmp);
	}
	if(bCons & 2){
		tmp = new Point();
		akk->setNextBottem(tmp);
		cons[2]->clone(tmp);
	}
}
#endif // POINT_H
