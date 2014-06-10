#ifndef UNDOREDOSTACK_H
#define UNDOREDOSTACK_H

class UndoRedoStack {
private:
    QStack internalUndoStack;
    QStack internalRedoStack;

public:
    UndoRedoStack(void) {
        internalUndoStack = new QStack;
        internalRedoStack = new QStack;
    }

    virtual undo() = 0;
    virtual redo() = 0;
    virtual undoDisplay() = 0;
    virtual redoDisplay() = 0;
};

#endif // EDITTABLEWIDGET_H
