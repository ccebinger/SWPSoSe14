#ifndef GRAPH_INTERFACE_H
#define GRAPH_INTERFACE_H

#include "Point.h"

class Graph_Interface{
private:
    Point* root;
    DoubleCheck *check;
    int maxRow, maxColm, last_Direction, l_row, l_colm;
public:
    Graph_Interface( void );
    Stack* setSign(int colm, int row, char sign);
    Stack* deleteSign(int colm, int row);
    void clear(void);
private:
    void make_Graph_to(int colm, int row, InternStack *change);
    Point* getPoint(int colm, int row,InternStack *change);
    Point* get_and_make_Point(int colm, int row);
    int get_I_Pointer_Direction(Point* to_Direction);
};

#endif // GRAPH_INTERFACE_H
