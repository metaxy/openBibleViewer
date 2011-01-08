#ifndef VERSEMODULE_H
#define VERSEMODULE_H
#include "src/core/bible/ranges.h"
#include "src/core/bible/range.h"
#include "src/core/bible/textrange.h"
#include "src/core/bible/textranges.h"
#include "src/core/bible/versification.h"
#include "src/module/simplemoduleclass.h"
/**
  * VerseModule is an abstract class for modules classes like Bible or StudyNotes which are based on a versification.
  * That means it has books, chapters and verse.
  */
class VerseModule : public SimpleModuleClass
{
public:
    VerseModule();
    virtual ~VerseModule();
    virtual TextRanges readRanges(const Ranges &ranges, bool ignoreModuleID = false);
    virtual TextRange readRange(const Range &range, bool ignoreModuleID = false);

    void setLastTextRanges(TextRanges *textRanges);
    TextRanges *lastTextRanges() const;

    Versification *versification() const;
protected:
    Versification *m_versification;
    TextRanges *m_lastTextRanges;
};

#endif // VERSEMODULE_H
