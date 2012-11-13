#include "rawtotextrangeparser.h"
#include "src/core/verse/verse.h"
RawToTextRangeParser::RawToTextRangeParser()
{
}
TextRange RawToTextRangeParser::toTextRange(ChapterBlock* b)
{
    TextRange range;
    range.setChapterID(b->chapterNumber);
    foreach(RBlock *c, b->children) {
        if(c->metaData().type == RMetaData::VerseBlock) {
            VerseBlock *vb = (VerseBlock*) c;
            Verse v(vb->verseNumber, parseVerse(vb));
            range.addVerse(v);
        }
    }
    return range;
}

