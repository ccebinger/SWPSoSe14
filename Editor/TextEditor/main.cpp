#include <stddef.h>
#include <QApplication>
#include "mainwindow.h"
#include "InterStack.h"
#include "Stack.h"
#include "Point.h"
#include "Graph_Interface.h"
#include "UndoRedoStack.h"
#include "UndoRedoElement.h"
#include "UndoRedoTypeCharacter.h"

//remove this later
#include "QDebug"

int main(int argc, char *argv[])
{
    //only for testing the text selection!

    QList<QChar>* list = new QList<QChar>();
    list->append('$');
    list->append(' ');
    list->append('[');
    list->append('-');
    list->append('<');
    list->append(' ');
    list->append('}');
    list->append(' ');
    list->append(']');
    TextSelection* t = new TextSelection(*list,3,3);
    t = &t->mirrorX();
    for(int y=0; y<3; y++) {
        for(int x=0; x<3; x++) {
            qDebug() << t->text().at(y*3+x);
        }
    }

    std::exit(0);

    //end of testing the text selection

    QApplication app(argc, argv);
    app.setOrganizationName("Jail Constructions Ltd.");
    app.setOrganizationDomain("https://github.com/ccebinger/SWPSoSe14");
    app.setApplicationName("Jail++");

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
