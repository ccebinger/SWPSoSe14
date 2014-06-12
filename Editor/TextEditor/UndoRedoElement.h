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

    virtual void undo() = 0;
    virtual void redo() = 0;
    virtual void undoDisplay() = 0;
    virtual void redoDisplay() = 0;
};

#endif // UNDOREDOELEMENT_H
