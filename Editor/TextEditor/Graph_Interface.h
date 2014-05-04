#ifndef GRAPHINTERFACE_H
#define GRAPHINTERFACE_H

//Nur diese Klasse verwenden um auf die Interne Repräsentation zuzugreifen
#include "Point.h"

class Graph_Interface
{

private:
    Point *root, *tmp;

public:
    Graph_Interface()
    {
        root = new Point();
    }

    void setSign(int x, int y, char sign)
    {
        root->getPoint(x,y)->setSign(sign);
    }

    void setCharArray(int x, int y, char signs[], int length)
    {
        Point *tmp2;
        int saveX = x, saveY = y, i = 0;
        tmp = root->getPoint(x,y);
        for( ; i < length ; i++)
        {
            if(signs[i] == '\n')
            {
                tmp->setSign(' ');
                tmp = root->getPoint(x,++y);
            }
            else
            {
                tmp->setSign(signs[i]);
                tmp = tmp->getPoint(1,0);
            }
        }
    }

    char getSign(int x,int y)
    {
        return root->getPoint(x,y)->getSign();
    }

    char* getLine(int y)
    {
        return (y < root->countRift()) ? root->getPoint(0,y)->getLine(root->getPoint(0,y)->countLine()): NULL;
    }
};

#endif // GRAPHINTERFACE_H
