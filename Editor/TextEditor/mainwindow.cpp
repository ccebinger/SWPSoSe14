#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qevent.h>
#include <QFileInfo>
#include <qfiledialog.h>
#include <qaction.h>
#include <qtextstream.h>
#include <qfont.h>
#include <qprocess.h>
#include <QDebug>

#include "UndoRedoStack.h"
#include "UndoRedoElement.h"
#include "UndoRedoTypeCharacter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->ui_insertModeGroupBox->hide();
    ui->ui_writeDirectionGroupBox->hide();

    QFont f("unexistent");
    f.setStyleHint(QFont::Monospace);
    ui->ui_inputPlainTextEdit->setFont(f);
    ui->ui_outputPlainTextEdit->setFont(f);

    setCurrentPath(QString());
    setModified(false);

    m_interpreterProcess = new QProcess(this);
    m_frontendProcess = new QProcess(this);

    ui->ui_windowMenu->addAction(ui->ui_ioDockWidget->toggleViewAction());
    ui->ui_windowMenu->addAction(ui->ui_compilerDockWidget->toggleViewAction());

    m_undoRedoStack = new UndoRedoStack();
    connect(m_undoRedoStack, SIGNAL(modified(bool)), this, SLOT(setModified(bool)));

    ui->ui_stopInterpreterAction->setEnabled(false);
    ui->ui_stopFrontendAction->setEnabled(false);

    connect(ui->ui_sourceEditTableWidget, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(cursorPositionChanged(int,int)));
    connect(ui->ui_sourceEditTableWidget, SIGNAL(textChanged()), this, SLOT(textChanged()));
    connect(ui->ui_newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(ui->ui_openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->ui_saveFileAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->ui_saveAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));
    connect(ui->ui_quitAction, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->ui_undoAction, SIGNAL(triggered()), this, SLOT(undo()));
    connect(ui->ui_redoAction, SIGNAL(triggered()), this, SLOT(redo()));

    connect(ui->ui_setInterpreterAction, SIGNAL(triggered()), this, SLOT(setInterpreter()));
    connect(ui->ui_runInterpreterAction, SIGNAL(triggered()), this, SLOT(runInterpreter()));
    connect(ui->ui_stopInterpreterAction, SIGNAL(triggered()), m_interpreterProcess, SLOT(kill()));

    connect(ui->ui_setFrontendAction, SIGNAL(triggered()), this, SLOT(setFrontend()));
    connect(ui->ui_runFrontendAction, SIGNAL(triggered()), this, SLOT(runFrontend()));
    connect(ui->ui_stopFrontendAction, SIGNAL(triggered()), m_frontendProcess, SLOT(kill()));

    connect(m_interpreterProcess, SIGNAL(started()), this, SLOT(interpreterStarted()));
    connect(m_interpreterProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(interpreterOutputReady()));
    connect(m_interpreterProcess, SIGNAL(readyReadStandardError()), this, SLOT(interpreterErrorReady()));
    connect(m_interpreterProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(interpreterFinished(int,QProcess::ExitStatus)));

    connect(m_frontendProcess, SIGNAL(started()), this, SLOT(frontendStarted()));
    connect(m_frontendProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(frontendOutputReady()));
    connect(m_frontendProcess, SIGNAL(readyReadStandardError()), this, SLOT(frontendErrorReady()));
    connect(m_frontendProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(frontendFinished(int,QProcess::ExitStatus)));
}

MainWindow::~MainWindow()
{
    delete ui;

    m_interpreterProcess->close();
    delete m_interpreterProcess;
    m_frontendProcess->close();
    delete m_frontendProcess;

    delete m_undoRedoStack;

    QString prefixBase = "temp";
    QString prefix = prefixBase + QString::number(m_tempFilesIndex);
    QFile source(prefix + "_source.rail");
    QFile input(prefix +"_input.txt");
    QFile output(prefix + "_output.txt");

    source.remove();
    input.remove();
    output.remove();
}

void MainWindow::createTempFiles()
{
    QString prefixBase = "temp";
    for(int i = 0;; i++)
    {
        QString prefix = prefixBase + QString::number(i);
        QFile source(prefix + "_source.rail");
        QFile input(prefix +"_input.txt");
        QFile output(prefix + "_output.txt");

        // check if the names are already used
        // if yes, try again with different names
        if(source.exists() || input.exists() || output.exists())
        {
            continue;
        }

        // create (temporary) files
        if(source.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            source.close();
        }
        else
        {
            continue;
        }

        if(input.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            input.close();
        }
        else
        {
            source.remove();
            continue;
        }


        if(output.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            output.close();
        }
        else
        {
            source.remove();
            input.remove();
            continue;
        }


        m_tempFilesIndex = i;
        return;
    }
}

bool MainWindow::saveChanges()
{
    if(!m_modified)
    {
        return true;
    }
    else
    {
        QMessageBox closeMessageBox;
        closeMessageBox.setText("You have unsaved changes.");
        closeMessageBox.setInformativeText("Do you want to save your changes?");
        QPushButton *saveButton = closeMessageBox.addButton("Save", QMessageBox::ActionRole);
        QPushButton *discardButton = closeMessageBox.addButton("Discard", QMessageBox::ActionRole);
        closeMessageBox.addButton("Cancel", QMessageBox::RejectRole);

        closeMessageBox.exec();

        if( closeMessageBox.clickedButton() == saveButton )
        {
            saveFile();
            return true;
        }
        else if( closeMessageBox.clickedButton() == discardButton )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *closeEvent)
{
    if(saveChanges())
    {
        closeEvent->accept();
    }
    else
    {
        closeEvent->ignore();
    }
}

void MainWindow::cursorPositionChanged(int row, int col)
{
    ui->ui_rowValueLabel->setText(QString::number(row + 1));
    ui->ui_columnValueLabel->setText(QString::number(col + 1));
}

void MainWindow::textChanged()
{
    setModified(true);
}

void MainWindow::setModified(bool modified)
{
    m_modified = modified;
    this->setWindowModified(modified);
}

void MainWindow::setCurrentPath(QString currentFilePath)
{
    m_currentFilePath = currentFilePath;
    if(currentFilePath.isEmpty())
    {
        this->setWindowTitle("Rail Editor - New File[*]");
    }
    else
    {
        this->setWindowTitle("Rail Editor - " + QFileInfo(currentFilePath).fileName() + "[*]");
    }
}

bool MainWindow::saveFile()
{
    if(m_currentFilePath.isEmpty())
    {
        return saveFileAs();
    }
    else
    {
        return save(m_currentFilePath);
    }
}

bool MainWindow::saveFileAs()
{
    QFileDialog saveDialog(this);
    saveDialog.setWindowTitle("Save File");
    saveDialog.setNameFilters(QStringList() << "Rail Files (*.rail)" << "Text Files (*.txt)" << "All Files (*.*)");

    if(saveDialog.exec() && !saveDialog.selectedFiles().isEmpty())
    {
        return save(saveDialog.selectedFiles().first());
    }
    return false;
}

bool MainWindow::save(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        return false;
    }
    QTextStream out(&file);
    out << ui->ui_sourceEditTableWidget->toPlainText();
    file.close();
    setModified(false);
    setCurrentPath(filePath);
    return true;
}

void MainWindow::openFile()
{
    if(saveChanges())
    {
        QFileDialog openDialog(this);
        openDialog.setWindowTitle("Open File");
        openDialog.setFileMode(QFileDialog::ExistingFile);
        openDialog.setNameFilters(QStringList() << "Rail Files (*.rail)" << "Text Files (*.txt)" << "All Files (*.*)");

        if(openDialog.exec() && !openDialog.selectedFiles().isEmpty())
        {
            QString filePath = openDialog.selectedFiles().first();
            QFile file(filePath);
            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                setCurrentPath(filePath);
                ui->ui_sourceEditTableWidget->clear();
                ui->ui_inputPlainTextEdit->clear();
                ui->ui_outputPlainTextEdit->clear();
                ui->ui_sourceEditTableWidget->setPlainText(file.readAll());
                setModified(false);
            }
            file.close();
        }
    }
}

void MainWindow::newFile()
{
    if(saveChanges())
    {
        ui->ui_sourceEditTableWidget->clear();
        ui->ui_inputPlainTextEdit->clear();
        ui->ui_outputPlainTextEdit->clear();
        setModified(false);
        setCurrentPath(QString());
    }
}

void MainWindow::createUndoElement(UndoRedoElement* e)
{
    m_undoRedoStack->createUndoElement(e);
}

void MainWindow::undo()
{
    UndoRedoElement* e = m_undoRedoStack->undo();
    ui->ui_sourceEditTableWidget->undo(e);
}

void MainWindow::redo()
{
    UndoRedoElement* e = m_undoRedoStack->redo();
    ui->ui_sourceEditTableWidget->redo(e);
}

void MainWindow::setInterpreter()
{
    QFileDialog openDialog(this);
    openDialog.setWindowTitle("Select Interpreter");
    openDialog.setFileMode(QFileDialog::ExistingFile);

    if(openDialog.exec() && !openDialog.selectedFiles().isEmpty())
    {
        m_currentInterpreterPath = openDialog.selectedFiles().first();
    }
}

void MainWindow::runInterpreter()
{
    if(m_currentInterpreterPath.isEmpty())
    {
        QMessageBox::information(this, "No Interpreter set.", "You need to set an Interpreter first.");
        return;
    }
    if(m_interpreterProcess->state() == QProcess::Running)
    {
        QMessageBox::information(this, "Interpreter is already running.", "The Interpreter is already running.");
        return;
    }

    QString prefixBase = "temp";
    QString prefix = prefixBase + QString::number(m_tempFilesIndex);
    QFile source(prefix + "_source.rail");
    QFile input(prefix +"_input.txt");
    QFile output(prefix + "_output.txt");
    if(source.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        QTextStream out(&source);
        out << ui->ui_sourceEditTableWidget->toPlainText();
        source.close();
    }
    else
    {
        return;
    }

    if(input.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        QTextStream out(&input);
        out << ui->ui_inputPlainTextEdit->toPlainText();
        input.close();
    }
    else
    {
        return;
    }

    if(output.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        QTextStream out(&input);
        out << QString();
        output.close();
    }
    else
    {
        return;
    }


    ui->ui_outputPlainTextEdit->clear();

    // run the interpreter
    QStringList parameter;
    parameter << "--input=" + QFileInfo(input).absoluteFilePath()
              << "--output=" + QFileInfo(output).absoluteFilePath()
              << QFileInfo(source).absoluteFilePath();
    m_interpreterProcess->start(m_currentInterpreterPath, parameter);
}

void MainWindow::interpreterStarted()
{
    ui->ui_stopInterpreterAction->setEnabled(true);
    ui->ui_runInterpreterAction->setEnabled(false);
}

void MainWindow::interpreterFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    ui->ui_stopInterpreterAction->setEnabled(false);
    ui->ui_runInterpreterAction->setEnabled(true);

    // print the results
    QString prefixBase = "temp";
    QString prefix = prefixBase + QString::number(m_tempFilesIndex);
    QFile output(prefix + "_output.txt");
    QString line;
    if(output.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&output);
        while(!stream.atEnd())
        {
            line = stream.readLine();
            ui->ui_outputPlainTextEdit->appendPlainText(line + "\n");
        }
    }
}

void MainWindow::interpreterOutputReady()
{
    ui->ui_outputPlainTextEdit->appendPlainText(m_interpreterProcess->readAllStandardOutput() + "\n");
}

void MainWindow::interpreterErrorReady()
{
    ui->ui_outputPlainTextEdit->appendHtml("<font color=red>" + m_interpreterProcess->readAllStandardError() + "</font>\n");
}

// --------------------------------------------------------------------------- frontend

void MainWindow::setFrontend()
{
    QFileDialog openDialog(this);
    openDialog.setWindowTitle("Select Frontend");
    openDialog.setFileMode(QFileDialog::ExistingFile);

    if(openDialog.exec() && !openDialog.selectedFiles().isEmpty())
    {
        m_currentFrontendPath = openDialog.selectedFiles().first();
    }
}

void MainWindow::runFrontend()
{
    if(m_currentFilePath.isEmpty())
    {
        if(!saveFile())
        {
            return;
        }
    }
    else
    {
        saveFile();
    }

    if(m_currentFrontendPath.isEmpty())
    {
        QMessageBox::information(this, "No Compiler set.", "You need to set a Compiler first.");
        return;
    }
    if(m_interpreterProcess->state() == QProcess::Running)
    {
        QMessageBox::information(this, "Compiler is already running.", "The Compiler is already running.");
        return;
    }

    QString basePath = QFileInfo(m_currentFilePath).absoluteFilePath().remove(QRegExp("\\.w*$"));
    QString inputFile = basePath + ".rail";
    QString astFile = basePath + ".ast";
    QString graphFile = basePath + ".dot";
    QString classFile = basePath + ".class";

    // run the compiler
    QStringList parameter;
    parameter << "-i " + inputFile
              << "-o " + classFile
              << "-s " + astFile
              << "-g " + graphFile
              << "-q"; // quiet
    m_frontendProcess->start(m_currentInterpreterPath, parameter);
}


void MainWindow::frontendStarted()
{
    ui->ui_stopFrontendAction->setEnabled(true);
    ui->ui_runFrontendAction->setEnabled(false);
}

void MainWindow::frontendFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    ui->ui_stopFrontendAction->setEnabled(false);
    ui->ui_runFrontendAction->setEnabled(true);
    ui->ui_compilerOutputPlainTextEdit->appendPlainText("Compiler finished with exit code " + QString::number(exitCode) + "\n");
}

void MainWindow::frontendOutputReady()
{
    ui->ui_compilerOutputPlainTextEdit->appendPlainText(m_frontendProcess->readAllStandardOutput());
}

void MainWindow::frontendErrorReady()
{
    ui->ui_compilerOutputPlainTextEdit->appendHtml("<font color=red>" + m_frontendProcess->readAllStandardError() + "</font>\n");
}
