#ifndef RAWTOTEXTRANGEPARSER_H
#define RAWTOTEXTRANGEPARSER_H
#include "src/core/raw/parser/rawtohtmlparser.h"
#include "src/core/verse/textrange.h"

class RawToTextRangeParser : public RawToHtmlParser
{
public:
    RawToTextRangeParser(QSharedPointer<ModuleDisplaySettings> displaySettings);
    QString parseVerse(VerseBlock *b);
    TextRange toTextRange(ChapterBlock *b);
private:
    //int m_chapterNumber;
    //int m_bookNumber;
};

#endif // RAWTOTEXTRANGEPARSER_H
