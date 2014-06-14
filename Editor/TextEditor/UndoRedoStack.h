#ifndef UNDOREDOSTACK_H
#define UNDOREDOSTACK_H

#include "UndoRedoElement.h"
#include <QStack>

class UndoRedoStack {
private:
    QStack<UndoRedoElement*> *internalUndoStack;
    QStack<UndoRedoElement*> *internalRedoStack;

public:
    UndoRedoStack(void) {
        internalUndoStack = new QStack<UndoRedoElement*>();
        internalRedoStack = new QStack<UndoRedoElement*>();
    }

    void pushToUndoStack(UndoRedoElement* e) {
        internalUndoStack->push(e);
    }

    void pushToRedoStack(UndoRedoElement *e) {
        internalRedoStack->push(e);
    }

    UndoRedoElement* popFromUndoStack(void) {
        internalUndoStack->pop();
    }

    UndoRedoElement* popFromRedoStack(void) {
        internalRedoStack->pop();
    }

/*
    virtual void undo() = 0;
    virtual void redo() = 0;
    virtual void undoDisplay() = 0;
    virtual void redoDisplay() = 0;*/
};

#endif // EDITTABLEWIDGET_H
