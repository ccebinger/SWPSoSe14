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

    QFont f("unexistent");
    f.setStyleHint(QFont::Monospace);
    ui->ui_inputPlainTextEdit->setFont(f);
    ui->ui_outputPlainTextEdit->setFont(f);

    setCurrentPath(QString());
    setModified(false);

    connect(ui->ui_sourceEditTableWidget, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(cursorPositionChanged(int,int)));
    connect(ui->ui_sourceEditTableWidget, SIGNAL(textChanged()), this, SLOT(textChanged()));
    connect(ui->ui_newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(ui->ui_openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->ui_saveFileAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->ui_saveAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));
    connect(ui->ui_quitAction, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->ui_setInterpreterAction, SIGNAL(triggered()), this, SLOT(setInterpreter()));
    connect(ui->ui_runInterpreterAction, SIGNAL(triggered()), this, SLOT(runInterpreter()));
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::cursorPositionChanged(int x, int y)
{
    ui->ui_rowValueLabel->setText(QString::number(x + 1));
    ui->ui_columnValueLabel->setText(QString::number(y + 1));
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
        QProcess *process = new QProcess(this);
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
        }

        // remove temporary files
        source.remove();
        input.remove();
        output.remove();
        return;
    }
}
