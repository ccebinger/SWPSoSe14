#ifndef UNDOREDOCUTCOPYPASTE_H
#define UNDOREDOCUTCOPYPASTE_H

#include <qlist.h>
#include <qchar.h>
#include <qstring.h>
#include "TextSelection.h"

class UndoRedoCutPaste: public UndoRedoElement
{
private:
    TextSelection pre, post;
    int topRow;
    int leftColumn;

public:

    UndoRedoCutPaste(int topRow, int leftColumn, TextSelection pre, TextSelection post, QString displayPrefix)
    {
        this->topRow = topRow;
        this->leftColumn = leftColumn;
        this->pre = pre;
        this->post = post;

        QString topRowString = QString::number(topRow + 1);
        QString leftColumnString = QString::number(leftColumn + 1);
        QString bottomRowString = QString::number(topRow + pre.height());
        QString rightColumnString = QString::number(leftColumn + pre.width());

        displayName = displayPrefix + " at (" + topRowString + ", " + leftColumnString + ", " + bottomRowString + ", " + rightColumnString + ")";
    }

    int getTop() const
    {
        return topRow;
    }

    int getLeft() const
    {
        return leftColumn;
    }

    TextSelection getPre() const
    {
        return pre;
    }

    TextSelection getPost() const
    {
        return post;
    }
};

#endif // UNDOREDOCUTCOPYPASTE_H
