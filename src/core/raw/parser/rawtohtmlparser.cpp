#include "rawtohtmlparser.h"
#include <typeinfo>
#include "src/core/dbghelper.h"
#include "src/core/raw/rblock.h"
#include "src/core/link/strongurl.h"
#include "src/core/moduletools.h"
RawToHtmlParser::RawToHtmlParser(QSharedPointer<ModuleDisplaySettings> displaySettings) : m_displaySettings(displaySettings)
{
}
QString RawToHtmlParser::parseBook(BookBlock *b)
{
    QString ret;
    const QString pre = "<div type='book' bookNumber='" + QString::number(b->bookNumber) + ">";
    const QString post = "</div>";
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return pre+ret+post;
}
QString RawToHtmlParser::parseChapter(ChapterBlock *b)
{
    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
}

QString RawToHtmlParser::parseVerse(VerseBlock *b)
{
    QString ret("<div type='verse' verseNumber='"+QString::number(b->verseNumber)+"'>");
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret+"</div>";
}
QString RawToHtmlParser::parseBr(BrFragment *b)
{
    if(b->type == BrFragment::NewLine)
        return "<br />";
    if(b->type == BrFragment::PageBreak)
        return "<div class=\"pageBreak\"></div>";
    return "<br />";
}

QString RawToHtmlParser::parseCaption(CaptionBlock *b)
{
    QString ret("<h3>");
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret + "</h3>";
}


QString RawToHtmlParser::parseDiv(DivBlock *b)
{
    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
}

QString RawToHtmlParser::parseGram(GramBlock *b)
{
    QString pre,post,ret;
    if(m_displaySettings->showStrong() && !b->strong.isEmpty()) {
        StrongUrl url;
        url.fromText(b->strong);
        if(m_displaySettings->showStrongInline()) {
            pre.prepend("<a class=\"inlinestronglink\" href=\""+url.toString()+"\" title=\""+ b->strong +"\">");
            post.append("</a>");
        } else {
            post += "<a class=\"stronglink\" href=\""+url.toString()+"\" title=\""+ b->strong +"\">"
                    + b->strong
                    + "</a>";
        }
    }
    if(m_displaySettings->showRMAC() && !b->rmac.isEmpty()) {
        post += "<a class=\"rmaclink\" href=\"" + ModuleTools::rmacScheme + b->rmac + "\">" + b->rmac + "</a>";
    }

    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return pre + ret + post;
}

QString RawToHtmlParser::parseNote(NoteBlock *b)
{
    if(!m_displaySettings->showStudyNotes())
        return QString();

    QString ret("<span class =\"studynote\">");
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret + "</span>";
}

QString RawToHtmlParser::parseProlog(PrologBlock *b)
{
    if(!m_displaySettings->showProlog())
        return QString();
    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
}

QString RawToHtmlParser::parseRemarks(RemarksBlock *b)
{
    //if(!m_displaySettings->s)
    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
}

QString RawToHtmlParser::parseStyle(StyleBlock *b)
{
    QString ret("<span style=\"" + b->css + "\">");
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret + "</span>";
}

QString RawToHtmlParser::parseSup(SupBlock *b)
{
    QString pre;
    QString post("</span>");
    if(b->type == SupBlock::Sub) {
        pre = "<span class =\"sub\">";
    } else if(b->type == SupBlock::Sup) {
        pre = "<span class =\"sup\">";
    } else {
        post = "";
    }

    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return pre + ret + post;
}

QString RawToHtmlParser::parseText(TextFragment *b)
{
    return b->text;
}

QString RawToHtmlParser::parseXRef(XRefFragment *b)
{
    return b->aix;
}
QString RawToHtmlParser::parseMedia(MediaBlock *b)
{
    if(!m_displaySettings->showMedia())
        return QString();
    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
}
QString RawToHtmlParser::parse(RBlock *block)
{
    switch(block->metaData().type) {
        case RMetaData::BookBlock:
            return parseBook((BookBlock*)block);
            break;
        case RMetaData::BrFragment:
            return parseBr((BrFragment*)block);
            break;
        case RMetaData::CaptionBlock:
            return parseCaption((CaptionBlock*)block);
            break;
        case RMetaData::ChapterBlock:
            return parseChapter((ChapterBlock*)block);
            break;
        case RMetaData::DivBlock:
            return parseDiv((DivBlock*)block);
            break;
        case RMetaData::GramBlock:
            return parseGram((GramBlock*)block);
            break;
        case RMetaData::MediaBlock:
            return parseMedia((MediaBlock*)block);
            break;
        case RMetaData::NoteBlock:
            return parseNote((NoteBlock*)block);
            break;
        case RMetaData::PrologBlock:
            return parseProlog((PrologBlock*)block);
            break;
        case RMetaData::RemarksBlock:
            return parseRemarks((RemarksBlock*)block);
            break;
        case RMetaData::StyleBlock:
            return parseStyle((StyleBlock*)block);
            break;
        case RMetaData::SupBlock:
            return parseSup((SupBlock*)block);
            break;
        case RMetaData::TextFragment:
            return parseText((TextFragment*)block);
            break;
        case RMetaData::VerseBlock:
            return parseVerse((VerseBlock*)block);
            break;
        case RMetaData::XRefFragment:
            return parseXRef((XRefFragment*)block);
            break;
        default:
            return "";
    }
}
