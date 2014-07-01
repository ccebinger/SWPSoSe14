#ifndef FILESELECTLINEEDIT_H
#define FILESELECTLINEEDIT_H

#include <QLineEdit>
#include <QFileInfo>

class FileSelectLineEdit : public QLineEdit
{
    Q_OBJECT

signals:
    void fileSelected(QFileInfo path);

public:
    explicit FileSelectLineEdit(QWidget *parent = NULL);
    explicit FileSelectLineEdit(QFileInfo info, QWidget *parent = NULL);
    ~FileSelectLineEdit();

    void setFileInfo(const QFileInfo &info);
    QFileInfo getFileInfo() const;

private:
    void mouseDoubleClickEvent(QMouseEvent *mouseEvent);
    void setText(const QString &text);

    QFileInfo m_currentFileInfo;
};

#endif // FILESELECTLINEEDIT_H
