#-------------------------------------------------
#
# Project created by QtCreator 2014-04-21T21:20:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TextEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    EditTableWidget.cpp \
    Point_Old.cpp \
    Point.cpp \
    Graph_Interface.cpp \
    ApplicationConsole.cpp \
    ApplicationPreferencesDialog.cpp \
    FileSelectLineEdit.cpp \
    ApplicationPreferences.cpp \
    TextSelection.cpp

HEADERS  += mainwindow.h \
    EditTableWidget.h \
    Stack.h \
    InterStack.h \
    UndoRedoTypeCharacter.h \
    UndoRedoElement.h \
    UndoRedoStack.h \
    Graph_Interface_old.h \
    Point_old.h \
    Graph_Interface.h \
    Point.h \
    UndoRedoCutPaste.h \
    ApplicationConsole.h \
    ApplicationPreferencesDialog.h \
    FileSelectLineEdit.h \
    ApplicationPreferences.h \
    TextSelection.h

FORMS    += mainwindow.ui \
    ApplicationPreferencesDialog.ui

RESOURCES += \
    resources.qrc
