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

#include <assert.h>

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
    ui->ui_compilerOutputPlainTextEdit->setFont(f);

    setCurrentPath(QString());
    setModified(false);
    createTempFiles();

    m_interpreterProcess = new QProcess(this);
    m_buildProcess = new QProcess(this);
    m_javaProcess = new QProcess(this);

    ui->ui_windowMenu->addAction(ui->ui_ioDockWidget->toggleViewAction());
    ui->ui_windowMenu->addAction(ui->ui_compilerDockWidget->toggleViewAction());
    ui->ui_windowMenu->addAction(ui->ui_consoleDockWidget->toggleViewAction());

    connect(ui->ui_sourceEditTableWidget, SIGNAL(undoRedoElementCreated(UndoRedoElement*)), this, SLOT(undoRedoElementCreated(UndoRedoElement*)));

    m_undoRedoStack = new UndoRedoStack();
    connect(m_undoRedoStack, SIGNAL(modified(bool)), this, SLOT(setModified(bool)));
    connect(m_undoRedoStack, SIGNAL(undoAvailable(bool)), this, SLOT(undoAvailable(bool)));
    connect(m_undoRedoStack, SIGNAL(redoAvailable(bool)), this, SLOT(redoAvailable(bool)));
    ui->ui_undoAction->setEnabled(false);
    ui->ui_redoAction->setEnabled(false);
    ui->ui_pasteAction->setEnabled(false);

    ui->ui_stopInterpreterAction->setEnabled(false);
    ui->ui_stopBuildAction->setEnabled(false);
    ui->ui_stopJavaAction->setEnabled(false);

    connect(ui->ui_sourceEditTableWidget, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(cursorPositionChanged(int,int)));
    connect(ui->ui_sourceEditTableWidget, SIGNAL(textChanged()), this, SLOT(textChanged()));
    connect(ui->ui_newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(ui->ui_openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->ui_saveFileAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->ui_saveAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));
    connect(ui->ui_quitAction, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->ui_undoAction, SIGNAL(triggered()), this, SLOT(undo()));
    connect(ui->ui_redoAction, SIGNAL(triggered()), this, SLOT(redo()));
    connect(ui->ui_cutAction, SIGNAL(triggered()), this, SLOT(cut()));
    connect(ui->ui_copyAction, SIGNAL(triggered()), this, SLOT(copy()));
    connect(ui->ui_pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    connect(ui->ui_setInterpreterAction, SIGNAL(triggered()), this, SLOT(setInterpreter()));
    connect(ui->ui_runInterpreterAction, SIGNAL(triggered()), this, SLOT(runInterpreter()));
    connect(ui->ui_stopInterpreterAction, SIGNAL(triggered()), m_interpreterProcess, SLOT(terminate()));

    connect(ui->ui_setBuildAction, SIGNAL(triggered()), this, SLOT(setBuild()));
    connect(ui->ui_runBuildAction, SIGNAL(triggered()), this, SLOT(runBuild()));
    connect(ui->ui_stopBuildAction, SIGNAL(triggered()), m_buildProcess, SLOT(terminate()));

    connect(ui->ui_buildAndRunJavaAction, SIGNAL(triggered()), this, SLOT(buildAndRun()));
    connect(ui->ui_runJavaAction, SIGNAL(triggered()), this, SLOT(runJava()));
    connect(ui->ui_stopJavaAction, SIGNAL(triggered()), m_javaProcess, SLOT(terminate()));

    connect(m_interpreterProcess, SIGNAL(started()), this, SLOT(interpreterStarted()));
    connect(m_interpreterProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(interpreterOutputReady()));
    connect(m_interpreterProcess, SIGNAL(readyReadStandardError()), this, SLOT(interpreterErrorReady()));
    connect(m_interpreterProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(interpreterFinished(int,QProcess::ExitStatus)));

    connect(m_buildProcess, SIGNAL(started()), this, SLOT(buildStarted()));
    connect(m_buildProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(buildOutputReady()));
    connect(m_buildProcess, SIGNAL(readyReadStandardError()), this, SLOT(buildErrorReady()));
    connect(m_buildProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(buildFinished(int,QProcess::ExitStatus)));
    connect(m_buildProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(buildProcessError(QProcess::ProcessError)));

    connect(m_javaProcess, SIGNAL(started()), this, SLOT(javaStarted()));
    connect(m_javaProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(javaOutputReady()));
    connect(m_javaProcess, SIGNAL(readyReadStandardError()), this, SLOT(javaErrorReady()));
    connect(m_javaProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(javaFinished(int,QProcess::ExitStatus)));
    connect(m_javaProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(javaProcessError(QProcess::ProcessError)));

    // default compiler path
    m_currentBuildPath = "/home/muellerz/SWP/Repository/SWPSoSe14/projekt-compiler/Debug/Compiler";
}

MainWindow::~MainWindow()
{
    delete ui;

    m_interpreterProcess->close();
    delete m_interpreterProcess;
    m_buildProcess->close();
    delete m_buildProcess;
    m_javaProcess->close();
    delete m_javaProcess;

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
    m_undoRedoStack->setSaved();
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
                m_undoRedoStack->clear();
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
        m_undoRedoStack->clear();
        setModified(false);
        setCurrentPath(QString());
    }
}

void MainWindow::undoRedoElementCreated(UndoRedoElement *e)
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

void MainWindow::undoAvailable(bool undoAvailable)
{
    ui->ui_undoAction->setEnabled(undoAvailable);
    QString display = undoAvailable ? m_undoRedoStack->undoDisplay() : "No undo available";
    ui->ui_undoAction->setText("Undo: " + display );
}

void MainWindow::redoAvailable(bool redoAvailable)
{
    ui->ui_redoAction->setEnabled(redoAvailable);
    QString display = redoAvailable ? m_undoRedoStack->redoDisplay() : "No redo available";
    ui->ui_redoAction->setText("Redo: " + display );
}

void MainWindow::cut()
{
    ui->ui_sourceEditTableWidget->cut();
    ui->ui_pasteAction->setEnabled(true);
}

void MainWindow::copy()
{
    ui->ui_sourceEditTableWidget->copy();
    ui->ui_pasteAction->setEnabled(true);
}

void MainWindow::paste()
{
   ui->ui_sourceEditTableWidget->paste();
}

// ----------------------------------------------------------------------------------------- Interpreter

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

// --------------------------------------------------------------------------- Build Process

void MainWindow::setBuild()
{
    QFileDialog openDialog(this);
    openDialog.setWindowTitle("Select Build");
    openDialog.setFileMode(QFileDialog::ExistingFile);

    if(openDialog.exec() && !openDialog.selectedFiles().isEmpty())
    {
        m_currentBuildPath = openDialog.selectedFiles().first();
    }
}

void MainWindow::runBuild()
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

    if(m_currentBuildPath.isEmpty())
    {
        QMessageBox::information(this, "No Compiler set.", "You need to set a Compiler first.");
        return;
    }
    if(m_buildProcess->state() == QProcess::Running)
    {
        QMessageBox::information(this, "Compiler is already running.", "The Compiler is already running.");
        return;
    }

    QFileInfo baseFileInfo(m_currentFilePath);
    QString directory = baseFileInfo.absoluteDir().path();
    QString baseName = baseFileInfo.completeBaseName();

    QString inputFile = directory + "/" + baseName + ".rail";
    QString astFile = directory + "/" + baseName + ".ast";
    QString graphFile = directory + "/" + baseName + ".dot";
    QString classFile = directory + "/" + baseName + ".class";

    // run the compiler
    QStringList parameter;
    parameter << "-i" << inputFile
              << "-o" << classFile
              << "-s" << astFile
              << "-g" << graphFile;
              //<< "-q"; // quiet
    m_buildProcess->start(m_currentBuildPath, parameter);
}


void MainWindow::buildStarted()
{
    ui->ui_stopBuildAction->setEnabled(true);
    ui->ui_runBuildAction->setEnabled(false);
    ui->ui_buildAndRunJavaAction->setEnabled(false);
    ui->ui_runJavaAction->setEnabled(false);

    ui->ui_issuesTableWidget->clearContents();
}

void MainWindow::buildFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    ui->ui_stopBuildAction->setEnabled(false);
    ui->ui_runBuildAction->setEnabled(true);
    ui->ui_buildAndRunJavaAction->setEnabled(true);
    ui->ui_runJavaAction->setEnabled(true);
    ui->ui_compilerOutputPlainTextEdit->appendPlainText("Compiler finished with exit code " + QString::number(exitCode) + "\n");
}

void MainWindow::buildOutputReady()
{
    ui->ui_compilerOutputPlainTextEdit->appendPlainText(m_buildProcess->readAllStandardOutput());
}

void MainWindow::buildErrorReady()
{
    QString stdError = m_buildProcess->readAllStandardError();
    ui->ui_compilerOutputPlainTextEdit->appendHtml("<font color=red>" + stdError + "</font><br>");
}

void MainWindow::buildProcessError(QProcess::ProcessError error)
{
    //QMessageBox::warning(this, "Compiler error.", "Compiler process error: " + QString::number(error));
}


// ------------------------------------------------------------------------------------------------- Java Process

void MainWindow::runJava()
{
    // check if the java process is running
    if(m_javaProcess->state() == QProcess::Running)
    {
        QMessageBox closeMessageBox;
        closeMessageBox.setText("The Java Process is already running.");
        closeMessageBox.setInformativeText("The Java Process is already running. Do you want to restart?");
        QPushButton *yesButton = closeMessageBox.addButton("Yes", QMessageBox::ActionRole);
        closeMessageBox.addButton("Cancel", QMessageBox::RejectRole);

        closeMessageBox.exec();

        if(closeMessageBox.clickedButton() == yesButton)
        {
            m_javaProcess->close();
        }
        else
        {
            return;
        }
    }

    // Just for testing: Start an echo-program
    //m_javaProcess->start("java", QStringList() << "-cp" << "/home/muellerz/SWP/Test/" << "Main");//className);
    //return;

    QString classFilePath = m_currentFilePath;
    QFileInfo classFile(classFilePath);
    QString directory(classFile.absoluteDir().path());
    QString className(classFile.completeBaseName());
    m_javaProcess->start("java", QStringList() << "-cp" << directory << className);
}

void MainWindow::javaStarted()
{
    // TODO: (maybe) Clear java console window
    ui->ui_consolePlainTextEdit->setReadOnly(false);
    ui->ui_consolePlainTextEdit->appendPlainText("Java execution started\n");
    connect(ui->ui_consolePlainTextEdit, SIGNAL(lineEntered(QString)), this, SLOT(consoleLineEntered(QString)));

    ui->ui_runBuildAction->setEnabled(false);
    ui->ui_stopJavaAction->setEnabled(true);
    ui->ui_buildAndRunJavaAction->setEnabled(false);
    //ui->ui_runJavaAction->setEnabled(false);
}

void MainWindow::javaFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    ui->ui_consolePlainTextEdit->appendPlainText("Java finished with exit code " + QString::number(exitCode) + "\n");
    ui->ui_consolePlainTextEdit->setReadOnly(true);
    disconnect(ui->ui_consolePlainTextEdit, SIGNAL(lineEntered(QString)), this, SLOT(consoleLineEntered(QString)));

    ui->ui_runBuildAction->setEnabled(true);
    ui->ui_stopJavaAction->setEnabled(false);
    ui->ui_buildAndRunJavaAction->setEnabled(true);
    //ui->ui_runJavaAction->setEnabled(true);
}

void MainWindow::javaOutputReady()
{
    QString stdOutput = m_javaProcess->readAllStandardOutput();
    //stdOutput = stdOutput.replace(QRegExp("\n"), "<br>");
    ui->ui_consolePlainTextEdit->appendPlainText(stdOutput);
}

void MainWindow::javaErrorReady()
{
    QString stdError = m_javaProcess->readAllStandardError();
    ui->ui_consolePlainTextEdit->appendHtml("<font color=red>" + stdError + "</font><br>");
}

void MainWindow::javaProcessError(QProcess::ProcessError error)
{
    qDebug() << "java error: " << error;
    QMessageBox::warning(this, "Java error.", "Java process error: " + QString::number(error));
}

void MainWindow::consoleLineEntered(QString line)
{
    assert(m_javaProcess->state() == QProcess::Running);
    m_javaProcess->write(line.toStdString().c_str());
}

void MainWindow::buildAndRun()
{
    QEventLoop loop;
    connect(m_buildProcess, SIGNAL(finished(int)), &loop, SLOT(quit()));
    runBuild();
    loop.exec();
    disconnect(m_buildProcess, SIGNAL(finished(int)), &loop, SLOT(quit()));
    if(m_buildProcess->exitCode() != 0)
    {
        QMessageBox closeMessageBox;
        closeMessageBox.setText("Build failed.");
        closeMessageBox.setInformativeText("The Build failed. Do you want to start the previous build.");
        closeMessageBox.addButton("Yes", QMessageBox::ActionRole);
        QPushButton *noButton = closeMessageBox.addButton("No", QMessageBox::RejectRole);

        closeMessageBox.exec();

        if(closeMessageBox.clickedButton() == noButton)
        {
            return;
        }
    }
    runJava();
}
