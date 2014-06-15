#ifndef UNDOREDOSTACK_H
#define UNDOREDOSTACK_H

#include "UndoRedoElement.h"
#include <QStack>
#include <QObject>

class UndoRedoStack: public QObject {
    Q_OBJECT

signals:
    void modified(bool isModified);

private:
    QStack<UndoRedoElement*> *internalUndoStack;
    QStack<UndoRedoElement*> *internalRedoStack;
    int savedAt;

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

    void createUndoElement(UndoRedoElement *e) {
        while(!internalRedoStack->isEmpty()) {
            delete internalRedoStack->pop();
        }
        // Don't know, if clear() calls the DTors of it's elements
        // Hence we delete them manually
        //internalRedoStack->clear();
        internalUndoStack->push(e);
    }

    UndoRedoElement* undo() {
        UndoRedoElement* e = internalUndoStack->pop();
        internalRedoStack->push(e);
        savedAt++;
        if(savedAt == 0) {
            emit modified(true);
        } else if(savedAt == 1) {
            emit modified(false);
        }
        return e;
    }

    UndoRedoElement* redo() {
        UndoRedoElement* e = internalRedoStack->pop();
        internalUndoStack->push(e);
        savedAt--;
        if(savedAt == 0) {
            emit modified(true);
        } else if(savedAt == 1) {
            emit modified(false);
        }
        return e;
    }

    QString undoDisplay() {
        return QString(internalUndoStack->top()->getRedoName());
    }

    QString redoDisplay() {
        return QString(internalRedoStack->top()->getRedoName());
    }
};

#endif // EDITTABLEWIDGET_H
