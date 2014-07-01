#ifndef TEXTSELECTION_H
#define TEXTSELECTION_H

#include <QList>
#include <QChar>

class TextSelection
{
public:
    TextSelection();
    TextSelection(QList<QChar> text, int width, int height);
    TextSelection(TextSelection const& other);
    TextSelection & operator=(TextSelection const& other);

    TextSelection & rotate90();
    TextSelection & rotate180();
    TextSelection & rotate270();
    TextSelection & mirrorX();
    TextSelection & mirrorY();

    QList<QChar> text() const;
    int width() const;
    int height() const;

private:
    QList<QChar> m_text;
    int m_width;
    int m_height;
};

#endif // TEXTSELECTION_H
