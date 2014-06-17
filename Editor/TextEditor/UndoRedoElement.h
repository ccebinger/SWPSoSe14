#ifndef UNDOREDOELEMENT_H
#define UNDOREDOELEMENT_H

#include <QString>

class UndoRedoElement {
protected:
    QString undoDisplayName;
    QString redoDisplayName;

public:

    UndoRedoElement(void) {
        undoDisplayName = "";
        redoDisplayName = "";
    }

    virtual ~UndoRedoElement() {

    }

    QString getUndoName() {
        return undoDisplayName;
    }

    QString getRedoName() {
        return redoDisplayName;
    }
};

#endif // UNDOREDOELEMENT_H
