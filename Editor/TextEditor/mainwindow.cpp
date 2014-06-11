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
    m_compilerProcess = new QProcess(this);

    ui->ui_stopInterpreterAction->setEnabled(false);
    ui->ui_stopCompilerAction->setEnabled(false);

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
    connect(ui->ui_setCompilerAction, SIGNAL(triggered()), this, SLOT(setCompiler()));
    connect(ui->ui_runCompilerAction, SIGNAL(triggered()), this, SLOT(runCompiler()));

    connect(m_interpreterProcess, SIGNAL(started()), this, SLOT(interpreterStarted()));
    connect(m_interpreterProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(interpreterOutputReady()));
    connect(m_interpreterProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(interpreterReadError(QProcess::ProcessError)));
    connect(m_interpreterProcess, SIGNAL(aboutToClose()), this, SLOT(interpreterFinished()));

    connect(m_compilerProcess, SIGNAL(started()), this, SLOT(compilerStarted()));
    connect(m_compilerProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(compilerOutputReady()));
    connect(m_compilerProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(compilerReadError(QProcess::ProcessError)));
    connect(m_compilerProcess, SIGNAL(aboutToClose()), this, SLOT(compilerFinished()));
}

MainWindow::~MainWindow()
{
    delete ui;

    m_interpreterProcess->close();
    delete m_interpreterProcess;
    m_compilerProcess->close();
    delete m_compilerProcess;

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

void MainWindow::saveFile()
{
    if(m_currentFilePath.isEmpty())
    {
        saveFileAs();
    }
    else
    {
        save(m_currentFilePath);
    }
}

void MainWindow::saveFileAs()
{
    QFileDialog saveDialog(this);
    saveDialog.setWindowTitle("Save File");
    saveDialog.setNameFilters(QStringList() << "Rail Files (*.rail)" << "Text Files (*.txt)" << "All Files (*.*)");

    if(saveDialog.exec() && !saveDialog.selectedFiles().isEmpty())
    {
        save(saveDialog.selectedFiles().first());
    }
}

void MainWindow::save(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        return;
    }
    QTextStream out(&file);
    out << ui->ui_sourceEditTableWidget->toPlainText();
    file.close();
    setModified(false);
    setCurrentPath(filePath);
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

void MainWindow::undo()
{

}

void MainWindow::redo()
{

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
    for(int i = 0;; i++)
    {
        QString prefix = prefixBase + QString::number(i);
        QFile source(prefix + "_source.rail");
        QFile input(prefix +"_input.txt");
        QFile output(prefix + "_output.txt");

        // check if the names is already used
        // if yes, try again with different names
        if(source.exists() || input.exists() || output.exists())
        {
            continue;
        }

        // create (temporary) files
        if(source.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&source);
            out << ui->ui_sourceEditTableWidget->toPlainText();
            source.close();
        }
        else
        {
            return;
        }

        if(input.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&input);
            out << ui->ui_inputPlainTextEdit->toPlainText();
            input.close();
        }
        else
        {
            return;
        }

        if(output.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            output.close();
        }
        else
        {
            return;
        }


        // run the interpreter
        QStringList parameter;
        parameter << "--input=" + QFileInfo(input).absoluteFilePath()
                  << "--output=" + QFileInfo(output).absoluteFilePath()
                  << QFileInfo(source).absoluteFilePath();
        int result = m_interpreterProcess->execute(m_currentInterpreterPath, parameter);
        if(result != 0)
        {
            QMessageBox::warning(this, "Error", m_interpreterProcess->errorString());
        }

        // print the results
        QString line;
        if(output.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&output);
            ui->ui_outputPlainTextEdit->clear();
            while(!stream.atEnd())
            {
                line = stream.readLine();
                ui->ui_outputPlainTextEdit->setPlainText(ui->ui_outputPlainTextEdit->toPlainText() + line + "\n");
            }
        }

        // remove temporary files
        source.remove();
        input.remove();
        output.remove();
        return;
    }
}

void MainWindow::interpreterStarted()
{
    ui->ui_stopInterpreterAction->setEnabled(true);
    ui->ui_runInterpreterAction->setEnabled(false);
}

void MainWindow::interpreterFinished()
{
    ui->ui_stopInterpreterAction->setEnabled(false);
    ui->ui_runInterpreterAction->setEnabled(true);
}

void MainWindow::interpreterOutputReady()
{
    ui->ui_outputPlainTextEdit->appendPlainText(m_interpreterProcess->readAllStandardOutput());
}

void MainWindow::interpreterReadError(QProcess::ProcessError error)
{

}

void MainWindow::setCompiler()
{
    QFileDialog openDialog(this);
    openDialog.setWindowTitle("Select Compiler");
    openDialog.setFileMode(QFileDialog::ExistingFile);

    if(openDialog.exec() && !openDialog.selectedFiles().isEmpty())
    {
        m_currentCompilerPath = openDialog.selectedFiles().first();
    }
}

void MainWindow::runCompiler()
{
    if(m_currentCompilerPath.isEmpty())
    {
        QMessageBox::information(this, "No Compiler set.", "You need to set a Compiler first.");
        return;
    }

    // TODO: get the requirements for running the compiler
    // i.e.: parameter, input and output files

    // run the compiler
   /* QProcess *process = new QProcess(this);
    QStringList parameter;
    parameter << "--input=" + QFileInfo(input).absoluteFilePath()
              << "--output=" + QFileInfo(output).absoluteFilePath()
              << QFileInfo(source).absoluteFilePath();
    int result = process->execute(m_currentInterpreterPath, parameter);
    if(result != 0)
    {
        QMessageBox::warning(this, "Error", process->errorString());
    }

    // print the results
    QString line;
    if(output.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&output);
        ui->ui_outputPlainTextEdit->clear();
        while(!stream.atEnd())
        {
            line = stream.readLine();
            ui->ui_outputPlainTextEdit->setPlainText(ui->ui_outputPlainTextEdit->toPlainText() + line + "\n");
        }
    }*/
}


void MainWindow::compilerStarted()
{
    ui->ui_stopCompilerAction->setEnabled(true);
    ui->ui_runCompilerAction->setEnabled(false);
}

void MainWindow::compilerFinished()
{
    ui->ui_stopCompilerAction->setEnabled(false);
    ui->ui_runCompilerAction->setEnabled(true);
}

void MainWindow::compilerOutputReady()
{

}

void MainWindow::compilerReadError(QProcess::ProcessError error)
{

}
