#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ui_mainwindow.h>

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


private slots:
    void cursorPositionChanged(int x, int y);
    void textChanged();
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();

    void setInterpreter();
    void runInterpreter();
};

#endif // MAINWINDOW_H
