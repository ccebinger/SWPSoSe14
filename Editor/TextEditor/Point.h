#ifndef POINT_H
#define POINT_H

#include "InterStack.h"
#include "Stack.h"

class Point{
private:
    int iStart,iConnections,iString,iFunctions,iVariables,b_Cons_to_Nodes,row,colm,mask;
    Point* cons_to_Nodes[8];
    enum Node_Type {Start, End, Rail_Slash, Rail_Junk, Rail_Line, Rail_Corss, Lambda, Reflector, Function, Shadow, String_Symble, Funktion_Symble, Variable_Symbol, White_Space};
    Node_Type type;
    char sign;
public:
    Point ( int colm, int row, char sing );
    Point ( int colm, int row);
    Point* get_Node_in_Direction ( int direction );
    void set_Node_in_Directions ( Point* North_West, Point* North, Point* North_East, Point* West, Point* East, Point* South_West, Point* South, Point* South_East);
    int get_Node_Font ( void );
    void make_Node_Changes ( InternStack* changes );
    int get_Cons_to_Nodes ( void );
    void setSign(char sign);
    int getRow( void );
    int getColm( void );
    char getSign( void );
protected:
    void change_Connections ( bool A_R_Start,bool A_R_iString, bool A_R_iFunctions, bool A_R_iVariable, int direction, InternStack* change);
    void make_Node_Type ( void );
    Node_Type get_Node_Type ( void );
private:
    int make_Node_Connection_Mask( void );
};

#endif // POINT_H
