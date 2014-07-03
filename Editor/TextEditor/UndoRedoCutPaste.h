#ifndef UNDOREDOCUTCOPYPASTE_H
#define UNDOREDOCUTCOPYPASTE_H

#include <qlist.h>
#include <qchar.h>
#include <qstring.h>

class UndoRedoCutPaste: public UndoRedoElement
{
private:
    int topRow;
    int leftColumn;
    int bottomRow;
    int rightColumn;
    QList<QChar> pre;
    QList<QChar> post;

public:

    UndoRedoCutPaste(int topRow, int leftColumn, int bottomRow, int rightColumn, QList<QChar> pre, QList<QChar> post, QString displayPrefix)
    {
        this->topRow = topRow;
        this->leftColumn = leftColumn;
        this->bottomRow = bottomRow;
        this->rightColumn = rightColumn;
        this->pre = pre;
        this->post = post;

        displayName = displayPrefix + " at (" + QString::number(topRow+1) + ", " + QString::number(leftColumn+1) + ", " + QString::number(bottomRow+1) + ", " + QString::number(rightColumn+1) + ")";
    }

    int getTop() const
    {
        return topRow;
    }

    int getLeft() const
    {
        return leftColumn;
    }

    int getBottom() const
    {
        return bottomRow;
    }

    int getRight() const
    {
        return rightColumn;
    }

    QList<QChar> getPre() const
    {
        return pre;
    }

    QList<QChar> getPost() const
    {
        return post;
    }
};

#endif // UNDOREDOCUTCOPYPASTE_H
