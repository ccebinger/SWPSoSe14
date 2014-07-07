#include "TextSelection.h"

TextSelection::TextSelection()
{
    this->m_text = QList<QChar>();
    this->m_width = 0;
    this->m_height = 0;
}

TextSelection::TextSelection(QList<QChar> text, int width, int height)
{
    this->m_text = text;
    this->m_width = width;
    this->m_height = height;
}

TextSelection::TextSelection(TextSelection const& other)
{
    this->m_text = other.m_text;
    this->m_width = other.m_width;
    this->m_height = other.m_height;
}

TextSelection & TextSelection::operator=(TextSelection const& other)
{
    this->m_text = other.m_text;
    this->m_width = other.m_width;
    this->m_height = other.m_height;
    return *this;
}

QList<QChar> TextSelection::text() const
{
    return m_text;
}

int TextSelection::width() const
{
    return m_width;
}

int TextSelection::height() const
{
    return m_height;
}

TextSelection & TextSelection::rotate90()
{
    QList<QChar>* newText = new QList<QChar>();
    for(int x=0; x<this->width(); x++) {
        for(int y=this->height()-1; y>=0; y--) {
            QChar newChar = this->text().at(y*this->width()+x);
            if(newChar=='-') {
                newText->append(*new QChar('|'));
            } else if(newChar=='|') {
                newText->append(*new QChar('-'));
            } else if(newChar=='/') {
                newText->append(*new QChar('\\'));
            } else if(newChar=='\\') {
                newText->append(*new QChar('/'));
            } else if(newChar=='>') {
                newText->append(*new QChar('v'));
            } else if(newChar=='<') {
                newText->append(*new QChar('^'));
            } else if(newChar=='v') {
                newText->append(*new QChar('<'));
            } else if(newChar=='^') {
                newText->append(*new QChar('>'));
            } else {
                newText->append(newChar);
            }
        }
    }
    return *new TextSelection(*newText, this->height(), this->width());
}

TextSelection & TextSelection::rotate180()
{
    QList<QChar>* newText = new QList<QChar>();
    for(int y=this->height()-1; y>=0; y--) {
        for(int x=this->width()-1; x>=0; x--) {
            QChar newChar = this->text().at(y*this->width()+x);
            if(newChar=='>') {
                newText->append(*new QChar('<'));
            } else if(newChar=='<') {
                newText->append(*new QChar('>'));
            } else if(newChar=='v') {
                newText->append(*new QChar('^'));
            } else if(newChar=='^') {
                newText->append(*new QChar('v'));
            } else if(newChar==']') {
                newText->append(*new QChar('['));
            } else if(newChar==']') {
                newText->append(*new QChar('['));
            } else if(newChar=='[') {
                newText->append(*new QChar(']'));
            } else if(newChar==']') {
                newText->append(*new QChar('['));
            } else if(newChar=='(') {
                newText->append(*new QChar(')'));
            } else if(newChar==')') {
                newText->append(*new QChar('('));
            } else if(newChar=='{') {
                newText->append(*new QChar('}'));
            } else if(newChar=='}') {
                newText->append(*new QChar('{'));
            } else {
                newText->append(newChar);
            }
        }
    }
    return *new TextSelection(*newText, this->width(), this->height());
}

TextSelection & TextSelection::rotate270()
{
    QList<QChar>* newText = new QList<QChar>();
    for(int x=this->width()-1; x>=0; x--) {
        for(int y=0; y<this->height(); y++) {
            QChar newChar = this->text().at(y*this->width()+x);
            if(newChar=='-') {
                newText->append(*new QChar('|'));
            } else if(newChar=='|') {
                newText->append(*new QChar('-'));
            } else if(newChar=='/') {
                newText->append(*new QChar('\\'));
            } else if(newChar=='\\') {
                newText->append(*new QChar('/'));
            } else if(newChar=='>') {
                newText->append(*new QChar('^'));
            } else if(newChar=='<') {
                newText->append(*new QChar('v'));
            } else if(newChar=='v') {
                newText->append(*new QChar('>'));
            } else if(newChar=='^') {
                newText->append(*new QChar('<'));
            } else {
                newText->append(newChar);
            }
        }
    }
    return *new TextSelection(*newText, this->height(), this->width());
}

TextSelection & TextSelection::mirrorX()
{
    QList<QChar>* newText = new QList<QChar>();
    for(int y=this->height()-1; y>=0; y--) {
        for(int x=0; x<this->width(); x++) {
            QChar newChar = this->text().at(y*this->width()+x);
            if(newChar=='/') {
                newText->append(*new QChar('\\'));
            } else if(newChar=='\\') {
                newText->append(*new QChar('/'));
            } else if(newChar=='^') {
                newText->append(*new QChar('v'));
            } else if(newChar=='v') {
                newText->append(*new QChar('^'));
            } else {
                newText->append(newChar);
            }
        }
    }
    return *new TextSelection(*newText, this->width(), this->height());
}

TextSelection & TextSelection::mirrorY()
{
    QList<QChar>* newText = new QList<QChar>();
    for(int y=0; y<this->height(); y++) {
        for(int x=this->width()-1; x>=0; x--) {
            QChar newChar = this->text().at(y*this->width()+x);
            if(newChar=='/') {
                newText->append(*new QChar('\\'));
            } else if(newChar=='\\') {
                newText->append(*new QChar('/'));
            } else if(newChar=='>') {
                newText->append(*new QChar('<'));
            } else if(newChar=='<') {
                newText->append(*new QChar('>'));
            } else if(newChar==']') {
                newText->append(*new QChar('['));
            } else if(newChar==']') {
                newText->append(*new QChar('['));
            } else if(newChar=='[') {
                newText->append(*new QChar(']'));
            } else if(newChar==']') {
                newText->append(*new QChar('['));
            } else if(newChar=='(') {
                newText->append(*new QChar(')'));
            } else if(newChar==')') {
                newText->append(*new QChar('('));
            } else if(newChar=='{') {
                newText->append(*new QChar('}'));
            } else if(newChar=='}') {
                newText->append(*new QChar('{'));
            } else {
                newText->append(newChar);
            }
        }
    }
    return *new TextSelection(*newText, this->width(), this->height());
}

