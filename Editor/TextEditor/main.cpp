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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Jail Constructions Ltd.");
    app.setOrganizationDomain("https://github.com/ccebinger/SWPSoSe14");
    app.setApplicationName("Jail++");

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
