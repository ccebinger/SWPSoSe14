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

    UndoRedoTypeCharacter(newRow, newColumn, newBitMask) {
        row = newRow;
        column = newColumn;
        bitMask = newBitMask;
    }
};

#endif // UNDOREDOTYPECHARACTER_H
