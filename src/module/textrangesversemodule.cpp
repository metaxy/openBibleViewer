#include "textrangesversemodule.h"

TextRangesVerseModule::TextRangesVerseModule()
{
    m_lastTextRanges = NULL;
}

void TextRangesVerseModule::setLastTextRanges(TextRanges *textRanges)
{
    m_lastTextRanges = textRanges;
}

TextRanges *TextRangesVerseModule::lastTextRanges() const
{
    return m_lastTextRanges;
}
