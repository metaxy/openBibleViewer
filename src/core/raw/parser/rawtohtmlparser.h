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
#ifndef RAWTOHTMLPARSER_H
#define RAWTOHTMLPARSER_H
#include <QtCore/QString>
#include "src/core/raw/bookblock.h"
#include "src/core/raw/brfragment.h"
#include "src/core/raw/captionblock.h"
#include "src/core/raw/chapterblock.h"
#include "src/core/raw/divblock.h"
#include "src/core/raw/gramblock.h"
#include "src/core/raw/noteblock.h"
#include "src/core/raw/prologblock.h"
#include "src/core/raw/remarksblock.h"
#include "src/core/raw/styleblock.h"
#include "src/core/raw/supblock.h"
#include "src/core/raw/textfragment.h"
#include "src/core/raw/verseblock.h"
#include "src/core/raw/xreffragment.h"
#include "src/core/raw/mediablock.h"
#include "src/core/settings/moduledisplaysettings.h"
#include <QtCore/QSharedPointer>

class RawToHtmlParser
{
public:
    RawToHtmlParser(QSharedPointer<ModuleDisplaySettings> displaySettings);

    virtual QString parseBook(BookBlock *b);
    virtual QString parseBr(BrFragment *b);
    virtual QString parseCaption(CaptionBlock *b);
    virtual QString parseChapter(ChapterBlock *b);
    virtual QString parseMedia(MediaBlock *b);
    virtual QString parseDiv(DivBlock *b);
    virtual QString parseGram(GramBlock *b);
    virtual QString parseNote(NoteBlock *b);
    virtual QString parseProlog(PrologBlock *b);
    virtual QString parseRemarks(RemarksBlock *b);
    virtual QString parseStyle(StyleBlock *b);
    virtual QString parseSup(SupBlock *b);
    virtual QString parseText(TextFragment *b);
    virtual QString parseVerse(VerseBlock *b);
    virtual QString parseXRef(XRefFragment *b);

    QString parse(RBlock *block);
protected:
     QSharedPointer<ModuleDisplaySettings> m_displaySettings;
};

#endif // RAWTOHTMLPARSER_H
