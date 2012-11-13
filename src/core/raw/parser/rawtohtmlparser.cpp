#include "rawtohtmlparser.h"
#include <typeinfo>
#include "src/core/dbghelper.h"
#include "src/core/raw/rblock.h"
RawToHtmlParser::RawToHtmlParser()
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
    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
}
QString RawToHtmlParser::parseChapter(ChapterBlock *b)
{
    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
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
    QString ret;
    const QString pre = "<a href='strong://"+b->strong+"'>";
    const QString post = "</a>";
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return pre + ret + post ;
}

QString RawToHtmlParser::parseNote(NoteBlock *b)
{
    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
}

QString RawToHtmlParser::parseProlog(PrologBlock *b)
{
    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
}

QString RawToHtmlParser::parseRemarks(RemarksBlock *b)
{
    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
}

QString RawToHtmlParser::parseStyle(StyleBlock *b)
{
    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
}

QString RawToHtmlParser::parseSup(SupBlock *b)
{
    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
}

QString RawToHtmlParser::parseText(TextFragment *b)
{
    return b->text;
}

QString RawToHtmlParser::parseVerse(VerseBlock *b)
{
    QString ret;
    const QString pre = "<div type='verse' verseNumber='"+QString::number(b->verseNumber)+"'>";
    const QString post = "</div>";
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return pre+ret+post;
}

QString RawToHtmlParser::parseXRef(XRefFragment *b)
{
    return b->aix;
}
QString RawToHtmlParser::parseMedia(MediaBlock *b)
{
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
