#include "versemodule.h"

VerseModule::VerseModule()
{
}
VerseModule::~VerseModule()
{

}
void VerseModule::setBibleDisplaySettings(BibleDisplaySettings *bibleDisplaySettings)
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

SearchQuery VerseModule::lastSearchQuery() const
{
    return m_lastSearchQuery;
}
void VerseModule::search(SearchQuery query, SearchResult *result)
{

}

QString VerseModule::bookName(const int &bookID, bool preferShort)
{
    return QString();
}
QStringList VerseModule::chapterNames()
{
    return QStringList();
}
int VerseModule::booksCount() const
{
    return 0;
}
int VerseModule::chaptersCount() const
{
    return 0;
}
