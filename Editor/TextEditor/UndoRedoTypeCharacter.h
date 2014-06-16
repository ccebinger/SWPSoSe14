#ifndef UNDOREDOTYPECHARACTER_H
#define UNDOREDOTYPECHARACTER_H

class UndoRedoTypeCharacter: public UndoRedoElement {
private:
    int row;
    int column;
    QChar pre;
    QChar post;

public:

    UndoRedoTypeCharacter(int newRow, int newColumn, QChar pre, QChar post) {
        row = newRow;
        column = newColumn;
        this->pre = pre;
        this->post = post;
        undoDisplayName = "Overwrite '" + QString(post) + "' with '" + QString(pre) + "'";
        redoDisplayName = "Overwrite '" + QString(pre) + "' with '" + QString(post) + "'";
    }

    int getRow() const {
        return row;
    }

    int getColumn() const {
        return column;
    }

    QChar getPre() const {
        return pre;
    }

    QChar getPost() const {
        return post;
    }
};

#endif // UNDOREDOTYPECHARACTER_H
