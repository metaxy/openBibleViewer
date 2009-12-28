#include "bibledisplay.h"

BibleDisplay::BibleDisplay()
{
}
void BibleDisplay::setHtml(QString text)
{
    m_text = text;
    emit newHtml(text);
}
