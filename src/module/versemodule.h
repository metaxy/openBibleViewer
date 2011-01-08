#ifndef VERSEMODULE_H
#define VERSEMODULE_H
#include "src/core/verse/ranges.h"
#include "src/core/verse/range.h"
#include "src/core/verse/textrange.h"
#include "src/core/verse/textranges.h"
#include "src/core/verse/versification.h"
#include "src/module/simplemoduleclass.h"
#include "src/core/settings/bibledisplaysettings.h"
/**
  * VerseModule is an abstract class for modules classes like Bible or StudyNotes which are based on a versification.
  * That means it has books, chapters and verse.
  */
class VerseModule : public SimpleModuleClass
{
public:
    VerseModule();
    virtual ~VerseModule();
    virtual void setBibleDisplaySettings(BibleDisplaySettings *bibleDisplaySettings);
    virtual TextRanges readRanges(const Ranges &ranges, bool ignoreModuleID = false);
    virtual TextRange readRange(const Range &range, bool ignoreModuleID = false);

    void setLastTextRanges(TextRanges *textRanges);
    TextRanges *lastTextRanges() const;

    Versification *versification() const;

    /**
      * Search in the current bible. Saves the results int the second argument (*result).
      */
    virtual void search(SearchQuery query, SearchResult *result);
    SearchQuery lastSearchQuery() const;
    virtual QString bookName(const int &bookID, bool preferShort = false);
    virtual QStringList chapterNames();
    virtual int booksCount() const;
    virtual int chaptersCount() const;
protected:
    SearchQuery m_lastSearchQuery;
    Versification *m_versification;
    TextRanges *m_lastTextRanges;
};

#endif // VERSEMODULE_H
