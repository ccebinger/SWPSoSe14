#ifndef UNDOREDOTYPECHARACTER_H
#define UNDOREDOTYPECHARACTER_H

class UndoRedoTypeCharacter: public UndoRedoElement {
private:
    int row;
    int column;
    int bitMask;

public:
    UndoRedoTypeCharacter(void) {
        row = 0;
        column = 0;
        bitMask = 0;
    }

    UndoRedoTypeCharacter(int newRow, int newColumn, int newBitMask) {
        row = newRow;
        column = newColumn;
        bitMask = newBitMask;
    }

    void undo() {

    }

    void redo() {

    }

    void undoDisplay() {

    }

    void redoDisplay() {

    }
};

#endif // UNDOREDOTYPECHARACTER_H
