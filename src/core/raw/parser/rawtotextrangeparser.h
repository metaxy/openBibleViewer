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
