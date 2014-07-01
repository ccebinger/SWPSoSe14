#include "EditTableWidget.h"
#include "Graph_Interface.h"
#include "mainwindow.h"
#include "ApplicationPreferences.h"

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
#include <QPixmap>
#include <QPainter>
#include <QMatrix>

#include <math.h>
#include <assert.h>
#include <UndoRedoStack.h>
#include <UndoRedoElement.h>
#include <UndoRedoTypeCharacter.h>
#include <UndoRedoCutPaste.h>

const QChar visibleWhiteSpace(9251); // whitespace underscore

EditTableWidget::EditTableWidget(QWidget *parent) :
    QTableWidget(parent),
    m_elementHeight(20),
    m_elementWidth(12),
    m_clipboard(QList<QChar>(), 0, 0)
{
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_cursorRowPos = -1;
    m_cursorColPos = -1;
    m_selectionStartColPos = 0;
    m_selectionStartRowPos = 0;
    setPosition(0, 0);    


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
                setPosition(m_cursorRowPos, m_cursorColPos - 1);
                removeSign(m_cursorRowPos, m_cursorColPos);
            }
            else if(m_cursorRowPos > 0)
            {
                setPosition(m_cursorRowPos - 1, this->columnCount());
                removeSign(m_cursorRowPos, m_cursorColPos);
            }
        }
        else if(key == Qt::Key_Delete)
        {
            cut(true);
        }
        else if(key != Qt::Key_Escape)
        {
            setSign(m_cursorRowPos, m_cursorColPos, keyEvent->text().at(0));
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

void EditTableWidget::setSign(int row, int col, QChar c, bool suppressUndoRedoCreation)
{
    QWidget *w = this->cellWidget(row, col);
    QLabel *l;
    QChar pre;
    QChar symbol;
    if(c == ' ' && ApplicationPreferences::showWhiteSpaces)
    {
        symbol = visibleWhiteSpace;
    }
    else
    {
        symbol = c;
    }

    if(w == NULL)
    {
        QFont f("unexistent");
        f.setStyleHint(QFont::Monospace);

        l = new QLabel(symbol);
        l->setFont(f);
        l->setAlignment(Qt::AlignCenter);
        this->setCellWidget(row, col, l);
    }
    else
    {
        l = dynamic_cast<QLabel *>(w);
        assert(l);
        pre = l->text().at(0);
        l->setText(symbol);
    }

    // generate undo/redo-element
    if(!suppressUndoRedoCreation && !this->signalsBlocked())
    {
        UndoRedoTypeCharacter *undoRedoElement = new UndoRedoTypeCharacter(row, col, pre, c);
        emit undoRedoElementCreated(undoRedoElement);
    }

    m_textMaxRow = std::max(m_textMaxRow, row);
    m_textMaxCol = std::max(m_textMaxCol, col);

    Stack *stack = m_graph.setSign(col, row, c.toLatin1());
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

void EditTableWidget::removeSign(int row, int col, bool suppressUndoRedoCreation)
{
    QWidget *w = this->cellWidget(row, col);
    if(w != NULL)
    {        
        // generate undo/redo-element
        if(!suppressUndoRedoCreation && !this->signalsBlocked())
        {
            QLabel *l = dynamic_cast<QLabel *>(w);
            assert(l);
            QChar pre = l->text().at(0);
            if(pre == visibleWhiteSpace)
            {
                pre = ' ';
            }
            UndoRedoTypeCharacter *undoRedoElement = new UndoRedoTypeCharacter(row, col, pre, QChar());
            emit undoRedoElementCreated(undoRedoElement);
        }

        removeCellWidget(row, col);
        if((col == m_textMaxCol) || (row == m_textMaxRow))
        {
            recalculateMaximumValues();
        }
        Stack *stack = m_graph.deleteSign(col, row);
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

QChar EditTableWidget::getSign(int row, int col) const
{
    QWidget *w = this->cellWidget(row, col);
    if(w != NULL)
    {
        QLabel *l = dynamic_cast<QLabel *>(w);
        assert(l);
        return l->text().at(0);
    }
    else
    {
        return QChar();
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
            QChar sign = getSign(row, col);
            if(sign == QChar() || sign == visibleWhiteSpace)
            {
                text.append(' ');
            }
            else
            {
                text.append(sign);
            }
        }
        text.append('\n');
    }
    // remove trailing whitespaces
    text = text.replace(QRegExp("[ \\t]*\\n"), "\n");
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
            setSign(m_cursorRowPos, m_cursorColPos, c);
            setPosition(m_cursorRowPos, m_cursorColPos + 1);
        }
    }
    this->blockSignals(false);
    this->setPosition(0, 0);
    // Somehow the very first letter is cleared, hence we set it here again manually
    setSign(0, 0, text.at(0));
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
            this->setSign(m_cursorRowPos, m_cursorColPos, pre, true);
        }
        else
        {
            this->removeSign(m_cursorRowPos, m_cursorColPos, true);
        }
    }
    UndoRedoCutPaste *cutPasteUndo = dynamic_cast<UndoRedoCutPaste *>(e);
    if(cutPasteUndo)
    {
        TextSelection tmpClipboard(m_clipboard);

        m_clipboard.m_text = cutPasteUndo->getPre();
        m_clipboard.m_height = cutPasteUndo->getBottom() - cutPasteUndo->getTop();
        m_clipboard.m_width = cutPasteUndo->getRight() - cutPasteUndo->getLeft();

        setPosition(cutPasteUndo->getTop(), cutPasteUndo->getLeft());
        paste(true);

        m_clipboard = tmpClipboard;
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
            this->setSign(m_cursorRowPos, m_cursorColPos, post, true);
        }
        else
        {
            this->removeSign(m_cursorRowPos, m_cursorColPos, true);
        }
    }
    UndoRedoCutPaste *cutPasteRedo = dynamic_cast<UndoRedoCutPaste *>(e);
    if(cutPasteRedo)
    {
        TextSelection tmpClipboard(m_clipboard);

        m_clipboard.m_text = cutPasteRedo->getPost();
        m_clipboard.m_height = cutPasteRedo->getBottom() - cutPasteRedo->getTop();
        m_clipboard.m_width = cutPasteRedo->getRight() - cutPasteRedo->getLeft();
        setPosition(cutPasteRedo->getTop(), cutPasteRedo->getLeft());
        paste(true);

        m_clipboard = tmpClipboard;
    }
}

void EditTableWidget::cut()
{
    cut(false);
}

void EditTableWidget::cut(bool isDelete)
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

    if(!isDelete)
    {
        m_clipboard.m_text = pre;
        m_clipboard.m_height = selection.bottomRow() - selection.topRow();
        m_clipboard.m_width = selection.rightColumn() - selection.leftColumn();
    }

    // generate undo/redo-element
    if(!this->signalsBlocked())
    {
        QString display = isDelete ? "Delete" : "Cut";
        UndoRedoCutPaste *undoRedoElement = new UndoRedoCutPaste(selection.topRow(), selection.leftColumn(), selection.bottomRow(), selection.rightColumn(), pre, post, display);
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

    m_clipboard.m_text = pre;
    m_clipboard.m_height = selection.bottomRow() - selection.topRow();
    m_clipboard.m_width = selection.rightColumn() - selection.leftColumn();
}

void EditTableWidget::paste()
{
    paste(false);
}

void EditTableWidget::paste(bool suppressUndoRedoCreation)
{
    // extend the table, if necessary
    int maxRow = std::max(this->rowCount(), m_cursorRowPos + m_clipboard.m_height+1);
    int maxCol = std::max(this->columnCount(), m_cursorColPos + m_clipboard.m_width+1);

    this->setRowCount(maxRow);
    this->setColumnCount(maxCol);

    QWidget *w;
    QLabel *l;
    QList<QChar> pre, post;
    int i = 0;
    for(int row = m_cursorRowPos; row <= m_cursorRowPos + m_clipboard.m_height; row++)
    {
        for(int col = m_cursorColPos; col <= m_cursorColPos + m_clipboard.m_width; col++)
        {
            QChar c = m_clipboard.m_text.at(i++);
            w = this->cellWidget(row, col);
            if(w)
            {
                l = dynamic_cast<QLabel *>(w);
                assert(l);
                pre << l->text().at(0);
            }
            else
            {
                pre << QChar();
            }

            if(c == QChar())
            {
                removeSign(row, col, true);
            }
            else
            {
                setSign(row, col, c, true);
            }
            post << c;
        }
    }
    setSelection(m_cursorRowPos, m_cursorColPos, m_clipboard.m_width, m_clipboard.m_height);
    recalculateMaximumValues();

    // generate undo/redo-element
    if(!suppressUndoRedoCreation && !this->signalsBlocked())
    {
        UndoRedoCutPaste *undoRedoElement = new UndoRedoCutPaste(m_cursorRowPos, m_cursorColPos, m_cursorRowPos+m_clipboard.m_height, m_cursorColPos+m_clipboard.m_width, pre, post, "Paste");
        emit undoRedoElementCreated(undoRedoElement);
        emit textChanged();
    }
}

void EditTableWidget::gotoPostion(int row, int column)
{
    setPosition(row, column, false);
}

void EditTableWidget::setSelection(int row, int col, int width, int height)
{
    setPosition(row + height - 1, col + height - 1, true);
    setPosition(row, col, false);
}

void EditTableWidget::updateTextStyle()
{
    QChar whiteSpace = ApplicationPreferences::showWhiteSpaces ? visibleWhiteSpace : ' ';
    for(int row = 0; row < this->rowCount(); row++)
    {
        for(int col = 0; col < this->columnCount(); col++)
        {
            QChar c = getSign(row, col);
            if(c == ' ' || c == visibleWhiteSpace)
            {
                setSign(row, col, whiteSpace, true);
            }
        }
    }
}
