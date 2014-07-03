#ifndef UNDOREDOELEMENT_H
#define UNDOREDOELEMENT_H

#include <QString>

class UndoRedoElement
{
protected:
    QString displayName;

public:

    UndoRedoElement(void) {
        displayName = "";
    }

    virtual ~UndoRedoElement() {

    }

    QString getDisplayName() {
        return displayName;
    }
};

#endif // UNDOREDOELEMENT_H
