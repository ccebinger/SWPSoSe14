#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ui_mainwindow.h>
#include <iostream>
#include <QProcess>

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
    void setModified(bool modified);
    void setCurrentPath(QString currentFilePath);
    void save(QString filePath);

    bool m_modified;
    QString m_currentFilePath;
    QString m_currentInterpreterPath;
    QString m_currentCompilerPath;

    QProcess *m_interpreterProcess;
    QProcess *m_compilerProcess;


private slots:
    void cursorPositionChanged(int row, int col);
    void textChanged();
    void pushSignToUndoStack();
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();

    void undo();
    void redo();
    void setInterpreter();
    void runInterpreter();
    void setCompiler();
    void runCompiler();

    void interpreterStarted();
    void interpreterFinished();
    void interpreterOutputReady();
    void interpreterReadError(QProcess::ProcessError error);

    void compilerStarted();
    void compilerFinished();
    void compilerOutputReady();
    void compilerReadError(QProcess::ProcessError error);

    /*void undo();
    void redo();
    void undoAvailable(bool available);
    void redoAvalable(bool available);*/
};

#endif // MAINWINDOW_H
