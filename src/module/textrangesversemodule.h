/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger <metaxy@walger.name>
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
