#ifndef GRAPH_INTERFACE_H
#define GRAPH_INTERFACE_H

#include "Point.h"
#include "ApplicationPreferences.h"

class Graph_Interface{
private:
    Point* root;
    DoubleCheck *check;
    int maxRow, maxColm, last_Direction, l_row, l_colm;
public:
    Graph_Interface( void );
    Stack* setSign(int colm, int row, char sign);
    Stack* deleteSign(int colm, int row);
    char getSign(int colm, int row);
    void clear(void);
    int get_Point_Type(int colm, int row);
    void directionToDelta(int *deltaX, int *deltaY, ApplicationConstants::Direction direction) const;
    Stack* insert_Row_at_Position(int row, int colm, char* signs, int length);
    Stack* insert_Colm_at_Position(int row, int colm, char* signs, int length);
    Stack* insert_Block_at_Position(int row, int colm, char* signs, int length);
private:
    void make_Graph_to(int colm, int row, InternStack *change);
    Point* getPoint(int colm, int row,InternStack *change);
    Point* get_and_make_Point(int colm, int row);
    int get_I_Pointer_Direction(Point* to_Direction);
};

#endif // GRAPH_INTERFACE_H
