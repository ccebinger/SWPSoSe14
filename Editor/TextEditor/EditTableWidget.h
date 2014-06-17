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

private:
    void mousePressEvent(QMouseEvent *mouseEvent);
    void keyPressEvent(QKeyEvent *keyEvent);

    void setPosition(int row, int col);
    void setSign(QChar c, bool isUndoRedo = false);
    void removeSign(bool isUndoRedo = false);
    void inputMethodEvent(QInputMethodEvent *event);

    void applyStyleChanges( Stack *stack );
    void setSignStyle(int row, int col, int byteMask);

private:
    int m_cursorRowPos, m_cursorColPos;
    int m_textMaxRow, m_textMaxCol;
    const int m_elementHeight, m_elementWidth;
    Graph_Interface m_graph;

    void calculateDimensions();
};

#endif // EDITTABLEWIDGET_H
