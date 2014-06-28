#include "FileSelectLineEdit.h"
#include <QFileDialog>
#include <QMouseEvent>

FileSelectLineEdit::FileSelectLineEdit(QWidget *parent)
    : QLineEdit(parent)
    , m_currentFileInfo(QFileInfo())
{
    this->setReadOnly(true);
    this->setText(m_currentFileInfo.absoluteFilePath());
}

FileSelectLineEdit::FileSelectLineEdit(QFileInfo info, QWidget *parent)
    : QLineEdit(parent)
    , m_currentFileInfo(info)
{
    this->setReadOnly(true);
    this->setText(m_currentFileInfo.absoluteFilePath());
}

FileSelectLineEdit::~FileSelectLineEdit()
{

}

void FileSelectLineEdit::mouseDoubleClickEvent(QMouseEvent *mouseEvent)
{
    QFileDialog fileDialog(this);
    fileDialog.setWindowTitle("Select File");

    if(fileDialog.exec() && !fileDialog.selectedFiles().isEmpty())
    {
        m_currentFileInfo = QFileInfo(fileDialog.selectedFiles().first());
        emit fileSelected(m_currentFileInfo);
        this->setText(m_currentFileInfo.absoluteFilePath());
    }
    mouseEvent->accept();
}

void FileSelectLineEdit::setText(const QString &text)
{
    QLineEdit::setText(text);
}

void FileSelectLineEdit::setFileInfo(const QFileInfo &info)
{
    m_currentFileInfo = info;
    this->setText(m_currentFileInfo.absoluteFilePath());
}

QFileInfo FileSelectLineEdit::getFileInfo() const
{
    return m_currentFileInfo;
}
