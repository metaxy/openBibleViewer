#include "rawtotextrangeparser.h"
#include "src/core/verse/verse.h"
RawToTextRangeParser::RawToTextRangeParser(QSharedPointer<ModuleDisplaySettings> displaySettings) : RawToHtmlParser(displaySettings)
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

QString RawToTextRangeParser::parseVerse(VerseBlock *b)
{
    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
}
