#ifndef UNDOREDOELEMENT_H
#define UNDOREDOELEMENT_H

class UndoRedoElement {
private:
    QString undoName;
    QString redoName;

public:
    UndoRedoElement(void) {
        undoName = "";
        redoName = "";
    }

    QString getUndoName() {
        return undoName;
    }

    QString getRedoName() {
        return redoName;
    }

    virtual undo() = 0;
    virtual redo() = 0;
    virtual undoDisplay() = 0;
    virtual redoDisplay() = 0;
};

#endif // UNDOREDOELEMENT_H
