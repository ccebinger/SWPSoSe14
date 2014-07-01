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
    QList<QChar> m_text;
    int m_width;
    int m_height;
};

#endif // TEXTSELECTION_H
