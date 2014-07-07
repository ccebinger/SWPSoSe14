#ifndef EDITTABLEWIDGET_H
#define EDITTABLEWIDGET_H

#include "Graph_Interface.h"

#include <QTableWidget>
#include "UndoRedoElement.h"
#include "TextSelection.h"

class EditTableWidget : public QTableWidget
{
    Q_OBJECT
signals:
    void cursorPositionChanged(int row, int col);
    void textChanged();
    void undoRedoElementCreated(UndoRedoElement *);
    void grabModeChanged(bool inGrab);

public:
    explicit EditTableWidget(QWidget *parent = 0);
    void undo(UndoRedoElement* e);
    void redo(UndoRedoElement* e);
    QString toPlainText() const;
    QPoint cursorPos() const;
    void clear();
    void setPlainText(QString text);
    void cut();
    void copy();
    void paste();
    void goToPostion(int row, int column);
    void updateTextStyle();

    void startGrab();
    void cancelGrab();
    void finishGrab();
    void rotateGrab90();
    void rotateGrab180();
    void rotateGrab270();
    void mirrorGrabHorizontal();
    void mirrorGrabVertical();

private:
    void mousePressEvent(QMouseEvent *mouseEvent);
    void mouseMoveEvent(QMouseEvent *mouseEvent);
    void keyPressEvent(QKeyEvent *keyEvent);
    void inputMethodEvent(QInputMethodEvent *event);

    void calculateCellFromPos(QPoint pos, int *row, int *column) const;
    void findContentMaxValues();

    void setPosition(int row, int col, bool extendSelection = false);
    void setSign(int row, int col, QChar c, bool suppressUndoRedoCreation = false);
    void setDisplaySign(int row, int col, QChar c);
    void removeSign(int row, int col, bool suppressUndoRedoCreation = false);
    void removeDisplaySign(int row, int col);
    QChar getSign(int row, int col) const;
    void cut(bool isDelete, bool suppressUndoRedoCreation);
    void paste(bool suppressUndoRedoCreation);

    void applyStyleChanges(Stack *stack);
    void setSignStyle(int row, int col, int byteMask);

    void setForegroundText(int row, int col);
    void restoreBackgroundText(int row, int col, int height, int width);
    void calculateDimensions();
    void setSelection(int row, int col, int height, int width);

private:
    int m_cursorRowPos, m_cursorColPos;
    int m_selectionStartRowPos, m_selectionStartColPos;
    int m_textMaxRow, m_textMaxCol;
    const int m_elementHeight, m_elementWidth;
    Graph_Interface m_graph;
    TextSelection m_clipboard;

    bool m_isInGrabMode;
    TextSelection m_currentGrabbedText;
    TextSelection m_originalGrabbedText;
    int m_originalGrabOriginRow;
    int m_originalGrabOriginCol;
    // current grab origin is current cursor position

};

#endif // EDITTABLEWIDGET_H
