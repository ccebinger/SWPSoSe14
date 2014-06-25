#ifndef EDITTABLEWIDGET_H
#define EDITTABLEWIDGET_H

#include "Graph_Interface.h"

#include <QTableWidget>
#include "UndoRedoElement.h"

class EditTableWidget : public QTableWidget
{
    Q_OBJECT
signals:
    void cursorPositionChanged(int row, int col);
    void textChanged();
    void undoRedoElementCreated(UndoRedoElement *);

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

private:
    void mousePressEvent(QMouseEvent *mouseEvent);
    void mouseMoveEvent(QMouseEvent *mouseEvent);
    void keyPressEvent(QKeyEvent *keyEvent);
    void inputMethodEvent(QInputMethodEvent *event);

    void calculateCellFromPos(QPoint pos, int *row, int *column) const;
    void recalculateMaximumValues();
    void setPosition(int row, int col, bool extendSelection = false);
    void setSign(QChar c, bool suppressUndoRedoCreation = false);
    void removeSign(bool suppressUndoRedoCreation = false);
    void paste(bool suppressUndoRedoCreation);

    void applyStyleChanges(Stack *stack);
    void setSignStyle(int row, int col, int byteMask);

private:
    int m_cursorRowPos, m_cursorColPos;
    int m_selectionStartRowPos, m_selectionStartColPos;
    int m_textMaxRow, m_textMaxCol;
    const int m_elementHeight, m_elementWidth;
    Graph_Interface m_graph;
    QList<QChar> m_clipboard;
    int m_clipboardWidth;
    int m_clipboardHeight;

    void calculateDimensions();
};

#endif // EDITTABLEWIDGET_H
