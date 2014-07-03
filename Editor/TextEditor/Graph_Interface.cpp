#include "Graph_Interface.h"
#include "InterStack.h"
#include "Stack.h"



Graph_Interface::Graph_Interface( void ){
    root = new Point(0,0,'\0');
    Point *tmp = new Point(1,0), *temp;
    temp = new Point(1,1);
    tmp->set_Node_in_Directions(NULL,NULL,NULL,NULL,NULL,NULL,temp,NULL);
    root->set_Node_in_Directions(NULL,NULL,NULL,NULL,tmp,NULL,new Point(0,1),temp);
    maxColm = 0;
    maxRow = 0;
    check = new DoubleCheck();
    last_Direction = 0;
}

Stack* Graph_Interface::setSign(int colm, int row, char sign){
    Stack *retStack = new Stack();
    InternStack *change = new InternStack();
    Point* temp = getPoint(colm,row,change);
    temp->setSign(sign);
    int direction = get_I_Pointer_Direction(temp);
    do{
        temp->make_Node_Changes(change);
        check->add(temp);
    }while((temp = change->pop()) != NULL);
    while((temp = check->get()) != NULL){
        retStack->push(temp->getColm(),temp->getRow(),temp->getSign(),temp->get_Node_Font());
    }
    //retStack->push(-1,-1,' ',direction);
    return retStack;
}
Stack* Graph_Interface::deleteSign(int colm, int row){
    return setSign(colm,row,'\0');
}
char Graph_Interface::getSign(int colm, int row)
{
    Point *tmp = getPoint(colm, row, NULL);
    if(tmp)
    {
        return tmp->getSign();
    }
    return '\0';
}
void Graph_Interface::clear(){
    for (int x = maxColm; x >= 0 ; x--) {
        for (int y = maxRow; y >= 0; y--) {
            delete getPoint(x,y,NULL);
        }
    }
    root = new Point(0,0,'\0');
    Point *tmp = new Point(1,0), *temp;
    temp = new Point(1,1);
    tmp->set_Node_in_Directions(NULL,NULL,NULL,NULL,NULL,NULL,temp,NULL);
    root->set_Node_in_Directions(NULL,NULL,NULL,NULL,tmp,NULL,new Point(0,1),temp);
    maxColm = 0;
    maxRow = 0;
    last_Direction = 0;
}
void Graph_Interface::make_Graph_to(int colm, int row, InternStack *change){
    Point* temp;
    for (int tmpCol = 0; tmpCol <= ((maxColm>colm)?maxColm:colm); ++tmpCol) {
        for (int tmpRow = 0; tmpRow <= ((maxRow>row)?maxRow:row); ++tmpRow) {
            if(tmpCol > maxColm || tmpRow > maxRow){
                temp = get_and_make_Point(tmpCol,tmpRow);
                temp->set_Node_in_Directions((tmpCol-1 >=0 && tmpRow-1 >= 0)?get_and_make_Point(tmpCol-1,tmpRow-1):NULL,
                                             (tmpRow-1 >= 0)?get_and_make_Point(tmpCol,tmpRow-1):NULL,
                                             (tmpRow-1 >= 0)?get_and_make_Point(tmpCol+1,tmpRow-1):NULL,
                                             (tmpCol-1 >=0 )?get_and_make_Point(tmpCol-1,tmpRow):NULL,
                                             get_and_make_Point(tmpCol+1,tmpRow),
                                             (tmpCol-1 >=0)?get_and_make_Point(tmpCol-1,tmpRow+1):NULL,
                                             get_and_make_Point(tmpCol,tmpRow+1),
                                             get_and_make_Point(tmpCol+1,tmpRow+1));
                change->push(temp);
            }
        }
    } maxColm = (maxColm>colm)?maxColm:colm; maxRow = (maxRow>row)?maxRow:row;

}
Point* Graph_Interface::getPoint(int colm, int row, InternStack *change){
    if(maxColm < colm || maxRow < row) make_Graph_to(colm,row,change);
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

int Graph_Interface::get_I_Pointer_Direction(Point* to_Direction){
    int direction = to_Direction->get_I_Direction();
    if(direction){
        last_Direction = direction;
        return direction;
    }else{
        if(last_Direction) return last_Direction;
        else return 8;
    }
    return 8;
}
int Graph_Interface::get_Point_Type(int colm, int row){
    return getPoint(colm,row,NULL)->get_Node_Font();
}
