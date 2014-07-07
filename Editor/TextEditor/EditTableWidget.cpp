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
#include <UndoRedoGrab.h>

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
    if(rowAt < 0)
    {
        cellRow = ceil((double)pos.y() / (double)m_elementHeight) - 1;
    }
    else
    {
        cellRow = rowAt;
    }
    if(colAt < 0)
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

void EditTableWidget::findContentMaxValues()
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

    if(m_isInGrabMode)
    {
        restoreBackgroundText(m_cursorRowPos, m_cursorColPos, m_currentGrabbedText.height(), m_currentGrabbedText.width());
        setPosition(row, col);
        setForegroundText(m_cursorRowPos, m_cursorColPos);
    }
    else
    {
        // check if shift-key is pressed
        bool shiftPressed = mouseEvent->modifiers() & Qt::ShiftModifier;
        setPosition(row, col, shiftPressed);
    }
}

void EditTableWidget::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    mouseEvent->accept();
    QPoint pos = mouseEvent->pos();
    int row, col;
    calculateCellFromPos(pos, &row, &col);

    if(m_isInGrabMode)
    {
        restoreBackgroundText(m_cursorRowPos, m_cursorColPos, m_currentGrabbedText.height(), m_currentGrabbedText.width());
        setPosition(row, col);
        setForegroundText(m_cursorRowPos, m_cursorColPos);
    }
    else
    {
        setPosition(row, col, true);
    }
}

void EditTableWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    keyEvent->accept();
    int key = keyEvent->key();


    if(!keyEvent->text().isEmpty())
    {
        if(key == Qt::Key_Enter || key == Qt::Key_Return)
        {
            if(m_isInGrabMode)
            {
                finishGrab();
                return;
            }
            setPosition(m_cursorRowPos + 1, 0);
        }
        else if(key == Qt::Key_Backspace)
        {
            if(m_isInGrabMode)
            {
                return;
            }
            if(m_cursorColPos > 0)
            {
                setPosition(m_cursorRowPos, m_cursorColPos - 1);

                Stack *stack = removeSign(m_cursorRowPos, m_cursorColPos);
                if(stack != NULL)
                {
                    Stack *tmp = stack->pop();
                    delete tmp;
                    applyStyleChanges(stack);
                    delete stack;
                }
            }
            else if(m_cursorRowPos > 0)
            {
                setPosition(m_cursorRowPos - 1, this->columnCount());
                Stack *stack = removeSign(m_cursorRowPos, m_cursorColPos);
                if(stack != NULL)
                {
                    Stack *tmp = stack->pop();
                    delete tmp;
                    applyStyleChanges(stack);
                    delete stack;
                }
            }
        }
        else if(key == Qt::Key_Delete)
        {
            if(m_isInGrabMode)
            {
                return;
            }
            cut(true, false);
        }
        else if(key != Qt::Key_Escape)
        {
            if(m_isInGrabMode)
            {
                return;
            }
            Stack *stack = setSign(m_cursorRowPos, m_cursorColPos, keyEvent->text().at(0));
            if(stack != NULL)
            {
                Stack *tmp = stack->pop();
                moveCursorFromGraph(tmp);
                delete tmp;
                applyStyleChanges(stack);
                delete stack;
            }
        }
        else if(key == Qt::Key_Escape && m_isInGrabMode)
        {
            cancelGrab();
            return;
        }
    }
    else
    {
        bool shiftPressed = keyEvent->modifiers() & Qt::ShiftModifier;
        if(m_isInGrabMode)
        {
            restoreBackgroundText(m_cursorRowPos, m_cursorColPos, m_currentGrabbedText.height(), m_currentGrabbedText.width());
        }
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
            setPosition(rowPos, m_cursorColPos, shiftPressed);
        }
        else if(key == Qt::Key_Down)
        {
            setPosition(m_cursorRowPos + 1, m_cursorColPos, shiftPressed);
        }
        if(m_isInGrabMode)
        {
            setForegroundText(m_cursorRowPos, m_cursorColPos);
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
        int rowCount = std::max(m_cursorRowPos + m_currentGrabbedText.height(), m_textMaxRow);
        int columnCount = std::max(m_cursorColPos + m_currentGrabbedText.width(), m_textMaxCol);
        this->setRowCount(rowCount + 1);
        this->setColumnCount(columnCount + 1);

        this->setCurrentCell(m_cursorRowPos, m_cursorColPos, QItemSelectionModel::Select);
        m_selectionStartRowPos = m_cursorRowPos;
        m_selectionStartColPos = m_cursorColPos;
    }
    emit cursorPositionChanged(row, col);
}

Stack * EditTableWidget::setSign(int row, int col, QChar c, bool suppressUndoRedoCreation)
{
    if(c == QChar())
    {
        return removeSign(row, col, suppressUndoRedoCreation);
    }

    QChar pre = getSign(row, col);
    setDisplaySign(row, col, c);

    // generate undo/redo-element
    if(!suppressUndoRedoCreation && !this->signalsBlocked())
    {
        UndoRedoTypeCharacter *undoRedoElement = new UndoRedoTypeCharacter(row, col, pre, c);
        emit undoRedoElementCreated(undoRedoElement);
    }

    m_textMaxRow = std::max(m_textMaxRow, row);
    m_textMaxCol = std::max(m_textMaxCol, col);

    Stack *stack = m_graph.setSign(col, row, c.toLatin1());
    if(!suppressUndoRedoCreation)
    {
        emit textChanged();
    }
    return stack;
}

void EditTableWidget::setDisplaySign(int row, int col, QChar c)
{
    if(c == QChar() || c == '\0')
    {
        removeDisplaySign(row, col);
        return;
    }
    QChar symbol = c;
    if(c == ' ' && ApplicationPreferences::showWhiteSpaces)
    {
        symbol = visibleWhiteSpace;
    }

    QWidget *w = this->cellWidget(row, col);
    QLabel *l;
    QChar pre;
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
}

Stack * EditTableWidget::removeSign(int row, int col, bool suppressUndoRedoCreation)
{
    QWidget *w = this->cellWidget(row, col);
    if(w != NULL)
    {
        // generate undo/redo-element
        if(!suppressUndoRedoCreation && !this->signalsBlocked())
        {
            QChar pre = getSign(row, col);
            if(pre == visibleWhiteSpace)
            {
                pre = ' ';
            }
            UndoRedoTypeCharacter *undoRedoElement = new UndoRedoTypeCharacter(row, col, pre, QChar());
            emit undoRedoElementCreated(undoRedoElement);
        }
        removeDisplaySign(row, col);

        if((col == m_textMaxCol) || (row == m_textMaxRow))
        {
            findContentMaxValues();
        }
        Stack *stack = m_graph.deleteSign(col, row);

        if(!suppressUndoRedoCreation)
        {
            emit textChanged();
        }
        return stack;
    }
    return NULL;
}

void EditTableWidget::removeDisplaySign(int row, int col)
{
    QWidget *w = this->cellWidget(row, col);
    if(w != NULL)
    {
        removeCellWidget(row, col);
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

void EditTableWidget::moveCursorFromGraph(Stack *stack)
{
    assert(stack);
    // The first element is the next position of the cursor
    // the direction is coded in the color value
    int deltaX = 0, deltaY = 0;
    ApplicationConstants::Direction direction = static_cast<ApplicationConstants::Direction>(stack->getColor());
    //qDebug() << "direction:" << direction;
    m_graph.directionToDelta(&deltaX, &deltaY, direction);
    setPosition(m_cursorRowPos + deltaY, m_cursorColPos + deltaX);
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
            Stack *stack = setSign(m_cursorRowPos, m_cursorColPos, c);
            if(stack != NULL)
            {
                Stack *tmp = stack->pop();
                delete tmp;
                applyStyleChanges(stack);
                delete stack;
            }
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

        int width = cutPasteUndo->getRight() - cutPasteUndo->getLeft();
        int height = cutPasteUndo->getBottom() - cutPasteUndo->getTop();
        m_clipboard = TextSelection(cutPasteUndo->getPre(), width, height);

        setPosition(cutPasteUndo->getTop(), cutPasteUndo->getLeft());
        paste(true);

        m_clipboard = tmpClipboard;
    }

    UndoRedoGrab *grabUndo = dynamic_cast<UndoRedoGrab *>(e);
    if(grabUndo)
    {
        TextSelection srcText, dstText;
        int srcRow, srcCol, dstRow, dstCol;
        grabUndo->getPre(&srcText, &dstText, &srcRow, &srcCol, &dstRow, &dstCol);
        TextSelection tmpClipboard = m_clipboard;
        m_clipboard = dstText;
        setPosition(dstRow, dstCol);
        paste(true);
        m_clipboard = srcText;
        setPosition(srcRow, srcCol);
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

        int width = cutPasteRedo->getRight() - cutPasteRedo->getLeft();
        int height = cutPasteRedo->getBottom() - cutPasteRedo->getTop();
        m_clipboard = TextSelection(cutPasteRedo->getPost(), width, height);
        setPosition(cutPasteRedo->getTop(), cutPasteRedo->getLeft());
        paste(true);

        m_clipboard = tmpClipboard;
    }

    UndoRedoGrab *grabRedo = dynamic_cast<UndoRedoGrab *>(e);
    if(grabRedo)
    {
        TextSelection srcText, dstText;
        int srcRow, srcCol, dstRow, dstCol;
        grabRedo->getPost(&srcText, &dstText, &srcRow, &srcCol, &dstRow, &dstCol);
        TextSelection tmpClipboard = m_clipboard;
        m_clipboard = srcText;
        setPosition(srcRow, srcCol);
        paste(true);
        m_clipboard = dstText;
        setPosition(dstRow, dstCol);
        paste(true);
        m_clipboard = tmpClipboard;
    }
}

void EditTableWidget::cut()
{
    cut(false, false);
}

void EditTableWidget::cut(bool isDelete, bool suppressUndoRedoCreation)
{
    QTableWidgetSelectionRange selection = this->selectedRanges().first();
    QList<QChar> pre, post;
    for(int row = selection.topRow(); row <= selection.bottomRow(); row++)
    {
        for(int col = selection.leftColumn(); col <= selection.rightColumn(); col++)
        {
            pre << getSign(row, col);
            removeSign(row, col, true);
            post << QChar();
        }
    }

    if(!isDelete)
    {
        int width = selection.rightColumn() - selection.leftColumn() + 1;
        int height = selection.bottomRow() - selection.topRow() + 1;
        m_clipboard = TextSelection(pre, width, height);
    }

    // generate undo/redo-element
    if(!this->signalsBlocked() && !suppressUndoRedoCreation)
    {
        QString display = isDelete ? "Delete" : "Cut";
        UndoRedoCutPaste *undoRedoElement = new UndoRedoCutPaste(selection.topRow(), selection.leftColumn(), selection.bottomRow(), selection.rightColumn(), pre, post, display);
        emit undoRedoElementCreated(undoRedoElement);
        emit textChanged();
    }
    // shouldn't be necessary, because removeSign() already calls it
    findContentMaxValues();
}

void EditTableWidget::copy()
{
    QTableWidgetSelectionRange selection = this->selectedRanges().first();
    QList<QChar> pre;
    for(int row = selection.topRow(); row <= selection.bottomRow(); row++)
    {
        for(int col = selection.leftColumn(); col <= selection.rightColumn(); col++)
        {
            pre << getSign(row, col);
        }
    }

    int width = selection.rightColumn() - selection.leftColumn() + 1;
    int height = selection.bottomRow() - selection.topRow() + 1;
    m_clipboard = TextSelection(pre, width, height);
}

void EditTableWidget::paste()
{
    paste(false);
}

void EditTableWidget::paste(bool suppressUndoRedoCreation)
{
    // extend the table, if necessary
    int maxRow = std::max(this->rowCount(), m_cursorRowPos + m_clipboard.height());
    int maxCol = std::max(this->columnCount(), m_cursorColPos + m_clipboard.width());

    this->setRowCount(maxRow);
    this->setColumnCount(maxCol);

    QList<QChar> pre, post;
    int i = 0;
    for(int row = m_cursorRowPos; row < m_cursorRowPos + m_clipboard.height(); row++)
    {
        for(int col = m_cursorColPos; col < m_cursorColPos + m_clipboard.width(); col++)
        {
            QChar c = m_clipboard.text().at(i++);

            pre << getSign(row, col);
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
    setSelection(m_cursorRowPos, m_cursorColPos, m_clipboard.height(), m_clipboard.width());
    findContentMaxValues();

    // generate undo/redo-element
    if(!suppressUndoRedoCreation && !this->signalsBlocked())
    {
        UndoRedoCutPaste *undoRedoElement = new UndoRedoCutPaste(m_cursorRowPos, m_cursorColPos, m_cursorRowPos+m_clipboard.height(), m_cursorColPos+m_clipboard.width(), pre, post, "Paste");
        emit undoRedoElementCreated(undoRedoElement);
        emit textChanged();
    }
}

void EditTableWidget::goToPostion(int row, int column)
{
    setPosition(row, column, false);
}

void EditTableWidget::setSelection(int row, int col, int height, int width)
{
    setPosition(row + height - 1, col + width - 1, false);
    setPosition(row, col, true);
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
            // TODO: get sign style from graph interface
        }
    }
}

// --------------------------------------------------------------------------------------- Grab text

void EditTableWidget::startGrab()
{
    if(m_isInGrabMode)
    {
        return;
    }
    m_isInGrabMode = true;
    // initialize current and origin preSrcText
    // cut out of place
    TextSelection tmpClipboard = m_clipboard;
    cut(false, true);
    m_originalGrabbedText = m_currentGrabbedText = m_clipboard;
    QTableWidgetSelectionRange selection = this->selectedRanges().first();
    m_originalGrabOriginRow = selection.topRow();
    m_originalGrabOriginCol = selection.leftColumn();

    m_clipboard = tmpClipboard;

    setForegroundText(m_originalGrabOriginRow, m_originalGrabOriginCol);
    emit grabModeChanged(true);
}

void EditTableWidget::cancelGrab()
{
    if(!m_isInGrabMode)
    {
        return;
    }
    // restore the current overwritten selection
    restoreBackgroundText(m_cursorRowPos, m_cursorColPos, m_currentGrabbedText.height(), m_currentGrabbedText.width());
    // paste back into place
    setPosition(m_originalGrabOriginRow, m_originalGrabOriginCol);
    TextSelection tmpClipboard = m_clipboard;
    m_clipboard = m_originalGrabbedText;
    paste(true);
    m_clipboard = tmpClipboard;
    emit grabModeChanged(false);

    m_originalGrabbedText = m_currentGrabbedText = TextSelection();
    m_isInGrabMode = false;
}

void EditTableWidget::finishGrab()
{
    if(!m_isInGrabMode)
    {
        return;
    }
    QTableWidgetSelectionRange selection = this->selectedRanges().first();
    QList<QChar> preDstList, postSrcList;
    for(int row = selection.topRow(); row <= selection.bottomRow(); row++)
    {
        for(int col = selection.leftColumn(); col <= selection.rightColumn(); col++)
        {
            QChar c;
            if(row <= m_textMaxRow && col <= m_textMaxCol)
            {
                c = QChar(m_graph.getSign(col, row));
            }
            preDstList << c;
            // the size of the list remains the same during grab
            // hence we just create a list of empty qchars here
            postSrcList << QChar();
        }
    }
    int width = selection.rightColumn() - selection.leftColumn() + 1;
    int height = selection.bottomRow() - selection.topRow() + 1;

    TextSelection preSrcText(m_originalGrabbedText);
    TextSelection preDstText(preDstList, width, height);
    TextSelection postSrcText(postSrcList, m_originalGrabbedText.width(), m_originalGrabbedText.height());
    TextSelection postDstText(m_currentGrabbedText);

    // paste into place
    TextSelection tmpClipboard = m_clipboard;
    m_clipboard = m_currentGrabbedText;
    paste(true);
    m_clipboard = tmpClipboard;

    // create UndoRedoElement
    UndoRedoGrab *undoRedoGrab = new UndoRedoGrab(preSrcText, preDstText,
                                                  postSrcText, postDstText,
                                                  m_originalGrabOriginRow, m_originalGrabOriginCol,
                                                  m_cursorRowPos, m_cursorColPos);

    m_originalGrabbedText = m_currentGrabbedText = TextSelection();
    emit undoRedoElementCreated(undoRedoGrab);
    emit grabModeChanged(false);
    m_isInGrabMode = false;
}

void EditTableWidget::rotateGrab90()
{
    if(!m_isInGrabMode)
    {
        return;
    }
    restoreBackgroundText(m_cursorRowPos, m_cursorColPos, m_currentGrabbedText.height(), m_currentGrabbedText.width());
    m_currentGrabbedText.rotate90();
    setForegroundText(m_cursorRowPos, m_cursorColPos);
}

void EditTableWidget::rotateGrab180()
{
    if(!m_isInGrabMode)
    {
        return;
    }
    restoreBackgroundText(m_cursorRowPos, m_cursorColPos, m_currentGrabbedText.height(), m_currentGrabbedText.width());
    m_currentGrabbedText.rotate180();
    setForegroundText(m_cursorRowPos, m_cursorColPos);
}

void EditTableWidget::rotateGrab270()
{
    if(!m_isInGrabMode)
    {
        return;
    }
    restoreBackgroundText(m_cursorRowPos, m_cursorColPos, m_currentGrabbedText.height(), m_currentGrabbedText.width());
    m_currentGrabbedText.rotate270();
    setForegroundText(m_cursorRowPos, m_cursorColPos);
}

void EditTableWidget::mirrorGrabHorizontal()
{
    if(!m_isInGrabMode)
    {
        return;
    }
    restoreBackgroundText(m_cursorRowPos, m_cursorColPos, m_currentGrabbedText.height(), m_currentGrabbedText.width());
    m_currentGrabbedText.mirrorHorizontal();
    setForegroundText(m_cursorRowPos, m_cursorColPos);
}

void EditTableWidget::mirrorGrabVertical()
{
    if(!m_isInGrabMode)
    {
        return;
    }
    restoreBackgroundText(m_cursorRowPos, m_cursorColPos, m_currentGrabbedText.height(), m_currentGrabbedText.width());
    m_currentGrabbedText.mirrorVertical();
    setForegroundText(m_cursorRowPos, m_cursorColPos);
}

void EditTableWidget::setForegroundText(int row, int col)
{
    setSelection(row, col, m_currentGrabbedText.height(), m_currentGrabbedText.width());
    int i = 0;
    for(int tmpRow = row; tmpRow < row + m_currentGrabbedText.height(); tmpRow++)
    {
        for(int tmpCol = col; tmpCol < col + m_currentGrabbedText.width(); tmpCol++)
        {
            QChar c = m_currentGrabbedText.text().at(i++);
            setDisplaySign(tmpRow, tmpCol, c);
            setSignStyle(tmpRow, tmpCol, 0x20FFFF00);
        }
    }
    setSelection(row, col, m_currentGrabbedText.height(), m_currentGrabbedText.width());
}

void EditTableWidget::restoreBackgroundText(int row, int col, int height, int width)
{
    for(int tmpRow = row; tmpRow < row + height; tmpRow++)
    {
        for(int tmpCol = col; tmpCol < col + width; tmpCol++)
        {
            QChar c;
            if(tmpRow <= m_textMaxRow && tmpCol <= m_textMaxCol)
            {
                c = QChar(m_graph.getSign(tmpCol, tmpRow));
            }
            setDisplaySign(tmpRow, tmpCol, c);
        }
    }
}
