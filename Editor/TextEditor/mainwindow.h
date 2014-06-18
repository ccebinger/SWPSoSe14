#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ui_mainwindow.h>
#include <iostream>
#include <QProcess>

#include "UndoRedoStack.h"
#include "UndoRedoElement.h"
#include "UndoRedoTypeCharacter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;

    bool saveChanges();
    void closeEvent(QCloseEvent *closeEvent);
    void setCurrentPath(QString currentFilePath);
    bool save(QString filePath);
    void createTempFiles();

    bool m_modified;
    QString m_currentFilePath;
    QString m_currentInterpreterPath;
    QString m_currentFrontendPath;

    QProcess *m_interpreterProcess;
    QProcess *m_frontendProcess;

    UndoRedoStack *m_undoRedoStack;

    int m_tempFilesIndex;

private slots:

    void cursorPositionChanged(int row, int col);
    void textChanged();
    void undoRedoElementCreated(UndoRedoElement *e);
    void undoAvailable(bool undoAvailable);
    void redoAvailable(bool redoAvailable);
    void newFile();
    void openFile();
    bool saveFile();
    bool saveFileAs();
    void setModified(bool modified);

    void undo();
    void redo();
    void setInterpreter();
    void runInterpreter();
    void setFrontend();
    void runFrontend();

    void interpreterStarted();
    void interpreterFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void interpreterOutputReady();
    void interpreterErrorReady();

    void frontendStarted();
    void frontendFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void frontendOutputReady();
    void frontendErrorReady();
    void frontendProcessError(QProcess::ProcessError error);
};

#endif // MAINWINDOW_H
