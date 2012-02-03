#ifndef TEXTRANGESVERSEMODULE_H
#define TEXTRANGESVERSEMODULE_H
#include "src/module/versemodule.h"
#include "src/core/verse/textranges.h"
class TextRangesVerseModule : public VerseModule
{
public:
    TextRangesVerseModule();
    void setLastTextRanges(TextRanges *textRanges);
    /**
     * Returns the last computed TextRanges.
     */
    TextRanges *lastTextRanges() const;
protected:
    virtual TextRange readRange(const Range &range, bool ignoreModuleID = false) = 0;
    TextRanges *m_lastTextRanges;
};

#endif // TEXTRANGESVERSEMODULE_H
