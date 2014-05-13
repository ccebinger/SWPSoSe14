#ifndef EDITTABLEWIDGET_H
#define EDITTABLEWIDGET_H

#include <QTableWidget>

class EditTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit EditTableWidget(QWidget *parent = 0);

signals:
    void cursorPositionChanged(int x, int y);
    void textChanged();

public:
    QString toPlainText() const;
    QPoint cursorPos() const;
    void clear();
    void setPlainText(QString text);

private:
    void mousePressEvent(QMouseEvent *mouseEvent);
    void keyPressEvent(QKeyEvent *keyEvent);

    void setPosition(int x, int y);
    void setSign(QChar c);
    void removeSign();
    void inputMethodEvent(QInputMethodEvent *event);

private:
    int m_cursorXPos, m_cursorYPos;
    int m_textMaxX, m_textMaxY; //
    const int m_elementHeight, m_elementWidth;

    void calculateDimensions();
};

#endif // EDITTABLEWIDGET_H
