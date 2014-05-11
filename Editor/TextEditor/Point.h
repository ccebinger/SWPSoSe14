#ifndef POINT_H
#define POINT_H
// Interne Repräsentation, wird nur von Graph_Interface verwendet
class Point{
	private:
		unsigned int connections : 8, muster : 8;
		char sign;
		Point *right, *bottem;
		bool bRight, bBottem;
	
	public:
	Point(char sign){
		this->sign = sign;
	}
	Point(){sign = ' ';}
	Point(Point *next, char sign){
		bRight = true;
		right = next;
		this->sign = sign;
	}
	void setPoint (char sign){
		this->sign = sign;
	}
	Point* getPoint (int x, int y){
		if((bool)y){
			if(bBottem)
				return this->bottem->getPoint(x,y-1);
			else {
				bBottem = true;
				bottem = new Point();
				return this->bottem->getPoint(x,y-1);
			} }
		else if((bool)x){
			if(bRight)
				return this->right->getPoint(x-1,y);
			else {
				bRight = true;
				right = new Point();
				return this->right->getPoint(x-1,y);
			} }
		else
			return this;
	}
	char getSign(void){
		return sign;
	}
	void setSign(char sign){
		this->sign = sign;
	}
	char* getLine(int length){
		Point *tmp = this;
		int z = 0;
		char *line = new char[length];
		for( ; z < length ; z++){
			line[z] = tmp->getSign();
			tmp = tmp->getNextRight();
		}
		line[length] = '\n';
		return line;
	}
	int countLine(){
		return bRight ? ( 1 + right->countLine()) : 1;
	}
	int countRift(){
		return bottem ? ( 1 + bottem->countRift()) : 1;
	}
	void insertSign(char sign){
		right = new Point(right,this->sign);
		this->sign = sign;
	}
	void clear(void);
	bool deleteSign(void);
	void clone(Point *akk);
	protected:
	Point* getNextRight(void){
		return bRight ? right : NULL;
	}
	Point* getNextBottem(void){
		return bBottem ? bottem : NULL;
	}
	void setNextRight(Point* right){
		this->right = right;
		bRight = true;
	}
	void setNextBottem(Point *bottem){
		this->bottem = bottem;
		bBottem = true;
	}
	private:
	void setConnections(void);
};

void Point::clear(void){
	if(bRight)right->clear();
	if(bBottem) bottem->clear();
	delete this;
};
bool Point::deleteSign(void){
	if(bRight){ sign = right->getSign(); if(right->deleteSign()) bRight = false ; return false;}
	else delete this;
	return true;
};
void Point::clone(Point *akk){
	Point *tmp;
	akk->setSign(getSign());
	if(bRight){
		tmp = new Point();
		akk->setNextRight(tmp);
		right->clone(tmp);
	}
	if(bBottem){
		tmp = new Point();
		akk->setNextBottem(tmp);
		bottem->clone(tmp);
	}
}
#endif // POINT_H
