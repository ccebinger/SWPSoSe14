#ifndef UNDOREDOSTACK_H
#define UNDOREDOSTACK_H

#include "UndoRedoElement.h"
#include <QStack>
#include <QObject>

class UndoRedoStack: public QObject {
    Q_OBJECT

signals:
    void modified(bool isModified);
    void undoAvailable(bool undoAvailable);
    void redoAvailable(bool redoAvailable);

private:
    QStack<UndoRedoElement*> *internalUndoStack;
    QStack<UndoRedoElement*> *internalRedoStack;
    int savedAt;
    // savedAt < 0: last safed state is in undo-Stack
    // savedAt = 0: last safed state is now
    // savedAt > 0: last safed state is in redo-Stack

public:
    explicit UndoRedoStack(QObject *parent = 0) {
        internalUndoStack = new QStack<UndoRedoElement*>();
        internalRedoStack = new QStack<UndoRedoElement*>();
        savedAt = 0;
    }

    virtual ~UndoRedoStack() {}

    void setSaved() {
        savedAt = 0;
    }

    void clear()
    {
        savedAt = 0;
        while(!internalUndoStack->isEmpty()) {
            delete internalUndoStack->pop();
        }
        while(!internalRedoStack->isEmpty()) {
            delete internalRedoStack->pop();
        }
        emit undoAvailable(false);
        emit redoAvailable(false);
    }

    void createUndoElement(UndoRedoElement *e) {
        while(!internalRedoStack->isEmpty()) {
            delete internalRedoStack->pop();
        }
        // Don't know, if clear() calls the DTors of it's elements
        // Hence we delete them manually
        //internalRedoStack->clear();

        // No need for a validity check for the value of savedAt, cause it is <= 0, then it's fine
        // otherwise it will never reach 0, cause that's only possible with a redo-stack, which was just cleared
        savedAt--;
        internalUndoStack->push(e);
        emit undoAvailable(true);
        emit redoAvailable(false);
    }

    UndoRedoElement* undo() {
        UndoRedoElement* e = internalUndoStack->pop();
        internalRedoStack->push(e);
        savedAt++;
        if(savedAt == 0) {
            emit modified(false);
        } else if(savedAt == 1) {
            emit modified(true);
        }
        emit undoAvailable(!internalUndoStack->isEmpty());
        emit redoAvailable(true);
        return e;
    }

    UndoRedoElement* redo() {
        UndoRedoElement* e = internalRedoStack->pop();
        internalUndoStack->push(e);
        savedAt--;
        if(savedAt == 0) {
            emit modified(false);
        } else if(savedAt == -1) {
            emit modified(true);
        }
        emit undoAvailable(true);
        emit redoAvailable(!internalRedoStack->isEmpty());
        return e;
    }

    QString undoDisplay() {
        if(internalUndoStack->isEmpty()) return QString();
        return QString(internalUndoStack->top()->getRedoName());
    }

    QString redoDisplay() {
        if(internalRedoStack->isEmpty()) return QString();
        return QString(internalRedoStack->top()->getRedoName());
    }

    void clear() {
        while(!internalUndoStack->isEmpty()) {
            delete internalUndoStack->pop();
        }

        while(!internalRedoStack->isEmpty()) {
            delete internalRedoStack->pop();
        }
    }
};

#endif // EDITTABLEWIDGET_H
