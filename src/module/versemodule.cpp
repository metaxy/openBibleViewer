#include "versemodule.h"

VerseModule::VerseModule()
{
}
VerseModule::~VerseModule()
{

}
void VerseModule::setLastTextRanges(TextRanges *textRanges)
{
    m_lastTextRanges = textRanges;
}
TextRanges *VerseModule::lastTextRanges() const
{
    return m_lastTextRanges;
}

Versification *VerseModule::versification() const
{
    return m_versification;
}
TextRanges VerseModule::readRanges(const Ranges &ranges, bool ignoreModuleID)
{
    return TextRanges();
}

TextRange VerseModule::readRange(const Range &range, bool ignoreModuleID)
{
    return TextRange();
}

