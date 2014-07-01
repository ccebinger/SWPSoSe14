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
