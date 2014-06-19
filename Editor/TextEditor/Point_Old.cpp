/*#include "InterStack.h"
#include "Stack.h"
#include "Point.h"

void Point::clear(void){
    if(bCons & 8)cons[4]->clear();
    if(bCons & 2)cons[6]->clear();
    delete this;
}
void Point::clone(Point *akk){
    Point *tmp;
    akk->setSign(getSign());
    if(bCons & 8){
        tmp = new Point(this->x+1,this->y);
        akk->setNextRight(tmp);
        cons[4]->clone(tmp);
    }
    if(bCons & 2){
        tmp = new Point(this->x,this->y+1);
        akk->setNextBottem(tmp);
        cons[2]->clone(tmp);
    }
}*/
