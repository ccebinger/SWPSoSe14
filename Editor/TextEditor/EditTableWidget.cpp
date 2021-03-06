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


    if(!keyEvent->text().isEmpty() || key == Qt::Key_Dead_Circumflex)
    {
        if(key == Qt::Key_Enter || key == Qt::Key_Return)
        {
            if(m_isInGrabMode)
            {
                finishGrab();
                return;
            }
            if((ApplicationPreferences::cursorMode == ApplicationConstants::SMART)
                    && (getDisplaySign(m_cursorRowPos, 0) == '$'))
            {
                setPosition(m_cursorRowPos + 1, 1);
            }
            else
            {
                setPosition(m_cursorRowPos + 1, 0);
            }
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
            if(key == Qt::Key_Dead_Circumflex)
            {
                setSign(m_cursorRowPos, m_cursorColPos, '^');
            }
            else
            {
                setSign(m_cursorRowPos, m_cursorColPos, keyEvent->text().at(0));
            }
            // next position is calculated by the internal graph
            // hence we don't set the next position here
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

void EditTableWidget::setSign(int row, int col, QChar c, bool suppressUndoRedoCreation)
{
    if(c == QChar())
    {
        removeSign(row, col, suppressUndoRedoCreation);
        return;
    }
    QChar pre = getDisplaySign(row, col);
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
    // The first element is the next position of the cursor
    // the direction is coded in the color value
    Stack *tmp = stack->pop();
    int deltaX = 0, deltaY = 0;
    ApplicationConstants::Direction direction = static_cast<ApplicationConstants::Direction>(tmp->getColor());
    m_graph.directionToDelta(&deltaX, &deltaY, direction);
    setPosition(std::max(0, m_cursorRowPos + deltaY), std::max(0, m_cursorColPos + deltaX));
    delete tmp;
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

void EditTableWidget::setDisplaySign(int row, int col, QChar c)
{
    if(c == QChar())// || c == '\0')
    {
        removeDisplaySign(row, col);
        return;
    }
    if(row >= this->rowCount())
    {
        this->setRowCount(row+1);
    }
    if(col >= this->columnCount())
    {
        this->setColumnCount(col+1);
    }

    QChar symbol = c;
    if(c == ' ' && ApplicationPreferences::showWhiteSpaces)
    {
        symbol = visibleWhiteSpace;
    }

    QWidget *w = this->cellWidget(row, col);
    QLabel *l;
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
        l->setText(symbol);
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
            QChar pre = getDisplaySign(row, col);
            UndoRedoTypeCharacter *undoRedoElement = new UndoRedoTypeCharacter(row, col, pre, QChar());
            emit undoRedoElementCreated(undoRedoElement);
        }
        removeDisplaySign(row, col);

        if((col == m_textMaxCol) || (row == m_textMaxRow))
        {
            findContentMaxValues();
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

void EditTableWidget::removeDisplaySign(int row, int col)
{
    QWidget *w = this->cellWidget(row, col);
    if(w != NULL)
    {
        removeCellWidget(row, col);
    }
}

QChar EditTableWidget::getDisplaySign(int row, int col) const
{
    QWidget *w = this->cellWidget(row, col);
    if(w != NULL)
    {
        QLabel *l = dynamic_cast<QLabel *>(w);
        assert(l);
        QChar c = l->text().at(0);
        if(c == visibleWhiteSpace)
        {
            c = ' ';
        }
        return c;
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
    bool unconnected = byteMask & ApplicationConstants::UNCONNECTED_OBJECT;
    bool connectedRail = byteMask & ApplicationConstants::CONNECTED_RAIL;
    bool functionName = byteMask & ApplicationConstants::FUNCTION_NAME;
    bool functionCall = byteMask & ApplicationConstants::FUNCTION_CALL;
    bool systemFunction = byteMask & ApplicationConstants::SYSTEM_FUNCTION;
    bool string = byteMask & ApplicationConstants::STRING;
    bool variable = byteMask & ApplicationConstants::VARIABLE;
    bool grab = byteMask & ApplicationConstants::GRAB;

    bool bold = false;

    QColor color;

    if(grab)
    {
        color = ApplicationPreferences::grabColor;
    }
    else if(unconnected)
    {
        color = ApplicationPreferences::unconnectedRailsColor;
    }
    else if(functionName)
    {
        color = ApplicationPreferences::functionNamesColor;
    }
    else if(string)
    {
        color = ApplicationPreferences::stringsColor;
        bold = variable || functionCall || systemFunction || connectedRail;
    }
    else if(variable)
    {
        color = ApplicationPreferences::variablesColor;
        bold = functionCall || systemFunction || connectedRail;
    }
    else if(functionCall)
    {
        color = ApplicationPreferences::functionCallsColor;
        bold = systemFunction || connectedRail;
    }
    else if(systemFunction)
    {
        color = ApplicationPreferences::systemFunctionColor;
    }
    else if(connectedRail)
    {
        color = ApplicationPreferences::connectedRailsColor;
    }

    QString colorName = color.name();
    QString boldName = bold ? "bold" : "normal";

    QWidget *w = this->cellWidget(row, col);

    QLabel *l;
    if(w != NULL)
    {
        l = dynamic_cast<QLabel *>(w);
        assert(l);
        l->setStyleSheet("QLabel { color: " + colorName + "; font-weight: " + boldName + ";};");
    }
}

QString EditTableWidget::toPlainText() const
{
    QString text;
    for(int row = 0; row < this->rowCount(); row++)
    {
        for(int col = 0; col < this->columnCount(); col++)
        {
            QChar sign = getDisplaySign(row, col);
            if(sign == QChar())
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
    ApplicationConstants::CursorMode tmpCursorMode = ApplicationPreferences::cursorMode;
    ApplicationPreferences::cursorMode = ApplicationConstants::NORMAL;
    for(int i = 0; i < text.size(); i++)
    {
        QChar c = text.at(i);
        if(c == '\n')
        {
            setPosition(m_cursorRowPos + 1, 0);
        }
        else
        {
            setSign(m_cursorRowPos, m_cursorColPos, c);
        }
    }
    this->blockSignals(false);
    this->setPosition(0, 0);
    ApplicationPreferences::cursorMode = tmpCursorMode;
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

        m_clipboard = cutPasteUndo->getPre();
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

        m_clipboard = cutPasteRedo->getPost();
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
    QList<QChar> preCharList, postCharList;
    for(int row = selection.topRow(); row <= selection.bottomRow(); row++)
    {
        for(int col = selection.leftColumn(); col <= selection.rightColumn(); col++)
        {
            QChar c = getDisplaySign(row, col);
            preCharList << c;
            removeSign(row, col, true);
            postCharList << QChar();
        }
    }

    int width = selection.rightColumn() - selection.leftColumn() + 1;
    int height = selection.bottomRow() - selection.topRow() + 1;
    if(!isDelete)
    {
        m_clipboard = TextSelection(preCharList, width, height);
    }

    // generate undo/redo-element
    if(!this->signalsBlocked() && !suppressUndoRedoCreation)
    {
        QString display = isDelete ? "Delete" : "Cut";
        TextSelection pre(preCharList, width, height);
        TextSelection post(postCharList, width, height);
        UndoRedoCutPaste *undoRedoElement = new UndoRedoCutPaste(selection.topRow(), selection.leftColumn(), pre, post, display);
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
            QChar c = getDisplaySign(row, col);
            pre << c;
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

    QList<QChar> preCharList, postCharList;
    int i = 0;
    int startRow = m_cursorRowPos;
    int startCol = m_cursorColPos;
    for(int row = startRow; row < startRow + m_clipboard.height(); row++)
    {
        for(int col = startCol; col < startCol + m_clipboard.width(); col++)
        {
            QChar c = m_clipboard.text().at(i++);

            preCharList << getDisplaySign(row, col);
            if(c == QChar())
            {
                removeSign(row, col, true);
            }
            else
            {
                setSign(row, col, c, true);
            }
            postCharList << c;
        }
    }
    setSelection(startRow, startCol, m_clipboard.height(), m_clipboard.width());
    findContentMaxValues();

    // generate undo/redo-element
    if(!suppressUndoRedoCreation && !this->signalsBlocked())
    {
        TextSelection pre(preCharList, m_clipboard.width(), m_clipboard.height());
        TextSelection post(m_clipboard);
        UndoRedoCutPaste *undoRedoElement = new UndoRedoCutPaste(m_cursorRowPos, m_cursorColPos, pre, post, "Paste");
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
    for(int row = 0; row < this->rowCount(); row++)
    {
        for(int col = 0; col < this->columnCount(); col++)
        {
            QChar c = getDisplaySign(row, col);
            if(c == ' ' || c == visibleWhiteSpace)
            {
                setDisplaySign(row, col, c);
            }
            setSignStyle(row, col, m_graph.get_Point_Type(col, row));
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
            setSignStyle(tmpRow, tmpCol, ApplicationConstants::GRAB);
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
            setSignStyle(row, col, m_graph.get_Point_Type(col, row));
        }
    }
}
