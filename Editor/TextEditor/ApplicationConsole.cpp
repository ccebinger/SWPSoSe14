#include "ApplicationConsole.h"

ApplicationConsole::ApplicationConsole(QWidget *parent)
    : QPlainTextEdit(parent)
{
}

void ApplicationConsole::appendHtml(const QString &html)
{
    if(!this->isReadOnly())
    {
        // appendHtml adds a newline before appending the text
        // hence we insert the text manually
        this->moveCursor(QTextCursor::End);
        this->textCursor().insertHtml(html);
        this->moveCursor(QTextCursor::End);
        //QPlainTextEdit::appendHtml(html);
        m_userInputStartPosition = this->toPlainText().count();
    }
}

void ApplicationConsole::appendPlainText(const QString &text)
{
    if(!this->isReadOnly())
    {
        // appendPlainText adds a newline before appending the text
        // hence we insert the text manually
        this->moveCursor(QTextCursor::End);
        this->textCursor().insertText(text);
        this->moveCursor(QTextCursor::End);
        //QPlainTextEdit::appendPlainText(text);
        m_userInputStartPosition = this->toPlainText().count();
    }
}

void ApplicationConsole::keyPressEvent(QKeyEvent *keyEvent)
{
    int cursorPos = this->textCursor().position();
    if( cursorPos < m_userInputStartPosition)
    {
        this->moveCursor(QTextCursor::End);
    }
    int key = keyEvent->key();
    // only delete the previous char if the cursor is ahead the start of the modifiable text
    if(key == Qt::Key_Backspace && m_userInputStartPosition >= cursorPos)
    {
        keyEvent->ignore();
        return;
    }
    QPlainTextEdit::keyPressEvent(keyEvent);
    keyEvent->accept();
    if(key == Qt::Key_Enter || key == Qt::Key_Return)
    {
        // read the written text and emit a signal
        QString line;
        QTextCursor tmp = this->textCursor();
        tmp.setPosition(m_userInputStartPosition);
        tmp.setPosition(this->toPlainText().count(), QTextCursor::KeepAnchor);
        line = tmp.selectedText();
        emit lineEntered(line);
        m_userInputStartPosition = this->toPlainText().count();
    }
}
