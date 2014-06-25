#ifndef APPLICATIONCONSOLE_H
#define APPLICATIONCONSOLE_H

#include <QEvent>
#include <QPlainTextEdit>
#include <QString>

class ApplicationConsole : public QPlainTextEdit
{
    Q_OBJECT

signals:
    void characterEntered(QString character);
    void lineEntered(QString line);

public:
    explicit ApplicationConsole(QWidget *parent = 0);
    void appendHtml(const QString &html);
    void appendPlainText(const QString &text);

private:
    void keyPressEvent(QKeyEvent *keyEvent);
    int m_userInputStartPosition;
};

#endif // APPLICATIONCONSOLE_H
