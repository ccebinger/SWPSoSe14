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
#include <QDebug>
#include <QSplashScreen>
#include <QTimer>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Jail Constructions Ltd.");
    app.setOrganizationDomain("https://github.com/ccebinger/SWPSoSe14");
    app.setApplicationName("Jail++");


    QPixmap pixmap(":/icons/Logos/project_logo.jpg");
    QSplashScreen splash (pixmap);
    splash.show();
    QEventLoop loop;
    QTimer::singleShot(1500, &loop, SLOT(quit()));
    loop.exec();

    /*splash.showMessage("Getting construction workers", Qt::AlignCenter, QColor("#ffaa00"));
    qApp->processEvents();
    QTimer::singleShot(1000, &loop, SLOT(quit()));
    loop.exec();

    splash.showMessage("Building Rails", Qt::AlignCenter, QColor("#ffaa00"));
    qApp->processEvents();
    QTimer::singleShot(1000, &loop, SLOT(quit()));
    loop.exec();

    splash.showMessage("Putting Train on Rails", Qt::AlignCenter, QColor("#ffaa00"));
    qApp->processEvents();
    QTimer::singleShot(1000, &loop, SLOT(quit()));
    loop.exec();*/

    MainWindow mainWindow;
    mainWindow.show();
    splash.finish(&mainWindow);

    return app.exec();
}
