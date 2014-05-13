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
    EditTableWidget.cpp

HEADERS  += mainwindow.h \
    EditTableWidget.h \
    Point.h \
    Graph_Interface.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
