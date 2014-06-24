#include "EditTableWidget.h"
#include "Graph_Interface.h"
#include "mainwindow.h"

#include <QString>
#include <QChar>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPoint>
#include <QLabel>
#include <QDebug>
#include <QTableWidgetItem>
#include <QSize>
#include <QHeaderView>
#include <QApplication>
#include <QColor>

#include <math.h>
#include <assert.h>
#include <UndoRedoStack.h>
#include <UndoRedoElement.h>
#include <UndoRedoTypeCharacter.h>
#include <UndoRedoCutPaste.h>

EditTableWidget::EditTableWidget(QWidget *parent) :
    QTableWidget(parent),
    m_elementHeight(20),
    m_elementWidth(12)
{
    m_cursorRowPos = -1;
    m_cursorColPos = -1;
    m_selectionStartColPos = 0;
    m_selectionStartRowPos = 0;
    setPosition(0, 0);    

    m_clipboard = QList<QChar>();
    m_clipboardHeight = 0;
    m_clipboardWidth = 0;

    QHeaderView *verticalHeader = this->verticalHeader();
    verticalHeader->setDefaultSectionSize(m_elementHeight);

    QHeaderView *horizontalHeader = this->horizontalHeader();
    horizontalHeader->setDefaultSectionSize(m_elementWidth);
}

void EditTableWidget::calculateCellFromPos(QPoint pos, int *row, int *column) const
{
    int rowAt = this->rowAt(pos.y());
    int colAt = this->columnAt(pos.x());
    int cellRow, cellCol;
    if(rowAt == -1)
    {
        cellRow = ceil((double)pos.y() / (double)m_elementHeight) - 1;
    }
    else
    {
        cellRow = rowAt;
    }
    if(colAt == -1)
    {
        cellCol = ceil((double)pos.x() / (double)m_elementWidth) - 1;
    }
    else
    {
        cellCol = colAt;
    }

    *row = std::max(0, cellRow);
    *column = std::max(0, cellCol);
}

void EditTableWidget::recalculateMaximumValues()
{
    // find the max column
    bool newMaxFound = false;
    for(int col = this->columnCount(); col >= 0 && !newMaxFound; col--)
    {
        for(int row = 0; row < this->rowCount() && !newMaxFound; row++)
        {
            if(this->cellWidget(row, col))
            {
                m_textMaxCol = col;
                newMaxFound = true;
            }
        }
    }
    if(!newMaxFound)
    {
        m_textMaxCol = 0;
    }

    // find the max row
    newMaxFound = false;
    for(int row = this->rowCount(); row >= 0 && !newMaxFound; row--)
    {
        for(int col = 0; col < this->columnCount() && !newMaxFound; col++)
        {
            if(this->cellWidget(row, col))
            {
                m_textMaxRow = row;
                newMaxFound = true;
            }
        }
    }
    if(!newMaxFound)
    {
        m_textMaxRow = 0;
    }
}

void EditTableWidget::mousePressEvent(QMouseEvent *mouseEvent)
{
    mouseEvent->accept();
    QPoint pos = mouseEvent->pos();
    int row, col;
    calculateCellFromPos(pos, &row, &col);

    // check if shift-key is pressed
    bool shiftPressed = mouseEvent->modifiers() & Qt::ShiftModifier;
    setPosition(row, col, shiftPressed);
}

void EditTableWidget::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    mouseEvent->accept();
    QPoint pos = mouseEvent->pos();
    int row, col;
    calculateCellFromPos(pos, &row, &col);

    setPosition(row, col, true);
}

void EditTableWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    keyEvent->accept();
    int key = keyEvent->key();

    if(!keyEvent->text().isEmpty())
    {
        if(key == Qt::Key_Enter || key == Qt::Key_Return)
        {
            setPosition(m_cursorRowPos + 1, 0);
        }
        else if(key == Qt::Key_Backspace)
        {
            if(m_cursorColPos > 0)
            {
                removeSign();
                setPosition(m_cursorRowPos, m_cursorColPos - 1);
            }
            else if(m_cursorRowPos > 0)
            {
                removeSign();
                setPosition(m_cursorRowPos - 1,this->columnCount());
            }
        }
        else if(key == Qt::Key_Delete)
        {
            removeSign();
        }
        else if(key != Qt::Key_Escape)
        {
            setSign(keyEvent->text().at(0));
            setPosition(m_cursorRowPos, m_cursorColPos + 1);
        }
    }
    else
    {
        bool shiftPressed = keyEvent->modifiers() & Qt::ShiftModifier;
        if(key == Qt::Key_Right)
        {
            setPosition(m_cursorRowPos, m_cursorColPos + 1, shiftPressed);
        }
        else if(key == Qt::Key_Left)
        {
            int colPos = std::max(0, m_cursorColPos - 1);
            setPosition(m_cursorRowPos, colPos, shiftPressed);
        }
        else if(key == Qt::Key_Up)
        {
            int rowPos = std::max(0, m_cursorRowPos - 1);
            setPosition(rowPos, m_cursorColPos,shiftPressed);
        }
        else if(key == Qt::Key_Down)
        {
            setPosition(m_cursorRowPos + 1, m_cursorColPos, shiftPressed);
        }
    }
}

void EditTableWidget::inputMethodEvent(QInputMethodEvent *event)
{
    if(event->commitString() == "^" )
    {
        keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Dead_Circumflex, Qt::NoModifier, "^"));
    }
    event->accept();
}

void EditTableWidget::setPosition(int row, int col, bool extendSelection)
{
    assert(row >= 0);
    assert(col >= 0);
    //if((row != m_cursorRowPos) || (col != m_cursorColPos))
    {
        this->clearSelection();
        m_cursorRowPos = row;
        m_cursorColPos = col;

        if(extendSelection)
        {
            int rowCount = std::max(m_selectionStartRowPos, std::max(m_cursorRowPos, m_textMaxRow));
            int columnCount = std::max(m_selectionStartColPos, std::max(m_cursorColPos, m_textMaxCol));
            this->setRowCount(rowCount + 1);
            this->setColumnCount(columnCount + 1);

            QTableWidgetSelectionRange selectedCells(m_selectionStartRowPos, m_selectionStartColPos, m_cursorRowPos, m_cursorColPos);
            this->setRangeSelected(selectedCells, true);
        }
        else
        {
            int rowCount = std::max(m_cursorRowPos, m_textMaxRow);
            int columnCount = std::max(m_cursorColPos, m_textMaxCol);
            this->setRowCount(rowCount + 1);
            this->setColumnCount(columnCount + 1);

            this->setCurrentCell(m_cursorRowPos, m_cursorColPos, QItemSelectionModel::Select);
            m_selectionStartRowPos = m_cursorRowPos;
            m_selectionStartColPos = m_cursorColPos;
        }
        emit cursorPositionChanged(row, col);
    }
}

void EditTableWidget::setSign(QChar c, bool suppressUndoRedoCreation)
{
    QWidget *w = this->cellWidget(m_cursorRowPos, m_cursorColPos);
    QString color;
    if(c == '$')
    {
        color = "blue";
    }
    else if(c == '#')
    {
        color = "green";
    }
    else if(c == '-' || c == '|' || c == '\\' || c == '/' || c == 'x' || c == '+' ||
            c == '*' || c == '<' || c == '>' || c == '^' || c == 'v' || c == '@')
    {
        color = "gray";
    }
    else
    {
        color = "black";
    }
    QString text = QString(c);

    //text = "<font color=\"" + color + "\">" + text + "</font>";
    QLabel *l;
    QChar pre;
    if(w == NULL)
    {
        QFont f("unexistent");
        f.setStyleHint(QFont::Monospace);

        l = new QLabel(text);
        l->setFont(f);
        l->setAlignment(Qt::AlignCenter);
        this->setCellWidget(m_cursorRowPos, m_cursorColPos, l);
    }
    else
    {
        l = dynamic_cast<QLabel *>(w);
        assert(l);
        pre = l->text().at(0);
        l->setText(text);
    }

    // generate undo/redo-element
    if(!suppressUndoRedoCreation && !this->signalsBlocked())
    {
        UndoRedoTypeCharacter *undoRedoElement = new UndoRedoTypeCharacter(m_cursorRowPos, m_cursorColPos, pre, c);
        emit undoRedoElementCreated(undoRedoElement);
    }

    l->setStyleSheet("QLabel { color: " + color + "; };");
    m_textMaxRow = std::max(m_textMaxRow, m_cursorRowPos);
    m_textMaxCol = std::max(m_textMaxCol, m_cursorColPos);

    Stack *stack = m_graph.setSign(m_cursorColPos, m_cursorRowPos, text.at(0).toLatin1());
    applyStyleChanges(stack);
    if(stack != NULL)
    {
        delete stack;
    }
    if(!suppressUndoRedoCreation)
    {
        emit textChanged();
    }

}

void EditTableWidget::removeSign(bool suppressUndoRedoCreation)
{
    QWidget *w = this->cellWidget(m_cursorRowPos, m_cursorColPos);
    if(w != NULL)
    {        
        // generate undo/redo-element
        if(!suppressUndoRedoCreation && !this->signalsBlocked())
        {
            QLabel *l = dynamic_cast<QLabel *>(w);
            assert(l);
            QChar pre = l->text().at(0);
            UndoRedoTypeCharacter *undoRedoElement = new UndoRedoTypeCharacter(m_cursorRowPos, m_cursorColPos, pre, QChar());
            emit undoRedoElementCreated(undoRedoElement);
        }

        removeCellWidget(m_cursorRowPos, m_cursorColPos);
        // TODO: move all elements on the right one position to the left(?)
        if((m_cursorColPos == m_textMaxCol) || (m_cursorRowPos == m_textMaxRow))
        {
            recalculateMaximumValues();
        }
        Stack *stack = m_graph.deleteSign(m_cursorColPos, m_cursorRowPos);
        applyStyleChanges(stack);
        if(stack != NULL)
        {
            delete stack;
        }

        if(!suppressUndoRedoCreation)
        {
            emit textChanged();
        }
    }
}

void EditTableWidget::applyStyleChanges(Stack *stack)
{
    Stack *tmp;
    while((tmp = stack->pop()) != NULL)
    {
        setSignStyle(tmp->getY(), tmp->getX(), tmp->getColor());

        delete tmp;
    }
}

void EditTableWidget::setSignStyle(int row, int col, int byteMask)
{
    // int: 00000000 00000000 00000000 00000000
    //      rrrrrrrr gggggggg bbbbbbbb aaaaaaaa (rgb alpha)
    //                                       ib
    // We ignore alpha and use the bits of the least significant byte as style flags

    bool bold = byteMask & 1;
    bool italic = byteMask & 2;

    int red = (byteMask & 0xFF000000) >> 24;
    int green = (byteMask & 0x00FF0000) >> 16;
    int blue = (byteMask & 0x0000FF00) >> 8;

    QColor color = QColor(red, green, blue);
    QString colorName = color.name();
    QString boldName = bold ? "bold" : "normal";
    QString italicName = italic ? "italic" : "normal";

    QWidget *w = this->cellWidget(row, col);

    QLabel *l;
    if(w != NULL)
    {
        l = dynamic_cast<QLabel *>(w);
        assert(l);
        l->setStyleSheet("QLabel { color: " + colorName + "; font-weight: " + boldName + "; font-style: " + italicName + ";};");
    }
}

QString EditTableWidget::toPlainText() const
{
    QString text;
    for(int row = 0; row < this->rowCount(); row++)
    {
        for(int col = 0; col < this->columnCount(); col++)
        {
            QLabel *cellLabel = dynamic_cast< QLabel * >(this->cellWidget(row, col));
            if(cellLabel)
            {
                text.append(cellLabel->text());
            }
            else
            {
                text.append(" ");
            }
        }
        if(row < this->rowCount() - 1)
        {
            text.append("\n");
        }
    }
    // remove trailing whitespaces
    text = text.replace(QRegExp("\\s*\\n"), "\n");
    return text;
}

void EditTableWidget::clear()
{
    this->setRowCount(0);
    this->setColumnCount(0);
    m_textMaxRow = 0;
    m_textMaxCol = 0;
    setPosition(0, 0);
    m_graph.clear();
}

void EditTableWidget::setPlainText(QString text)
{
    this->blockSignals(true);
    clear();
    for(int i = 0; i < text.size(); i++)
    {
        QChar c = text.at(i);
        //qDebug() << "i: " << i << c;
        //qDebug() << "row: " << m_cursorRowPos << "col: " << m_cursorColPos;
        if(c == '\n')
        {
            setPosition(m_cursorRowPos + 1, 0);
        }
        else
        {
            setSign(c);
            setPosition(m_cursorRowPos, m_cursorColPos + 1);
        }
    }
    this->blockSignals(false);
    this->setPosition(0, 0);
    // Somehow the very first letter is cleared, hence we set it here again manually
    setSign(text.at(0));
}

void EditTableWidget::undo(UndoRedoElement *e)
{
    UndoRedoTypeCharacter *typeUndo = dynamic_cast<UndoRedoTypeCharacter *>(e);
    if(typeUndo)
    {
        this->setPosition(typeUndo->getRow(), typeUndo->getColumn());
        QChar pre = typeUndo->getPre();
        if(pre != QChar())
        {
            this->setSign(pre, true);
        }
        else
        {
            this->removeSign(true);
        }
    }
    UndoRedoCutPaste *cutPasteUndo = dynamic_cast<UndoRedoCutPaste *>(e);
    if(cutPasteUndo)
    {
        QList<QChar> tmpClipboard = m_clipboard;
        int tmpClipboardHeight = m_clipboardHeight;
        int tmpClipboardWidth = m_clipboardWidth;

        m_clipboard = cutPasteUndo->getPre();
        m_clipboardHeight = cutPasteUndo->getBottom() - cutPasteUndo->getTop();
        m_clipboardWidth = cutPasteUndo->getRight() - cutPasteUndo->getLeft();
        setPosition(cutPasteUndo->getTop(), cutPasteUndo->getLeft());
        paste(true);

        m_clipboard = tmpClipboard;
        m_clipboardHeight = tmpClipboardHeight;
        m_clipboardWidth = tmpClipboardWidth;
    }
}

void EditTableWidget::redo(UndoRedoElement *e)
{
    UndoRedoTypeCharacter *typeRedo = dynamic_cast<UndoRedoTypeCharacter *>(e);
    if(typeRedo)
    {
        this->setPosition(typeRedo->getRow(), typeRedo->getColumn());
        QChar post = typeRedo->getPost();
        if(post != QChar())
        {
            this->setSign(post, true);
        }
        else
        {
            this->removeSign(true);
        }
    }
    UndoRedoCutPaste *cutPasteRedo = dynamic_cast<UndoRedoCutPaste *>(e);
    if(cutPasteRedo)
    {
        QList<QChar> tmpClipboard = m_clipboard;
        int tmpClipboardHeight = m_clipboardHeight;
        int tmpClipboardWidth = m_clipboardWidth;

        m_clipboard = cutPasteRedo->getPost();
        m_clipboardHeight = cutPasteRedo->getBottom() - cutPasteRedo->getTop();
        m_clipboardWidth = cutPasteRedo->getRight() - cutPasteRedo->getLeft();
        setPosition(cutPasteRedo->getTop(), cutPasteRedo->getLeft());
        paste(true);

        m_clipboard = tmpClipboard;
        m_clipboardHeight = tmpClipboardHeight;
        m_clipboardWidth = tmpClipboardWidth;
    }
}

void EditTableWidget::cut()
{
    QTableWidgetSelectionRange selection = this->selectedRanges().first();
    QWidget *w;
    QLabel *l;
    QList<QChar> pre, post;
    for(int row = selection.topRow(); row <= selection.bottomRow(); row++)
    {
        for(int col = selection.leftColumn(); col <= selection.rightColumn(); col++)
        {
            w = this->cellWidget(row, col);
            if(w == NULL)
            {
                pre << QChar();
            }
            else
            {
                l = dynamic_cast<QLabel *>(w);
                assert(l);
                pre << l->text().at(0);
                removeCellWidget(row, col);
            }
            post << QChar();
            Stack *stack = m_graph.deleteSign(col, row);
            applyStyleChanges(stack);
            if(stack != NULL)
            {
                delete stack;
            }
        }
    }
    recalculateMaximumValues();

    m_clipboard = pre;
    m_clipboardHeight = selection.bottomRow() - selection.topRow();
    m_clipboardWidth = selection.rightColumn() - selection.leftColumn();

    // generate undo/redo-element
    if(!this->signalsBlocked())
    {
        UndoRedoCutPaste *undoRedoElement = new UndoRedoCutPaste(selection.topRow(), selection.leftColumn(), selection.bottomRow(), selection.rightColumn(), pre, post, "Cut");
        emit undoRedoElementCreated(undoRedoElement);
        emit textChanged();
    }
}

void EditTableWidget::copy()
{
    QTableWidgetSelectionRange selection = this->selectedRanges().first();
    QWidget *w;
    QLabel *l;
    QList<QChar> pre;
    for(int row = selection.topRow(); row <= selection.bottomRow(); row++)
    {
        for(int col = selection.leftColumn(); col <= selection.rightColumn(); col++)
        {
            w = this->cellWidget(row, col);
            if(w == NULL)
            {
                pre << QChar();
            }
            else
            {
                l = dynamic_cast<QLabel *>(w);
                assert(l);
                pre << l->text().at(0);
            }
        }
    }

    m_clipboard = pre;
    m_clipboardHeight = selection.bottomRow() - selection.topRow();
    m_clipboardWidth = selection.rightColumn() - selection.leftColumn();
}

void EditTableWidget::paste()
{
    paste(false);
}

void EditTableWidget::paste(bool suppressUndoRedoCreation)
{
    // extend the table, if necessary
    int maxRow = std::max(this->rowCount(), m_cursorRowPos + m_clipboardHeight+1);
    int maxCol = std::max(this->columnCount(), m_cursorColPos + m_clipboardWidth+1);

    this->setRowCount(maxRow);
    this->setColumnCount(maxCol);

    QWidget *w;
    QLabel *l;
    QList<QChar> pre, post;
    int i = 0;
    for(int row = m_cursorRowPos; row <= m_cursorRowPos + m_clipboardHeight; row++)
    {
        for(int col = m_cursorColPos; col <= m_cursorColPos + m_clipboardWidth; col++)
        {
            w = this->cellWidget(row, col);
            QChar c = m_clipboard.at(i++);
            post << c;
            if(w)
            {
                l = dynamic_cast<QLabel *>(w);
                assert(l);
                pre << l->text().at(0);
                if(c == QChar())
                {
                    // remove sign
                    this->removeCellWidget(row, col);
                    Stack *stack = m_graph.deleteSign(col, row);
                    applyStyleChanges(stack);
                    if(stack != NULL)
                    {
                        delete stack;
                    }
                }
                else
                {
                    l->setText(c);

                    Stack *stack = m_graph.setSign(col, row, c.toLatin1());
                    applyStyleChanges(stack);
                    if(stack != NULL)
                    {
                        delete stack;
                    }
                }
            }
            else
            {
                pre << QChar();
                if(c == QChar())
                {
                    // do nothing
                }
                else
                {
                    // set sign
                    QFont f("unexistent");
                    f.setStyleHint(QFont::Monospace);

                    l = new QLabel(c);
                    l->setFont(f);
                    l->setAlignment(Qt::AlignCenter);
                    this->setCellWidget(row, col, l);

                    Stack *stack = m_graph.setSign(col, row, c.toLatin1());
                    applyStyleChanges(stack);
                    if(stack != NULL)
                    {
                        delete stack;
                    }
                }
            }
        }
    }
    recalculateMaximumValues();

    // generate undo/redo-element
    if(!suppressUndoRedoCreation && !this->signalsBlocked())
    {
        UndoRedoCutPaste *undoRedoElement = new UndoRedoCutPaste(m_cursorRowPos, m_cursorColPos, m_cursorRowPos+m_clipboardHeight, m_cursorColPos+m_clipboardWidth, pre, post, "Paste");
        emit undoRedoElementCreated(undoRedoElement);
        emit textChanged();
    }
}
