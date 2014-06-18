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

EditTableWidget::EditTableWidget(QWidget *parent) :
    QTableWidget(parent),
    m_elementHeight(20),
    m_elementWidth(12)
{
    m_cursorRowPos = -1;
    m_cursorColPos = -1;
    setPosition(0, 0);

    QHeaderView *verticalHeader = this->verticalHeader();
    verticalHeader->setDefaultSectionSize(m_elementHeight);

    QHeaderView *horizontalHeader = this->horizontalHeader();
    horizontalHeader->setDefaultSectionSize(m_elementWidth);
}

void EditTableWidget::mousePressEvent(QMouseEvent *mouseEvent)
{
    mouseEvent->accept();
    QPoint pos = mouseEvent->pos();
    int rowAt = this->rowAt(pos.y());
    int colAt = this->columnAt(pos.x());
    int cellRow, cellCol;
    if(rowAt == -1)
    {
        cellRow = ceil((double)pos.y() / (double)m_elementHeight);
    }
    else
    {
        cellRow = rowAt + 1;
    }
    if(colAt == -1)
    {
        cellCol = ceil((double)pos.x() / (double)m_elementWidth);
    }
    else
    {
        cellCol = colAt + 1;
    }

    setPosition(cellRow - 1, cellCol - 1);
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
                removeSign();
            }
            else if(m_cursorRowPos > 0)
            {
                setPosition(m_cursorRowPos - 1,this->columnCount());
                removeSign();
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
        if(key == Qt::Key_Right)
        {
            setPosition(m_cursorRowPos, m_cursorColPos + 1);
        }
        else if(key == Qt::Key_Left)
        {
            int colPos = std::max(0, m_cursorColPos - 1);
            setPosition(m_cursorRowPos, colPos);
        }
        else if(key == Qt::Key_Up)
        {
            int rowPos = std::max(0, m_cursorRowPos - 1);
            setPosition(rowPos, m_cursorColPos);
        }
        else if(key == Qt::Key_Down)
        {
            setPosition(m_cursorRowPos + 1, m_cursorColPos);
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

void EditTableWidget::setPosition(int row, int col)
{
    assert(row >= 0);
    assert(col >= 0);
    //qDebug() << "set " << row << col;
    if((row != m_cursorRowPos) || (col != m_cursorColPos))
    {
        this->setCurrentCell(m_cursorRowPos, m_cursorColPos, QItemSelectionModel::Deselect);
        m_cursorRowPos = row;
        m_cursorColPos = col;

        int rowCount = std::max(m_cursorRowPos, m_textMaxRow);
        int columnCount = std::max(m_cursorColPos, m_textMaxCol);
        this->setRowCount(rowCount + 1);
        this->setColumnCount(columnCount + 1);

        this->setCurrentCell(m_cursorRowPos, m_cursorColPos, QItemSelectionModel::Select);
        emit cursorPositionChanged(row, col);
    }
}

void EditTableWidget::setSign(QChar c, bool isUndoRedo)
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
    if(!isUndoRedo && !this->signalsBlocked())
    {
        UndoRedoTypeCharacter *undoRedoElement = new UndoRedoTypeCharacter(m_cursorRowPos, m_cursorColPos, pre, c);
        emit undoRedoElementCreated(undoRedoElement);
    }

    l->setStyleSheet("QLabel { color: " + color + "; };");
    m_textMaxRow = std::max(m_textMaxRow, m_cursorRowPos);
    m_textMaxCol = std::max(m_textMaxCol, m_cursorColPos);

    Stack *stack = m_graph.setSign(m_cursorColPos, m_cursorRowPos, text.at(0).toLatin1());
    applyStyleChanges(stack);

    // just for testing
    //               red           green         blue       style
    //int byteMask = (255 << 24) | (170 << 16) | (0 << 8) | 3; // orange, bold, italic
    //setSignStyle(m_cursorRowPos, m_cursorColPos, byteMask);

    if(stack != NULL)
    {
        delete stack;
    }
    if(!isUndoRedo)
    {
        emit textChanged();
    }

}

void EditTableWidget::removeSign(bool isUndoRedo)
{
    QWidget *w = this->cellWidget(m_cursorRowPos, m_cursorColPos);
    if(w != NULL)
    {        
        // generate undo/redo-element
        if(!isUndoRedo && !this->signalsBlocked())
        {
            QLabel *l = dynamic_cast<QLabel *>(w);
            assert(l);
            QChar pre = l->text().at(0);
            UndoRedoTypeCharacter *undoRedoElement = new UndoRedoTypeCharacter(m_cursorRowPos, m_cursorColPos, pre, QChar());
            emit undoRedoElementCreated(undoRedoElement);
        }

        removeCellWidget(m_cursorRowPos, m_cursorColPos);
        // TODO: move all elements on the right one position to the left(?)
        bool newMaxFound;
        if(m_cursorColPos == m_textMaxCol)
        {
            newMaxFound = false;
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
        }
        if(m_cursorRowPos == m_textMaxRow)
        {
            newMaxFound = false;
            for(int row = this->rowCount(); row >= 0 && !newMaxFound; row--)
            {
                for(int col = 0; col < this->rowCount() && !newMaxFound; col++)
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
        Stack *stack = m_graph.deleteSign(m_cursorColPos, m_cursorRowPos);
        applyStyleChanges(stack);
        if(stack != NULL)
        {
            delete stack;
        }

        if(!isUndoRedo)
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
    return text;
}

void EditTableWidget::clear()
{
    this->setRowCount(0);
    this->setColumnCount(0);
    m_textMaxRow = 0;
    m_textMaxCol = 0;
    setPosition(0, 0);
    //m_graph.clear();
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
    UndoRedoTypeCharacter *typeUndo = dynamic_cast< UndoRedoTypeCharacter * >( e );
    assert(typeUndo);
    // TODO: later try if and else to determine the derived type of e
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

void EditTableWidget::redo(UndoRedoElement *e)
{
    UndoRedoTypeCharacter *typeRedo = dynamic_cast< UndoRedoTypeCharacter * >( e );
    assert(typeRedo);
    // TODO: later try if and else to determine the derived type of e
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
