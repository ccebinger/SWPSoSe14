#ifndef POINT_H
#define POINT_H
// Interne Repräsentation, wird nur von Graph_Interface verwendet
class Point{

private:
    bool connections[3][3];
    char sign;
    Point *right, *bottom;
    bool bRight, bBottom;

public:
    Point(char sign)
    {
        this->sign = sign;
    }

    Point()
    {
        sign = ' ';
    }

    void setPoint (char sign)
    {
        this->sign = sign;
    }

    Point* getPoint (int x, int y)
    {
        if((bool)y)
        {
            if(bBottom)
                return this->bottom->getPoint(x,y-1);
            else
            {
                bBottom = true;
                bottom = new Point();
                return this->bottom->getPoint(x,y-1);
            }
        }
        else if((bool)x)
        {
            if(bRight)
                return this->right->getPoint(x-1,y);
            else
            {
                bRight = true;
                right = new Point();
                return this->right->getPoint(x-1,y);
            }
        }
        else
            return this;
    }

    char getSign(void)
    {
        return sign;
    }

    void setSign(char sign)
    {
        this->sign = sign;
    }

    char* getLine(int length)
    {
        Point *tmp = this;
        int z = 0;
        char *line = new char[length];
        for( ; z < length ; z++)
        {
            line[z] = tmp->getSign();
            tmp = tmp->getNextRight();
        }
        line[length] = '\n';
        return line;
    }

    int countLine()
    {
        return bRight ? ( 1 + right->countLine()) : 1;
    }

    int countRift()
    {
        return bottom ? ( 1 + bottom->countRift()) : 1;
    }

protected:
    Point* getNextRight(void)
    {
        return bRight ? right : NULL;
    }

    Point* getNextBottom(void)
    {
        return bBottom ? bottom : NULL;
    }
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
