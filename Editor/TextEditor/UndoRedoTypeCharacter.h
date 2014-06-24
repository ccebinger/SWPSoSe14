#ifndef UNDOREDOTYPECHARACTER_H
#define UNDOREDOTYPECHARACTER_H

class UndoRedoTypeCharacter: public UndoRedoElement {
private:
    int row;
    int column;
    QChar pre;
    QChar post;

public:

    UndoRedoTypeCharacter(int newRow, int newColumn, QChar newPre, QChar newPost) {
        row = newRow;
        column = newColumn;
        pre = newPre;
        post = newPost;

        QString preString = pre.isNull() ? QString() : QString(pre);
        QString postString = post.isNull() ? QString() : QString(post);
        displayName = "Overwrite '" + preString + "' with '" + postString + "' at (" + QString::number(row+1) + ", " + QString::number(column+1) + ")";
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
