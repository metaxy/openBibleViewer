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
#include "zefania.h"
#include "src/core/verse/reftext.h"
#include "src/core/link/strongurl.h"
#include "config.h"
#include <QTextDocument> //!!!
#include <QXmlStreamReader>

#include "src/core/obvcore.h" //???
#include "src/core/raw/rmetadata.h"
#include <src/core/raw/chapterblock.h>
#include <src/core/raw/textfragment.h>
#include "src/core/raw/blocktools.h"
#include "src/core/raw/parser/rawtotextrangeparser.h"

ZefaniaXmlReader::ZefaniaXmlReader(const QString &fileName, QSharedPointer<Versification> v11n) :
    m_fileName(fileName), m_versification(v11n), m_xml(nullptr), m_file(nullptr)
{
}
ZefaniaXmlReader::ZefaniaXmlReader(const QString &fileName) :
    m_fileName(fileName), m_xml(nullptr), m_file(nullptr)
{
}
ZefaniaXmlReader::~ZefaniaXmlReader()
{
    if(m_xml != nullptr) {
        delete m_xml;
        m_xml = nullptr;
    }
    if(m_file != nullptr) {
        delete m_file;
        m_file = nullptr;
    }
}

void ZefaniaXmlReader::genStrongsPrefix(const int bookID)
{
    if(m_versification->bookCount() == 66) {
        if(bookID < 39) {
            m_strongAdd = "H";
        } else {
            m_strongAdd = "G";
        }
    } else if(m_versification->name.endsWith("-nt")) {
        m_strongAdd = "G";
    } else if(m_versification->name.endsWith("-ot")) {
        m_strongAdd = "H";
    } else if(m_versification->bookCount() == 27) {
        m_strongAdd = "G";
    } else if(m_versification->bookCount() == 39) {
        m_strongAdd = "H";
    }
}

MetaInfo ZefaniaXmlReader::readMetaInfo()
{
    DEBUG_FUNC_NAME;
    MetaInfo ret;
    create();
    if(m_xml->readNextStartElement()) {
        if(cmp(m_xml->name(), AnyTop)) {
            if(m_xml->attributes().value("type") == "x-bible") {
                ret.setContent(ModuleTools::BibleContent);
                ret.setDefaultModule(ModuleTools::DefaultBibleModule);
            }
            while(m_xml->readNextStartElement()) {
                if(cmp(m_xml->name(), Information)) {
                    ret = readMetaInfo(ret);
                    break;
                } else {
                    m_xml->skipCurrentElement();
                }
            }
        }
    }
    destroy();
    return ret;
}
void ZefaniaXmlReader::create()
{
   // DEBUG_FUNC_NAME;
    if(m_xml != nullptr) {
        delete m_xml;
        m_xml = nullptr;
    }
    if(m_file == nullptr) {
        m_file = new QFile(m_fileName);
    }
    m_file->open(QIODevice::ReadOnly | QIODevice::Text);
    m_xml = new QXmlStreamReader(m_file);
}
void ZefaniaXmlReader::destroy()
{
    //DEBUG_FUNC_NAME;
    if(m_xml != nullptr) {
        delete m_xml;
        m_xml = nullptr;
    }
    if(m_file != nullptr) {
        m_file->close();
    }

}
TextFragment* ZefaniaXmlReader::rawReadText(quint32 parent)
{
    //DEBUG_FUNC_NAME;
    TextFragment *t = (TextFragment*) BlockTools::create(m_idGen.next(), parent, RMetaData::TextFragment);
    t->text = m_xml->text().toString().toHtmlEscaped();
    return t;
}
TextFragment* ZefaniaXmlReader::rawReadChildText(quint32 parent)
{
    //DEBUG_FUNC_NAME;
    TextFragment *t = (TextFragment*) BlockTools::create(m_idGen.next(), parent, RMetaData::TextFragment);
    t->text = m_xml->readElementText(QXmlStreamReader::IncludeChildElements).toHtmlEscaped();
    return t;
}
BookBlock ZefaniaXmlReader::readBookBlock(const int bookID)
{
    //DEBUG_FUNC_NAME;
    genStrongsPrefix(bookID);
    create();
    if(m_xml->readNextStartElement()) {
        if(cmp(m_xml->name(), XmlBible)) {
            while(m_xml->readNextStartElement()) {
                if(cmp(m_xml->name(), Biblebook)) {
                    if(m_xml->attributes().value("bnumber").toString().toInt() == bookID + 1) {
                        BookBlock *block = rawReadBook(0);
                        destroy();
                        return *block;
                    } else {
                        m_xml->skipCurrentElement();
                    }
                } else {
                    m_xml->skipCurrentElement();
                }
            }
        }
    }
    destroy();
}
ChapterBlock ZefaniaXmlReader::readChapterBlock(const int bookID, const int chapterID)
{
    //DEBUG_FUNC_NAME;
    genStrongsPrefix(bookID);
    create();
    if(m_xml->readNextStartElement()) {
        if(cmp(m_xml->name(), XmlBible)) {
            while(m_xml->readNextStartElement()) {
                if(cmp(m_xml->name(), Biblebook)) {
                    if(m_xml->attributes().value("bnumber").toString().toInt() == bookID + 1) {
                        while(m_xml->readNextStartElement()) {
                            if(cmp(m_xml->name(), Chapter)) {
                                if(m_xml->attributes().value("cnumber").toString().toInt() == chapterID + 1) {
                                    ChapterBlock *chapter = rawReadChapter(0);
                                    destroy();
                                    return *chapter;
                                } else {
                                    m_xml->skipCurrentElement();
                                }
                            } else {
                                m_xml->skipCurrentElement();
                            }
                        }
                    } else {
                        m_xml->skipCurrentElement();
                    }
                } else {
                    m_xml->skipCurrentElement();
                }
            }
        }
    }
    destroy();
    return ChapterBlock(-1, RMetaData(-1, RMetaData::Block));
}
MetaInfo ZefaniaXmlReader::readMetaInfo(MetaInfo ret)
{
    DEBUG_FUNC_NAME;
    myDebug() << "second";
    while(m_xml->readNextStartElement()) {
        myDebug() << m_xml->name();
        if(m_xml->name() == QLatin1String("publisher")) {
            ret.publisher = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("contributors")) {
            ret.contributors = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("date")) {
            ret.date = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("type")) {
            ret.type = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("format")) {
            ret.format = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("identifier")) {
            ret.identifier = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("source")) {
            ret.source = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("language")) {
            ret.language = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("coverage")) {
            ret.coverage = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("rights")) {
            ret.rights = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("subject")) {
            ret.subject = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("description")) {
            ret.description = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("creator")) {
            ret.creator = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("title")) {
            ret.setName(m_xml->readElementText(QXmlStreamReader::IncludeChildElements));
            myDebug() << ret.name();
        } else {
            m_xml->skipCurrentElement();
        }
    }
    return ret;
}


bool ZefaniaXmlReader::cmp(const QStringRef &r, const QString &s)
{
    return r == s || r == s.toLower();
}

BookBlock* ZefaniaXmlReader::rawReadBook(quint32 parent)
{
    //DEBUG_FUNC_NAME;
    const int bookNumber = m_xml->attributes().value("bnumber").toString().toInt() - 1;
    myDebug() << bookNumber;
    quint32 id = m_idGen.next();
    BookBlock *book = (BookBlock*) BlockTools::create(id, parent, RMetaData::BookBlock);
    book->bookNumber = bookNumber;
    while(m_xml->readNextStartElement()) {
        if(cmp(m_xml->name(), Chapter)) {
            book->add(rawReadChapter(id));
        } else {
            m_xml->skipCurrentElement();
        }
    }
    return book;
}

ChapterBlock* ZefaniaXmlReader::rawReadChapter(quint32 parent)
{
    //DEBUG_FUNC_NAME;
    const int chapterNumber = m_xml->attributes().value("cnumber").toString().toInt() - 1;
    quint32 id = m_idGen.next();
    ChapterBlock *chapter = (ChapterBlock *)BlockTools::create(id, parent, RMetaData::ChapterBlock);
    chapter->chapterNumber = chapterNumber;

    while(m_xml->readNextStartElement()) {
        if(cmp(m_xml->name(), Vers)) {
            chapter->add(rawReadVerse(parent));
        } else if(cmp(m_xml->name(), Prolog)) {
            chapter->add(rawReadProlog(parent));
        } else if(cmp(m_xml->name(), Caption)) {
            chapter->add(rawReadCaption(parent));
        } else if(cmp(m_xml->name(), Remark)) {
            chapter->add(rawReadRemarks(parent));
        } else if(cmp(m_xml->name(), XRef)) {
            chapter->add(rawReadXRef(parent));
        } else if(cmp(m_xml->name(), Media)) {
           chapter->add(rawReadMedia(parent));
        } else {
            m_xml->skipCurrentElement();
        }
    }
    return chapter;
}


VerseBlock* ZefaniaXmlReader::rawReadVerse(quint32 parent)
{
    //DEBUG_FUNC_NAME
    quint32 id = m_idGen.next();
    VerseBlock *verse = (VerseBlock*) BlockTools::create(id,parent,RMetaData::VerseBlock);
    const int verseNumber = m_xml->attributes().value("vnumber").toString().toInt() - 1;
    verse->verseNumber = verseNumber;

    while(!m_xml->atEnd()) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), Vers)))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            verse->add(rawReadText(id));
        } else if(cmp(m_xml->name(), Style)) {
            verse->add(rawReadStyle(parent));
        } else if(cmp(m_xml->name(), Note)) {
          verse->add(rawReadNote(parent));
        } else if(cmp(m_xml->name(), Br)) {
            verse->add(rawReadBr(parent));
        } else if(cmp(m_xml->name(), "DIV")) {
           verse->add(rawReadDiv(parent));
        } else if(cmp(m_xml->name(), Gram)) {
           verse->add(rawReadGram(parent));
        } else if(cmp(m_xml->name(), Sup)) {
            verse->add(rawReadSup(parent));
        } else if(cmp(m_xml->name(), XRef)) {
            verse->add(rawReadXRef(parent));
        } else {
            verse->add(rawReadChildText(id));
        }
    }
    return verse;
}
PrologBlock* ZefaniaXmlReader::rawReadProlog(quint32 parent)
{
    //DEBUG_FUNC_NAME;
    quint32 id = m_idGen.next();
    PrologBlock *prolog = (PrologBlock*) BlockTools::create(id,parent,RMetaData::PrologBlock);
    while(!m_xml->atEnd()) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), Prolog)))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            prolog->add(rawReadText(id));
        } else if(cmp(m_xml->name(), Style)) {
            prolog->add(rawReadStyle(parent));
        } else if(cmp(m_xml->name(), Br)) {
            prolog->add(rawReadBr(parent));
        } else if(cmp(m_xml->name(), Gram)) {
            prolog->add(rawReadGram(parent));
        } else if(cmp(m_xml->name(), XRef)) {
            prolog->add(rawReadXRef(parent));
        } else if(cmp(m_xml->name(), Sup)) {
            prolog->add(rawReadSup(parent));
        }  else {
            prolog->add(rawReadChildText(id));
        }
    }

    return prolog;
}
NoteBlock* ZefaniaXmlReader::rawReadNote(quint32 parent)
{
    //DEBUG_FUNC_NAME;
    quint32 id = m_idGen.next();
    NoteBlock *note = (NoteBlock*) BlockTools::create(id,parent,RMetaData::NoteBlock);
    while(!m_xml->atEnd()) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), Note)))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            note->add(rawReadText(id));
        } else if(cmp(m_xml->name(), Style)) {
            note->add(rawReadStyle(parent));
        } else if(cmp(m_xml->name(), Br)) {
            note->add(rawReadBr(parent));
        } else if(cmp(m_xml->name(), Gram)) {
            note->add(rawReadGram(parent));
        } else if(cmp(m_xml->name(), XRef)) {
            note->add(rawReadXRef(parent));
        } else if(cmp(m_xml->name(), Sup)) {
            note->add(rawReadSup(parent));
        }  else {
            note->add(rawReadChildText(id));
        }
    }

    return note;
}

CaptionBlock* ZefaniaXmlReader::rawReadCaption(quint32 parent)
{
    //DEBUG_FUNC_NAME;
    quint32 id = m_idGen.next();
    CaptionBlock *caption = (CaptionBlock *)BlockTools::create(id,parent,RMetaData::CaptionBlock);
    while(!m_xml->atEnd()) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), Caption)))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            caption->add(rawReadText(id));
        } else if(cmp(m_xml->name(), Style)) {
            caption->add(rawReadStyle(parent));
        } else if(cmp(m_xml->name(), Br)) {
            caption->add(rawReadBr(parent));
        } else if(cmp(m_xml->name(), Gram)) {
            caption->add(rawReadGram(parent));
        } else if(cmp(m_xml->name(), XRef)) {
            caption->add(rawReadXRef(parent));
        } else if(cmp(m_xml->name(), Sup)) {
            caption->add(rawReadSup(parent));
        }  else {
            caption->add(rawReadChildText(id));
        }
    }
    return caption;
}

RemarksBlock* ZefaniaXmlReader::rawReadRemarks(quint32 parent)
{
    //DEBUG_FUNC_NAME;
    quint32 id = m_idGen.next();
    RemarksBlock *remarks = (RemarksBlock *)BlockTools::create(id,parent,RMetaData::RemarksBlock);
    while(!m_xml->atEnd()) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), Remark)))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            remarks->add(rawReadText(id));
        } else if(cmp(m_xml->name(), Style)) {
            remarks->add(rawReadStyle(parent));
        } else if(cmp(m_xml->name(), Br)) {
            remarks->add(rawReadBr(parent));
        } else if(cmp(m_xml->name(), Gram)) {
            remarks->add(rawReadGram(parent));
        } else if(cmp(m_xml->name(), XRef)) {
            remarks->add(rawReadXRef(parent));
        } else if(cmp(m_xml->name(), Sup)) {
            remarks->add(rawReadSup(parent));
        }  else {
            remarks->add(rawReadChildText(id));
        }
    }

    return remarks;
}

XRefFragment* ZefaniaXmlReader::rawReadXRef(quint32 parent)
{
    //DEBUG_FUNC_NAME;
    quint32 id = m_idGen.next();
    XRefFragment *xref = (XRefFragment *)BlockTools::create(id,parent,RMetaData::XRefFragment);
    xref->aix = m_xml->attributes().value("aix").toString();
    xref->fscope = m_xml->attributes().value("fscope").toString();
    xref->vef = m_xml->attributes().value("vef").toString().toInt();
    while(m_xml->readNextStartElement()) {
        xref->add(rawReadText(id));
    }
    return xref;
}

MediaBlock* ZefaniaXmlReader::rawReadMedia(quint32 parent)
{
    //DEBUG_FUNC_NAME;
    quint32 id = m_idGen.next();
    MediaBlock *media = (MediaBlock *)BlockTools::create(id,parent,RMetaData::MediaBlock);

    while(m_xml->readNextStartElement()) {
        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            TextFragment *t = (TextFragment*)BlockTools::create(m_idGen.next(), id, RMetaData::TextFragment);
            t->text = m_xml->text().toString().toHtmlEscaped();
            media->add(t);
        } else {
            m_xml->skipCurrentElement();
        }
    }
    return media;
}
BrFragment* ZefaniaXmlReader::rawReadBr(quint32 parent)
{
    //DEBUG_FUNC_NAME;
    quint32 id = m_idGen.next();
    BrFragment *br = (BrFragment *)BlockTools::create(id, parent, RMetaData::BrFragment);
    const QStringRef art = m_xml->attributes().value("art");
    m_xml->skipCurrentElement(); // todo: do we really need this?
    if(art == "x-p")
        br->type = BrFragment::PageBreak;
    else if(art == "x-nl")
        br->type = BrFragment::NewLine;
    return br;
}

StyleBlock* ZefaniaXmlReader::rawReadStyle(quint32 parent)
{
    //DEBUG_FUNC_NAME;
    quint32 id = m_idGen.next();
    StyleBlock *style = (StyleBlock *)BlockTools::create(id,parent,RMetaData::StyleBlock);
    style->css = m_xml->attributes().value("css").toString();
    style->fs = m_xml->attributes().value("fs").toString();
    style->id = m_xml->attributes().value("id").toString();

    while(!m_xml->atEnd()) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), Style)))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            style->add(rawReadText(id));
        } else if(cmp(m_xml->name(), Style)) {
            style->add(rawReadStyle(parent));
        } else if(cmp(m_xml->name(), Br)) {
            style->add(rawReadBr(parent));
        } else if(cmp(m_xml->name(), Gram)) {
            style->add(rawReadGram(parent));
        } else if(cmp(m_xml->name(), Sup)) {
            style->add(rawReadSup(parent));
        }  else {
            style->add(rawReadChildText(id));
        }
    }
    return style;
}

GramBlock* ZefaniaXmlReader::rawReadGram(quint32 parent)
{
    //DEBUG_FUNC_NAME;
    quint32 id = m_idGen.next();
    GramBlock *gram = (GramBlock *)BlockTools::create(id, parent, RMetaData::GramBlock);
    gram->rmac = m_xml->attributes().value("rmac").toString();
    gram->strong = m_xml->attributes().value("str").toString();

    if(!(gram->strong.startsWith("H") || gram->strong.startsWith("G"))) {
        gram->strong.prepend(m_strongAdd);
    }

    while(!m_xml->atEnd()) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), Gram))) break;
 
        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            gram->add(rawReadText(id));
        } else if(cmp(m_xml->name(), Style)) {
            gram->add(rawReadStyle(parent));
        } else if(cmp(m_xml->name(), Br)) {
            gram->add(rawReadBr(parent));
        } else if(cmp(m_xml->name(), Gram)) {
            gram->add(rawReadGram(parent));
        } else if(cmp(m_xml->name(), Sup)) {
            gram->add(rawReadSup(parent));
        } else {
            gram->add(rawReadChildText(id));
        }
    }
    return gram;
}

SupBlock* ZefaniaXmlReader::rawReadSup(quint32 parent)
{
    //DEBUG_FUNC_NAME;
    quint32 id = m_idGen.next();
    SupBlock *sup = (SupBlock *)BlockTools::create(id, parent, RMetaData::SupBlock);
    const QStringRef art = m_xml->attributes().value("art");
    if(art == "x-sub") {
        sup->type = SupBlock::Sub;
    } else if(art == "x-sup") {
        sup->type = SupBlock::Sup;
    } else {
        sup->type = SupBlock::Sub;
    }
    while(!m_xml->atEnd()) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), Sup)))
            break;
        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            sup->add(rawReadText(id));
        } else if(cmp(m_xml->name(), Style)) {
            sup->add(rawReadStyle(parent));
        } else if(cmp(m_xml->name(), Gram)) {
            sup->add(rawReadGram(parent));
        } else {
            sup->add(rawReadChildText(id));
        }
    }
    return sup;
}
DivBlock* ZefaniaXmlReader::rawReadDiv(quint32 parent)
{
    //DEBUG_FUNC_NAME;
    quint32 id = m_idGen.next();
    DivBlock *div = (DivBlock *)BlockTools::create(id,parent,RMetaData::DivBlock);

    while(m_xml->readNextStartElement()) {
        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            TextFragment *t = (TextFragment*)BlockTools::create(m_idGen.next(), id, RMetaData::TextFragment);
            t->text = m_xml->text().toString().toHtmlEscaped();
            div->add(t);
        } else if(cmp(m_xml->name(), Style)) {
            div->add(rawReadStyle(parent));
        } else if(cmp(m_xml->name(), Gram)) {
            div->add(rawReadGram(parent));
        } else if(cmp(m_xml->name(), Note)) {
            div->add(rawReadDiv(parent));
        }  else {
            m_xml->skipCurrentElement();
        }
    }
    return div;
}
bool ZefaniaXmlReader::cmp(const QStringRef& r, const ZefaniaXmlReader::TagName n)
{
    switch (n)
    {
        case AnyTop:
            return r == "x" || r == "XMLBIBLE" || r == "xmlbible" || r == "dictionary" || r == "DICTIONARY";
        case Biblebook:
            return r == "b" || r == "BIBLEBOOK" || r == "biblebook";
        case Br:
            return r == "br" || r == "BR";
        case Caption:
            return r == "ca" || r == "CAPTION" || r == "caption";
        case Chapter:
            return r == "c" || r == "CHAPTER" || r == "chapter";
        case Gram:
            return r == "g" || r == "gr" || r == "GRAM" || r == "gram";
        case Information:
            return r == "i" || r == "INFORMATION" || r == "information";
        case Media:
            return r == "m" || r == "MEDIA" || r == "media";
        case Note:
            return r == "n" || r == "NOTE" || r == "note";
        case Prolog:
            return r == "p" || r == "PROLOG" || r == "prolog";
        case Remark:
            return r == "r" || r == "REMARK" || r == "remark";
        case Style:
            return r == "s" || r == "st" || r == "STYLE" || r == "style";
        case Sup:
            return r == "SUP" || r == "sup";
        case Vers:
           return r == "v" || r == "VERS" || r == "vers";
        case XmlBible:
            return r == "x" || r == "XMLBIBLE" || r == "xmlbible";
        case XRef:
            return r == "xr" || r == "XREF" || r == "xref";
    }
    return "";
}
