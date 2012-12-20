/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
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
