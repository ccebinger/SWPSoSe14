#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ui_mainwindow.h>
#include <iostream>
#include <QProcess>
#include <QSettings>

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
    void readSettings();
    void writeSettings() const;
    void updateRecentFiles();
    void openFile(QString const& filePath);
    void setCursorMode();

    bool m_modified;
    QString m_currentFilePath;

    QProcess *m_interpreterProcess;
    QProcess *m_buildProcess;
    QProcess *m_javaProcess;

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
    void openRecent();
    bool saveFile();
    bool saveFileAs();
    void setModified(bool modified);

    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void setInterpreter();
    void runInterpreter();
    void setBuild();
    bool runBuild();
    void buildAndRun();
    void runJava();
    void showApplicationPreferences();

    void interpreterStarted();
    void interpreterFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void interpreterOutputReady();
    void interpreterErrorReady();

    void buildStarted();
    void buildFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void buildOutputReady();
    void buildErrorReady();

    void javaStarted();
    void javaFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void javaOutputReady();
    void javaErrorReady();

    void consoleLineEntered(QString line);

    void issueDoubleClicked(QListWidgetItem *item);

    void startGrab();
    void modifyGrab();
    void cancelGrab();
    void finishGrab();
    void grabModeChanged(bool inGrab);

    void cursorModeChanged(bool state);
};

#endif // MAINWINDOW_H
