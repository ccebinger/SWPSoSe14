#include "Graph_Interface.h"
#include "InterStack.h"
#include "Stack.h"



Graph_Interface::Graph_Interface( void ){
    root = new Point(0,0,' ');
    Point *tmp = new Point(1,0), *temp;
    temp = new Point(1,1);
    tmp->set_Node_in_Directions(NULL,NULL,NULL,NULL,NULL,NULL,temp,NULL);
    root->set_Node_in_Directions(NULL,NULL,NULL,NULL,tmp,NULL,new Point(0,1),temp);
    maxCalm = 0;
    maxRow = 0;
    check = new DoubleCheck();
}

Stack* Graph_Interface::setSign(int colm, int row, char sign){
    Stack *retStack = new Stack();
    InternStack *change = new InternStack();
    Point* temp = getPoint(colm,row,change);
    temp->setSign(sign);
    do{
        temp->make_Node_Changes(change);
        check->add(temp);
    }while((temp = change->pop()) != NULL);
    while((temp = check->get()) != NULL){
        retStack->push(temp->getColm(),temp->getRow(),temp->getSign(),temp->get_Node_Font());
    }
    return retStack;
}
Stack* Graph_Interface::deleteSign(int colm, int row){
    return setSign(colm,row,' ');
}
void Graph_Interface::clear(){
    for (int x = maxCalm; x >= 0 ; x--) {
        for (int y = maxRow; y >= 0; y--) {
            delete getPoint(x,y,NULL);
        }
    }
    root = new Point(0,0,' ');
    Point *tmp = new Point(1,0), *temp;
    temp = new Point(1,1);
    tmp->set_Node_in_Directions(NULL,NULL,NULL,NULL,NULL,NULL,temp,NULL);
    root->set_Node_in_Directions(NULL,NULL,NULL,NULL,tmp,NULL,new Point(0,1),temp);
    maxCalm = 0;
    maxRow = 0;
}
void Graph_Interface::make_Graph_to(int colm, int row, InternStack *change){
    Point* temp;
    for (int x = 0; x <= colm; ++x) {
        for (int y = 0; y <= row; ++y) {
            if(x > maxCalm || y>maxRow){
                temp = get_and_make_Point(x,y);
                temp->set_Node_in_Directions((x-1 >=0 && y-1 >= 0)?get_and_make_Point(x-1,y-1):NULL,
                                             (y-1 >= 0)?get_and_make_Point(x,y-1):NULL,
                                             (y-1 >= 0)?get_and_make_Point(x+1,y-1):NULL,
                                             (x-1 >=0 )?get_and_make_Point(x-1,y):NULL,
                                             get_and_make_Point(x+1,y),
                                             (x-1 >=0)?get_and_make_Point(x-1,y+1):NULL,
                                             get_and_make_Point(x,y+1),
                                             get_and_make_Point(x+1,y+1));
                change->push(temp);
            }
        }
    } maxCalm = colm; maxRow = row;

}
Point* Graph_Interface::getPoint(int colm, int row, InternStack *change){
    if(maxCalm < colm || maxRow < row) make_Graph_to(colm,row,change);
    int var = 0;
    Point* temp = root;
    for (; var < colm && var < row; var++) {
        temp = temp->get_Node_in_Direction(1);
    }
    for (int y = var; y < colm; y++) {
        temp = temp->get_Node_in_Direction(8);
    }
    for (int x = var; x < row; x++) {
        temp = temp->get_Node_in_Direction(2);
    }
    return temp;
}
Point* Graph_Interface::get_and_make_Point(int colm, int row){
    Point *temp1 = root, *temp2 = NULL;
    for (int var = 0; var < colm; ++var) {
        temp2 = temp1->get_Node_in_Direction(8);
        if(temp2 == NULL){
            temp2 = new Point(var+1,0);
            temp1->set_Node_in_Directions(NULL,NULL,NULL,NULL,temp2,NULL,NULL,NULL);
        }
        temp1 = temp2;
    }
    for (int var = 0; var < row; ++var) {
        temp2 = temp1->get_Node_in_Direction(2);
        if(temp2 == NULL){
            temp2 = new Point(colm,var+1);
            temp1->set_Node_in_Directions(NULL,NULL,NULL,NULL,NULL,NULL,temp2,NULL);
        }
        temp1 = temp2;
    }
    return temp1;
}
