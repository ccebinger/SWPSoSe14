#include "EditTableWidget.h"

#include <QString>
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

#include <math.h>
#include <assert.h>

EditTableWidget::EditTableWidget(QWidget *parent) :
    QTableWidget(parent),
    m_elementHeight(20),
    m_elementWidth(12)
{
    m_cursorXPos = -1;
    m_cursorYPos = -1;
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
    int cellX, cellY;
    cellX = ceil((double)pos.x() / (double)m_elementWidth);
    cellY = ceil((double)pos.y() / (double)m_elementHeight);
    setPosition(cellX - 1, cellY - 1);
}

void EditTableWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    keyEvent->accept();
    int key = keyEvent->key();

    if(!keyEvent->text().isEmpty())
    {
        if(key == Qt::Key_Enter || key == Qt::Key_Return)
        {
            setPosition(0, m_cursorYPos + 1);
        }
        else if(key == Qt::Key_Backspace)
        {
            if(m_cursorXPos > 0)
            {
                setPosition(m_cursorXPos - 1, m_cursorYPos);
                removeSign();
            }
            else if(m_cursorYPos > 0)
            {
                setPosition(this->columnCount(), m_cursorYPos - 1);
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
            setPosition(m_cursorXPos + 1, m_cursorYPos);
        }
    }
    else
    {
        if(key == Qt::Key_Right)
        {
            setPosition(m_cursorXPos + 1, m_cursorYPos);
        }
        else if(key == Qt::Key_Left)
        {
            int xPos = std::max(0, m_cursorXPos-1);
            setPosition(xPos, m_cursorYPos);
        }
        else if(key == Qt::Key_Up)
        {
            int yPos = std::max(0, m_cursorYPos-1);
            setPosition(m_cursorXPos, yPos);
        }
        else if(key == Qt::Key_Down)
        {
            setPosition(m_cursorXPos, m_cursorYPos + 1);
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

void EditTableWidget::setPosition(int x, int y)
{
    assert(x >= 0);
    assert(y >= 0);
    if(x != m_cursorXPos || y != m_cursorYPos)
    {
        this->setCurrentCell(m_cursorYPos, m_cursorXPos, QItemSelectionModel::Deselect);
        m_cursorXPos = x;
        m_cursorYPos = y;

        int columnCount = std::max(m_cursorXPos, m_textMaxX);
        int rowCount = std::max(m_cursorYPos, m_textMaxY);
        this->setColumnCount(columnCount + 1);
        this->setRowCount(rowCount + 1);

        this->setCurrentCell(m_cursorYPos, m_cursorXPos, QItemSelectionModel::Select);
        emit cursorPositionChanged(x, y);
    }
}

void EditTableWidget::setSign(QChar c)
{
    QWidget *w = this->cellWidget(m_cursorYPos, m_cursorXPos);
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
            c == '*' || c == '<' || c == '>' || c == '^' || c == 'v' )
    {
        color = "gray";
    }
    else
    {
        color = "black";
    }
    QString text;
    if(c == '<')
    {
        text = "&lt;";
    }
    else
    {
        text = QString(c);
    }
    text = "<font color=\"" + color + "\">" + text + "</font>";
    if(w == NULL)
    {
        QFont f("unexistent");
        f.setStyleHint(QFont::Monospace);

        QLabel *l = new QLabel(text);
        l->setFont(f);
        l->setAlignment(Qt::AlignCenter);
        this->setCellWidget(m_cursorYPos, m_cursorXPos, l);
    }
    else
    {
        QLabel *l = dynamic_cast<QLabel *>(w);
        assert(l);
        l->setText(text);
    }
    m_textMaxX = std::max(m_textMaxX, m_cursorXPos);
    m_textMaxY = std::max(m_textMaxY, m_cursorYPos);
    emit textChanged();
}

void EditTableWidget::removeSign()
{
    QWidget *w = this->cellWidget(m_cursorYPos, m_cursorXPos);
    if(w != NULL)
    {
        removeCellWidget(m_cursorYPos, m_cursorXPos);
        // TODO: move all elements on the right one position to the left(?)
        bool newMaxFound;
        if(m_cursorXPos == m_textMaxX)
        {
            newMaxFound = false;
            for(int col = this->columnCount(); col >= 0 && !newMaxFound; col--)
            {
                for(int row = 0; row < this->rowCount() && !newMaxFound; row++)
                {
                    if(this->cellWidget(row, col))
                    {
                        m_textMaxX = col;
                        newMaxFound = true;
                    }
                }
            }
            if(!newMaxFound)
            {
                m_textMaxX = 0;
            }
        }
        if(m_cursorYPos == m_textMaxY)
        {
            newMaxFound = false;
            for(int row = this->rowCount(); row >= 0 && !newMaxFound; row--)
            {
                for(int col = 0; col < this->rowCount() && !newMaxFound; col++)
                {
                    if(this->cellWidget(row, col))
                    {
                        m_textMaxY = row;
                        newMaxFound = true;
                    }
                }
            }
            if(!newMaxFound)
            {
                m_textMaxY = 0;
            }
        }
        emit textChanged();
    }
}

QString EditTableWidget::toPlainText() const
{
    QString text;
    for(int i = 0; i < this->rowCount(); i++)
    {
        for(int j = 0; j < this->columnCount(); j++)
        {
            QLabel *cellLabel = dynamic_cast< QLabel * >(this->cellWidget(i,j));
            if(cellLabel)
            {
                text.append(cellLabel->text());
            }
            else
            {
                text.append(" ");
            }
        }
        if(i < this->rowCount() - 1)
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
    m_textMaxX = 0;
    m_textMaxY = 0;
    setPosition(0, 0);
}

void EditTableWidget::setPlainText(QString text)
{
    this->blockSignals(true);
    clear();
    for(int i = 0; i < text.size(); i++)
    {
        QChar c = text.at(i);
        if(c == '\n')
        {
            setPosition(0, m_cursorYPos + 1);
        }
        else
        {
            setSign(c);
            setPosition(m_cursorXPos + 1, m_cursorYPos);
        }
    }
    this->blockSignals(false);
    this->setPosition(0, 0);
    // Somehow the very first letter is cleared, hence we set it here again manually
    setSign(text.at(0));
}
