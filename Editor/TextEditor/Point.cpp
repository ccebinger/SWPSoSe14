#include "Point.h"
#include "InterStack.h"

Point::Point ( int colm, int row, char sign){
    this->row = row;
    this->colm = colm;
    this->sign = sign;
    make_Node_Type();
    iStart = 0;iConnections = 0;iString = 0;iFunctions = 0;iVariables = 0;b_Cons_to_Nodes = 0;
}
Point::Point (int colm, int row ){
    this->row = row;
    this->colm = colm;
    this->type = Shadow;
    sign = '\0';
    iStart = 0;iConnections = 0;iString = 0;iFunctions = 0;iVariables = 0;b_Cons_to_Nodes = 0;
}

Point* Point::get_Node_in_Direction ( int direction ){
    Point* temp;
    switch (direction) {
    case 128:
        temp = ( b_Cons_to_Nodes & 128 ) ? cons_to_Nodes[0] : NULL;
        break;
    case 64:
        temp = ( b_Cons_to_Nodes & 64 ) ? cons_to_Nodes[1] : NULL;
        break;
    case 32:
        temp = ( b_Cons_to_Nodes & 32 ) ? cons_to_Nodes[2] : NULL;
        break;
    case 16:
        temp = ( b_Cons_to_Nodes & 16 ) ? cons_to_Nodes[3] : NULL;
        break;
    case 8:
        temp = ( b_Cons_to_Nodes & 8 ) ? cons_to_Nodes[4] : NULL;
        break;
    case 4:
        temp = ( b_Cons_to_Nodes & 4 ) ? cons_to_Nodes[5] : NULL;
        break;
    case 2:
        temp = ( b_Cons_to_Nodes & 2 ) ? cons_to_Nodes[6] : NULL;
        break;
    case 1:
        temp = ( b_Cons_to_Nodes & 1 ) ? cons_to_Nodes[7] : NULL;
        break;
    default:
        temp = NULL;
        break;
    }
    return temp;
}
void Point::set_Node_in_Directions(Point *North_West, Point *North, Point *North_East, Point *West, Point *East, Point *South_West, Point *South, Point *South_East){
    if( North_West != NULL ){ b_Cons_to_Nodes |= 128; cons_to_Nodes[0] = North_West;}
    if( North != NULL ){ b_Cons_to_Nodes |= 64; cons_to_Nodes[1] = North;}
    if( North_East != NULL ){ b_Cons_to_Nodes |= 32; cons_to_Nodes[2] = North_East;}
    if( West != NULL ){ b_Cons_to_Nodes |= 16; cons_to_Nodes[3] = West;}
    if( East != NULL ){ b_Cons_to_Nodes |= 8; cons_to_Nodes[4] = East;}
    if( South_West != NULL ){ b_Cons_to_Nodes |= 4; cons_to_Nodes[5] = South_West;}
    if( South != NULL ){ b_Cons_to_Nodes |= 2; cons_to_Nodes[6] = South;}
    if( South_East != NULL ){ b_Cons_to_Nodes |= 1; cons_to_Nodes[7] = South_East;}
}
int Point::get_Node_Font( void ){
    int font = 0;
    int legal_Start = 0;
    if( iStart & 128 && cons_to_Nodes[0]->get_Node_Type() != Rail_Line) legal_Start = 128;
    if( iStart & 32 && cons_to_Nodes[2]->get_Node_Type() != Rail_Line) legal_Start = 32;
    if( iStart & 4 && cons_to_Nodes[5]->get_Node_Type() != Rail_Line) legal_Start = 4;
    if( iStart & 1 && cons_to_Nodes[7]->get_Node_Type() != Rail_Line) legal_Start = 1;
    legal_Start |= iStart & (64+16+8+2);
    switch (type) {
    case Start:
    case End:
        if( legal_Start ) font = 0x00FF0000;
    case Lambda:
    case Reflector:
    case Function:
        if( legal_Start && !(iString) && !(iFunctions) && !(iVariables) ) font = 0x00FF0000;
        else if ( mask && iString && (iFunctions || iVariables || (legal_Start & ~ iString)) ) font = 0x0000FF01;
        else if ( mask && iString && !iFunctions && !iVariables ) font = 0x0000FF00;
        else if ( mask && !iString && (iFunctions || iVariables) ) font = 0x00FF0000;
        else if (type == Start ) font = 0x00FF0000;
        else font = 0xFF000000;
        break;
    case Rail_Junk:
    case Rail_Line:
    case Rail_Slash:
    case Rail_Corss:
        if( mask ) font = 0;
        else font = 0xFF000000;
        break;
    case String_Symble:
        if( mask ) font = 0x0000FF00;
        else font = 0xFF000000;
        break;
    case Variable_Symbol:
        if( mask ) font = 0x00FF0000;
        else font = 0xFF000000;
        break;
    case Funktion_Symble:
        if( mask ) font = 0x00FF0000;
        else font = 0xFF000000;
        break;
    default:
        if( mask && iString && (iFunctions || iVariables || (legal_Start & ~iString))) font = 0x0000FF01;
        else if( mask && iString) font = 0x0000FF00;
        else if( mask && iFunctions) font = 0x00FF0000;
        else if( mask && iVariables) font = 0x00FF0000;
        else font = 0xFF000000;
        break;
    }
    return font;
}
void Point::make_Node_Changes(InternStack *changes){
    if(type == Shadow) make_Node_Type();
    mask = make_Node_Connection_Mask();
    switch (type) {
    case Start:
        if(b_Cons_to_Nodes & 128)cons_to_Nodes[0]->change_Connections(128 & mask ,iString & 1 && 128 & mask & ~iString,iFunctions & 1 && 128 & mask & ~iFunctions,iVariables & 1 && 128 &  mask & ~iVariables, 1, changes);
        if(b_Cons_to_Nodes & 64)cons_to_Nodes[1]->change_Connections(64 & mask ,iString & 2 && 64 & mask & ~iString,iFunctions & 2 && 64 & mask & ~iFunctions,iVariables & 2 && 64 &  mask & ~iVariables, 2, changes);
        if(b_Cons_to_Nodes & 32)cons_to_Nodes[2]->change_Connections(32 & mask ,iString & 4 && 32 & mask & ~iString,iFunctions & 4 && 32 & mask & ~iFunctions,iVariables & 4 && 32 &  mask & ~iVariables, 4, changes);
        if(b_Cons_to_Nodes & 16)cons_to_Nodes[3]->change_Connections(16 & mask ,iString & 8 && 16 & mask & ~iString,iFunctions & 8 && 16 & mask & ~iFunctions,iVariables & 8 && 16 &  mask & ~iVariables, 8, changes);
        if(b_Cons_to_Nodes & 8)cons_to_Nodes[4]->change_Connections(8 & mask ,iString & 16 && 8 & mask & ~iString,iFunctions & 16 && 8 & mask & ~iFunctions,iVariables & 16 && 8 &  mask & ~iVariables, 16, changes);
        if(b_Cons_to_Nodes & 4)cons_to_Nodes[5]->change_Connections(4 & mask ,iString & 32 && 4 & mask & ~iString,iFunctions & 32 && 4 & mask & ~iFunctions,iVariables & 32 && 4 &  mask & ~iVariables, 32, changes);
        if(b_Cons_to_Nodes & 2)cons_to_Nodes[6]->change_Connections(2 & mask ,iString & 64 && 2 & mask & ~iString,iFunctions & 64 && 2 & mask & ~iFunctions,iVariables & 64 && 2 &  mask & ~iVariables, 64, changes);
        if(b_Cons_to_Nodes & 1)cons_to_Nodes[7]->change_Connections(true,iString & 128 && 1 & mask & ~iString,iFunctions & 128 && 1 & mask & ~iFunctions,iVariables & 128 && 1 &  mask & ~iVariables, 128, changes);
        break;
    case Lambda:
        if(b_Cons_to_Nodes & 128)cons_to_Nodes[0]->change_Connections(128 & mask & ~iString ,iString & 1 && 128 & mask & ~iString,iFunctions & 1 && 128 & mask & ~iFunctions,iVariables & 1 && 128 &  mask & ~iVariables, 1, changes);
        if(b_Cons_to_Nodes & 64)cons_to_Nodes[1]->change_Connections(64 & mask & ~iString ,iString & 2 && 64 & mask & ~iString,iFunctions & 2 && 64 & mask & ~iFunctions,iVariables & 2 && 64 &  mask & ~iVariables, 2, changes);
        if(b_Cons_to_Nodes & 32)cons_to_Nodes[2]->change_Connections(32 & mask & ~iString ,iString & 4 && 32 & mask & ~iString,iFunctions & 4 && 32 & mask & ~iFunctions,iVariables & 4 && 32 &  mask & ~iVariables, 4, changes);
        if(b_Cons_to_Nodes & 16)cons_to_Nodes[3]->change_Connections(16 & mask & ~iString ,iString & 8 && 16 & mask & ~iString,iFunctions & 8 && 16 & mask & ~iFunctions,iVariables & 8 && 16 &  mask & ~iVariables, 8, changes);
        if(b_Cons_to_Nodes & 8)cons_to_Nodes[4]->change_Connections(8 & mask & ~iString ,iString & 16 && 8 & mask & ~iString,iFunctions & 16 && 8 & mask & ~iFunctions,iVariables & 16 && 8 &  mask & ~iVariables, 16, changes);
        if(b_Cons_to_Nodes & 4)cons_to_Nodes[5]->change_Connections(4 & mask & ~iString ,iString & 32 && 4 & mask & ~iString,iFunctions & 32 && 4 & mask & ~iFunctions,iVariables & 32 && 4 &  mask & ~iVariables, 32, changes);
        if(b_Cons_to_Nodes & 2)cons_to_Nodes[6]->change_Connections(2 & mask & ~iString ,iString & 64 && 2 & mask & ~iString,iFunctions & 64 && 2 & mask & ~iFunctions,iVariables & 64 && 2 &  mask & ~iVariables, 64, changes);
        if(b_Cons_to_Nodes & 1)cons_to_Nodes[7]->change_Connections(1 & mask & ~iString,iString & 128 && 1 & mask & ~iString,iFunctions & 128 && 1 & mask & ~iFunctions,iVariables & 128 && 1 &  mask & ~iVariables, 128, changes);
        break;
    case Reflector:
        if(b_Cons_to_Nodes & 128)cons_to_Nodes[0]->change_Connections(128 & mask & ~iString ,iString & 1 && 128 & mask & ~iString,iFunctions & 1 && 128 & mask & ~iFunctions,iVariables & 1 && 128 &  mask & ~iVariables, 1, changes);
        if(b_Cons_to_Nodes & 64)cons_to_Nodes[1]->change_Connections(64 & mask & ~iString ,iString & 2 && 64 & mask & ~iString,iFunctions & 2 && 64 & mask & ~iFunctions,iVariables & 2 && 64 &  mask & ~iVariables, 2, changes);
        if(b_Cons_to_Nodes & 32)cons_to_Nodes[2]->change_Connections(32 & mask & ~iString ,iString & 4 && 32 & mask & ~iString,iFunctions & 4 && 32 & mask & ~iFunctions,iVariables & 4 && 32 &  mask & ~iVariables, 4, changes);
        if(b_Cons_to_Nodes & 16)cons_to_Nodes[3]->change_Connections(16 & mask & ~iString ,iString & 8 && 16 & mask & ~iString,iFunctions & 8 && 16 & mask & ~iFunctions,iVariables & 8 && 16 &  mask & ~iVariables, 8, changes);
        if(b_Cons_to_Nodes & 8)cons_to_Nodes[4]->change_Connections(8 & mask & ~iString ,iString & 16 && 8 & mask & ~iString,iFunctions & 16 && 8 & mask & ~iFunctions,iVariables & 16 && 8 &  mask & ~iVariables, 16, changes);
        if(b_Cons_to_Nodes & 4)cons_to_Nodes[5]->change_Connections(4 & mask & ~iString ,iString & 32 && 4 & mask & ~iString,iFunctions & 32 && 4 & mask & ~iFunctions,iVariables & 32 && 4 &  mask & ~iVariables, 32, changes);
        if(b_Cons_to_Nodes & 2)cons_to_Nodes[6]->change_Connections(2 & mask & ~iString ,iString & 64 && 2 & mask & ~iString,iFunctions & 64 && 2 & mask & ~iFunctions,iVariables & 64 && 2 &  mask & ~iVariables, 64, changes);
        if(b_Cons_to_Nodes & 1)cons_to_Nodes[7]->change_Connections(1 & mask & ~iString,iString & 128 && 1 & mask & ~iString,iFunctions & 128 && 1 & mask & ~iFunctions,iVariables & 128 && 1 &  mask & ~iVariables, 128, changes);
        break;
    case String_Symble:
        if(b_Cons_to_Nodes & 128)cons_to_Nodes[0]->change_Connections(128 & mask,128 & mask && !(1 & iStart & iString),iFunctions & 1 && 128 & mask & ~iFunctions,iVariables & 1 && 128 &  mask & ~iVariables, 1, changes);
        if(b_Cons_to_Nodes & 64)cons_to_Nodes[1]->change_Connections(64 & mask,64 & mask && !(2 & iStart & iString),iFunctions & 2 && 64 & mask & ~iFunctions,iVariables & 2 && 64 &  mask & ~iVariables, 2, changes);
        if(b_Cons_to_Nodes & 32)cons_to_Nodes[2]->change_Connections(32 & mask,32 & mask && !(4 & iStart & iString),iFunctions & 4 && 32 & mask & ~iFunctions,iVariables & 4 && 32 &  mask & ~iVariables, 4, changes);
        if(b_Cons_to_Nodes & 16)cons_to_Nodes[3]->change_Connections(16 & mask,16 & mask && !(8 & iStart & iString),iFunctions & 8 && 16 & mask & ~iFunctions,iVariables & 8 && 16 &  mask & ~iVariables, 8, changes);
        if(b_Cons_to_Nodes & 8)cons_to_Nodes[4]->change_Connections(8 & mask,8 & mask && !(16 & iStart & iString),iFunctions & 16 && 8 & mask & ~iFunctions,iVariables & 16 && 8 &  mask & ~iVariables, 16, changes);
        if(b_Cons_to_Nodes & 4)cons_to_Nodes[5]->change_Connections(4 & mask,4 & mask && !(32 & iStart & iString),iFunctions & 32 && 4 & mask & ~iFunctions,iVariables & 32 && 4 &  mask & ~iVariables, 32, changes);
        if(b_Cons_to_Nodes & 2)cons_to_Nodes[6]->change_Connections(2 & mask,2 & mask && !(64 & iStart & iString),iFunctions & 64 && 2 & mask & ~iFunctions,iVariables & 64 && 2 &  mask & ~iVariables, 64, changes);
        if(b_Cons_to_Nodes & 1)cons_to_Nodes[7]->change_Connections(1 & mask,1 & mask && !(128 & iStart & iString),iFunctions & 128 && 1 & mask & ~iFunctions,iVariables & 128 && 1 &  mask & ~iVariables, 128, changes);
        break;
    case Funktion_Symble:
        if(b_Cons_to_Nodes & 128)cons_to_Nodes[0]->change_Connections(128 & mask ,iString & 1 && 128 & mask & ~iString,128 & mask && !(1 & iStart & iFunctions),iVariables & 1 && 128 &  mask & ~iVariables, 1, changes);
        if(b_Cons_to_Nodes & 64)cons_to_Nodes[1]->change_Connections(64 & mask ,iString & 2 && 64 & mask & ~iString,64 & mask && !(2 & iStart & iFunctions),iVariables & 2 && 64 &  mask & ~iVariables, 2, changes);
        if(b_Cons_to_Nodes & 32)cons_to_Nodes[2]->change_Connections(32 & mask ,iString & 4 && 32 & mask & ~iString,32 & mask && !(4 & iStart & iFunctions),iVariables & 4 && 32 &  mask & ~iVariables, 4, changes);
        if(b_Cons_to_Nodes & 16)cons_to_Nodes[3]->change_Connections(16 & mask ,iString & 8 && 16 & mask & ~iString,16 & mask && !(8 & iStart & iFunctions),iVariables & 8 && 16 &  mask & ~iVariables, 8, changes);
        if(b_Cons_to_Nodes & 8)cons_to_Nodes[4]->change_Connections(8 & mask ,iString & 16 && 8 & mask & ~iString,8 & mask && !(16 & iStart & iFunctions),iVariables & 16 && 8 &  mask & ~iVariables, 16, changes);
        if(b_Cons_to_Nodes & 4)cons_to_Nodes[5]->change_Connections(4 & mask ,iString & 32 && 4 & mask & ~iString,4 & mask && !(32 & iStart & iFunctions),iVariables & 32 && 4 &  mask & ~iVariables, 32, changes);
        if(b_Cons_to_Nodes & 2)cons_to_Nodes[6]->change_Connections(2 & mask ,iString & 64 && 2 & mask & ~iString,2 & mask && !(64 & iStart & iFunctions),iVariables & 64 && 2 &  mask & ~iVariables, 64, changes);
        if(b_Cons_to_Nodes & 1)cons_to_Nodes[7]->change_Connections(1 & mask ,iString & 128 && 1 & mask & ~iString,1 & mask && !(128 & iStart & iFunctions),iVariables & 128 && 1 &  mask & ~iVariables, 128, changes);
        break;
    case Variable_Symbol:
        if(b_Cons_to_Nodes & 128)cons_to_Nodes[0]->change_Connections(128 & mask ,iString & 1 && 128 & mask & ~iString,iFunctions & 1 && 128 & mask & ~iFunctions,128 & mask && !(1 & iStart & iVariables), 1, changes);
        if(b_Cons_to_Nodes & 64)cons_to_Nodes[1]->change_Connections(64 & mask ,iString & 2 && 64 & mask & ~iString,iFunctions & 2 && 64 & mask & ~iFunctions,64 & mask && !(2 & iStart & iVariables), 2, changes);
        if(b_Cons_to_Nodes & 32)cons_to_Nodes[2]->change_Connections(32 & mask ,iString & 4 && 32 & mask & ~iString,iFunctions & 4 && 32 & mask & ~iFunctions,32 & mask && !(4 & iStart & iVariables), 4, changes);
        if(b_Cons_to_Nodes & 16)cons_to_Nodes[3]->change_Connections(16 & mask ,iString & 8 && 16 & mask & ~iString,iFunctions & 8 && 16 & mask & ~iFunctions,16 & mask && !(8 & iStart & iVariables), 8, changes);
        if(b_Cons_to_Nodes & 8)cons_to_Nodes[4]->change_Connections(8 & mask ,iString & 16 && 8 & mask & ~iString,iFunctions & 16 && 8 & mask & ~iFunctions,8 & mask && !(16 & iStart & iVariables), 16, changes);
        if(b_Cons_to_Nodes & 4)cons_to_Nodes[5]->change_Connections(4 & mask ,iString & 32 && 4 & mask & ~iString,iFunctions & 32 && 4 & mask & ~iFunctions,4 & mask && !(32 & iStart & iVariables), 32, changes);
        if(b_Cons_to_Nodes & 2)cons_to_Nodes[6]->change_Connections(2 & mask ,iString & 64 && 2 & mask & ~iString,iFunctions & 64 && 2 & mask & ~iFunctions,2 & mask && !(64 & iStart & iVariables), 64, changes);
        if(b_Cons_to_Nodes & 1)cons_to_Nodes[7]->change_Connections(1 & mask ,iString & 128 && 1 & mask & ~iString,iFunctions & 128 && 1 & mask & ~iFunctions,1 & mask && !(128 & iStart & iVariables), 128, changes);
        break;
    default:
        if(b_Cons_to_Nodes & 128)cons_to_Nodes[0]->change_Connections(128 & mask ,iString & 1 && 128 & mask & ~iString,iFunctions & 1 && 128 & mask & ~iFunctions,iVariables & 1 && 128 &  mask & ~iVariables, 1, changes);
        if(b_Cons_to_Nodes & 64)cons_to_Nodes[1]->change_Connections(64 & mask ,iString & 2 && 64 & mask & ~iString,iFunctions & 2 && 64 & mask & ~iFunctions,iVariables & 2 && 64 &  mask & ~iVariables, 2, changes);
        if(b_Cons_to_Nodes & 32)cons_to_Nodes[2]->change_Connections(32 & mask ,iString & 4 && 32 & mask & ~iString,iFunctions & 4 && 32 & mask & ~iFunctions,iVariables & 4 && 32 &  mask & ~iVariables, 4, changes);
        if(b_Cons_to_Nodes & 16)cons_to_Nodes[3]->change_Connections(16 & mask ,iString & 8 && 16 & mask & ~iString,iFunctions & 8 && 16 & mask & ~iFunctions,iVariables & 8 && 16 &  mask & ~iVariables, 8, changes);
        if(b_Cons_to_Nodes & 8)cons_to_Nodes[4]->change_Connections(8 & mask ,iString & 16 && 8 & mask & ~iString,iFunctions & 16 && 8 & mask & ~iFunctions,iVariables & 16 && 8 &  mask & ~iVariables, 16, changes);
        if(b_Cons_to_Nodes & 4)cons_to_Nodes[5]->change_Connections(4 & mask ,iString & 32 && 4 & mask & ~iString,iFunctions & 32 && 4 & mask & ~iFunctions,iVariables & 32 && 4 &  mask & ~iVariables, 32, changes);
        if(b_Cons_to_Nodes & 2)cons_to_Nodes[6]->change_Connections(2 & mask ,iString & 64 && 2 & mask & ~iString,iFunctions & 64 && 2 & mask & ~iFunctions,iVariables & 64 && 2 &  mask & ~iVariables, 64, changes);
        if(b_Cons_to_Nodes & 1)cons_to_Nodes[7]->change_Connections(1 & mask ,iString & 128 && 1 & mask & ~iString,iFunctions & 128 && 1 & mask & ~iFunctions,iVariables & 128 && 1 &  mask & ~iVariables, 128, changes);
        break;
    }
}
void Point::change_Connections(bool A_R_Start, bool A_R_iString, bool A_R_iFunctions, bool A_R_iVariable, int direction, InternStack *change){
    int old = iStart; bool add = false;
    A_R_Start ? iStart |= direction : iStart &= ~direction;
    if(old != iStart) add = true;
    old = iString;
    A_R_iString ? iString |= direction : iString &= ~direction;
    if(old != iString) add = true;
    old = iFunctions;
    A_R_iFunctions ? iFunctions |= direction : iFunctions &= ~direction;
    if(old != iFunctions) add = true;
    old = iVariables;
    A_R_iVariable ? iVariables |= direction : iVariables &= ~direction;
    if(old != iVariables) add = true;
    if(add && type != Shadow)change->push(this);
}
void Point::make_Node_Type ( void ){
    switch (sign) {
    case '$':
        type = Start;
        break;
    case '#':
        type = End;
        break;
    case '&':
        type = Lambda;
        break;
    case '@':
        type = Reflector;
        break;
    case '-':
    case '|':
        type = Rail_Line;
        break;
    case '\\':
    case '/':
        type = Rail_Slash;
        break;
    case '+':
    case '*':
    case 'x':
        type = Rail_Corss;
        break;
    case '<':
    case '>':
    case 'v':
    case '^':
        type = Rail_Junk;
        break;
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '0':
    case 'b':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
    case '?':
    case 'a':
    case 'd':
    case 'm':
    case 'r':
    case 's':
    case 'n':
    case ':':
    case '~':
    case 'f':
    case 'g':
    case 'q':
    case 't':
    case 'c':
        type = Function;
        break;
    case '[':
    case ']':
        type = String_Symble;
        break;
    case '{':
    case '}':
        type = Funktion_Symble;
        break;
    case '(':
    case ')':
        type = Variable_Symbol;
        break;
    default:
        type = White_Space;
        break;
    }
}
int Point::make_Node_Connection_Mask( void ){
    int mask = 0;
    switch (type) {
    case Start:
        mask |= 1;
        break;
    case Rail_Slash:
        if(sign == '\\'){
            if(iStart & 128 || (iStart & 64 && cons_to_Nodes[1]->get_Node_Type() != Rail_Slash) || (iStart & 16 && cons_to_Nodes[3]->get_Node_Type() != Rail_Slash))  mask |= 1+2+8;
            if(iStart & 1 || (iStart & 2 && cons_to_Nodes[6]->get_Node_Type() != Rail_Slash) || (iStart & 8 && cons_to_Nodes[4]->get_Node_Type() != Rail_Slash))  mask |= 16+128+64;
        }else if(sign == '/'){
            if((iStart & 64 && cons_to_Nodes[1]->get_Node_Type() != Rail_Slash) || iStart & 32 || (iStart & 8 && cons_to_Nodes[4]->get_Node_Type() != Rail_Slash)) mask |= 16+4+2;
            if((iStart & 2 && cons_to_Nodes[6]->get_Node_Type() != Rail_Slash) || iStart & 4 || (iStart & 16 && cons_to_Nodes[3]->get_Node_Type() != Rail_Slash)) mask |= 64+32+8;
        }
        break;
    case Rail_Line:
        if(sign == '|'){
            if(iStart & (64) || (iStart & 128 && cons_to_Nodes[0]->get_Node_Type() != Rail_Line) || (iStart & 32 && cons_to_Nodes[2]->get_Node_Type() != Rail_Line)) mask |= 4+2+1;
            if(iStart & 2 || (iStart & 4 && cons_to_Nodes[5]->get_Node_Type() != Rail_Line) || (iStart & 1 && cons_to_Nodes[7]->get_Node_Type() != Rail_Line)) mask |= 128+64+32;
        }else if( sign == '-'){
            if(iStart & 16 || (iStart & 4 && cons_to_Nodes[5]->get_Node_Type() != Rail_Line) || (iStart & 128 && cons_to_Nodes[0]->get_Node_Type() != Rail_Line)) mask |= 32+8+1;
            if(iStart & 8 || (iStart & 32 && cons_to_Nodes[2]->get_Node_Type() != Rail_Line) || (iStart & 1 && cons_to_Nodes[7]->get_Node_Type() != Rail_Line)) mask |= 128+16+4;
        } break;
    case Rail_Junk:
        if(sign == '<'){
            if(iStart & 16) mask |= 32+1;
            if(iStart & 32) mask |= 16+1;
            if(iStart & 1) mask |= 16+32;
        }else if( sign == '>'){
            if(iStart & 8) mask |= 128+4;
            if(iStart & 128) mask |= 4+8;
            if(iStart & 4) mask |= 128+8;
        }
        else if(sign == '^'){
            if(iStart & 64) mask |= 4+1;
            if(iStart & 4) mask |= 64+1;
            if(iStart & 1) mask |= 64+4;
        }else if(sign == 'v'){
            if(iStart & 2) mask |= 128+32;
            if(iStart & 128) mask |= 2+32;
            if(iStart & 32) mask |= 128+2;
        }break;
    case  Rail_Corss:
        if(sign == '+'){
            if(iStart & 64) mask |= 2;
            if(iStart & 16) mask |= 8;
            if(iStart & 8) mask |= 16;
            if(iStart & 2) mask |= 64;
        }else if( sign == 'x'){
            if(iStart & 128) mask |= 1;
            if(iStart & 32) mask |= 4;
            if(iStart & 4) mask |= 32;
            if(iStart & 1) mask |= 128;
        }else if(sign == '*'){
            if(iStart & 64) mask |= 2;
            if(iStart & 16) mask |= 8;
            if(iStart & 8) mask |= 16;
            if(iStart & 2) mask |= 64;
            if(iStart & 128) mask |= 1;
            if(iStart & 32) mask |= 4;
            if(iStart & 4) mask |= 32;
            if(iStart & 1) mask |= 128;
        }break;
    case String_Symble:
    case Funktion_Symble:
    case Variable_Symbol:
    case Function:
        if(iStart & 128 && cons_to_Nodes[0]->get_Node_Type() != Rail_Line) mask |= 1;
        if(iStart & 64) mask |= 2;
        if(iStart & 32 && cons_to_Nodes[2]->get_Node_Type() != Rail_Line) mask |= 4;
        if(iStart & 16) mask |= 8;
        if(iStart & 8) mask |= 16;
        if(iStart & 4 && cons_to_Nodes[5]->get_Node_Type() != Rail_Line) mask |= 32;
        if(iStart & 2) mask |= 64;
        if(iStart & 1 && cons_to_Nodes[7]->get_Node_Type() != Rail_Line) mask |= 128;
        break;
    case Reflector:
        mask |= iStart;
        break;
    case Lambda:
        mask |= iStart;
        if(iStart & 128 && cons_to_Nodes[0]->get_Node_Type() != Rail_Line) mask |= 1;
        if(iStart & 64) mask |= 2;
        if(iStart & 32 && cons_to_Nodes[2]->get_Node_Type() != Rail_Line) mask |= 4;
        if(iStart & 16) mask |= 8;
        if(iStart & 8) mask |= 16;
        if(iStart & 4 && cons_to_Nodes[5]->get_Node_Type() != Rail_Line) mask |= 32;
        if(iStart & 2) mask |= 64;
        if(iStart & 1 && cons_to_Nodes[7]->get_Node_Type() != Rail_Line) mask |= 128;
        break;
    case White_Space:
    default:
        mask =0;
        break;
    }
    if((iFunctions | iVariables | iString) & 128) mask |= 1;
    if((iFunctions | iVariables | iString) & 64) mask |= 2;
    if((iFunctions | iVariables | iString) & 32) mask |= 4;
    if((iFunctions | iVariables | iString) & 16) mask |= 8;
    if((iFunctions | iVariables | iString) & 8) mask |= 16;
    if((iFunctions | iVariables | iString) & 4) mask |= 32;
    if((iFunctions | iVariables | iString) & 2) mask |= 64;
    if((iFunctions | iVariables | iString) & 1) mask |= 128;
    return mask;
}
int Point::get_Cons_to_Nodes ( void ){
    return b_Cons_to_Nodes;
}

Point::Node_Type Point::get_Node_Type(){
    return type;
}
void Point::setSign(char sign){
    this->sign = sign;
    make_Node_Type();
}
int Point::getRow(){
    return row;
}
int Point::getColm(){
    return colm;
}
char Point::getSign(){
    return sign;
}
int Point::get_Start_Direction(){
    return this->iStart;
}
int Point::get_I_Direction(){
    int direction = 0;
    int directions = 0;
    if(iStart & 128) directions++;
    if(iStart & 64) directions++;
    if(iStart & 32) directions++;
    if(iStart & 16) directions++;
    if(iStart & 8) directions++;
    if(iStart & 4) directions++;
    if(iStart & 2) directions++;
    if(iStart & 1) directions++;
    if(directions > 1) return 0;
    switch (sign) {
    case '$':
        direction = 1;
        break;
    case '-':
        if(iStart & (128+16+4))direction = 8;
        else if(iStart & (1+8+32))direction = 16;
        break;
    case '|':
        if(iStart & (1+2+4))direction = 64;
        else if(iStart & (32+64+128))direction = 2;
        break;
    case '<':
        if(iStart & 16)direction = 1;
        else if(iStart & 32)direction = 16;
        else if(iStart & 1)direction = 32;
        break;
    case '>':
        if(iStart & 128)direction = 4;
        else if(iStart & 8)direction = 128;
        else if(iStart & 4)direction = 8;
        break;
    case '^':
        if(iStart & 64)direction = 4;
        else if(iStart & 1)direction = 64;
        else if(iStart & 4)direction = 1;
        break;
    case 'v':
        if(iStart & 128)direction = 2;
        else if(iStart & 32)direction = 128;
        else if(iStart & 2)direction = 32;
        break;
    case '\\':
        if(iStart & (16+128+64))direction = 1;
        else if(iStart & (2+1+8))direction = 128;
        break;
    case '/':
        if(iStart & (16+4+2))direction = 32;
        else if(iStart & (64+32+8))direction = 4;
        break;
    default:
        if(iStart & 128)direction = 1;
        else if(iStart & 64)direction = 2;
        else if(iStart & 32)direction = 4;
        else if(iStart & 16)direction = 8;
        else if(iStart & 8)direction = 16;
        else if(iStart & 4)direction = 32;
        else if(iStart & 2)direction = 64;
        else if(iStart & 1)direction = 128;
        break;
    }
    return direction;
}
