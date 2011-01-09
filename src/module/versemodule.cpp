#include "versemodule.h"

VerseModule::VerseModule()
{
    m_versification = 0;
    m_lastTextRanges = 0;
}
VerseModule::~VerseModule()
{

}
void VerseModule::setBibleDisplaySettings(BibleDisplaySettings *bibleDisplaySettings)
{
    myWarning() << "calling VerseModule";
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
    myWarning() << "calling VerseModule";
    return TextRanges();
}

TextRange VerseModule::readRange(const Range &range, bool ignoreModuleID)
{
    myWarning() << "calling VerseModule";
    return TextRange();
}

SearchQuery VerseModule::lastSearchQuery() const
{
    return m_lastSearchQuery;
}
void VerseModule::search(SearchQuery query, SearchResult *result)
{
myWarning() << "calling VerseModule";
}

QString VerseModule::bookName(const int &bookID, bool preferShort)
{
    myWarning() << "calling VerseModule";
    return QString();
}
QStringList VerseModule::chapterNames()
{
    myWarning() << "calling VerseModule";
    return QStringList();
}
int VerseModule::booksCount() const
{
    myWarning() << "calling VerseModule";
    return 0;
}
int VerseModule::chaptersCount() const
{
    myWarning() << "calling VerseModule";
    return 0;
}
