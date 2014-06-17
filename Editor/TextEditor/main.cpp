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
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
