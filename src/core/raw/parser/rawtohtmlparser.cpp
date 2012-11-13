#include "rawtohtmlparser.h"
#include <typeinfo>
#include "src/core/dbghelper.h"
RawToHtmlParser::RawToHtmlParser()
{
}
QString RawToHtmlParser::parseBook(BookBlock *b)
{
     DEBUG_FUNC_NAME;
    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
}

QString RawToHtmlParser::parseBr(BrFragment *b)
{
    return "<br />";
}

QString RawToHtmlParser::parseCaption(CaptionBlock *b)
{
    DEBUG_FUNC_NAME;
    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
}
QString RawToHtmlParser::parseChapter(ChapterBlock *b)
{
     DEBUG_FUNC_NAME;
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
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
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
     DEBUG_FUNC_NAME;
    return b->text;
}

QString RawToHtmlParser::parseVerse(VerseBlock *b)
{
    DEBUG_FUNC_NAME;
    if(!b) return "";
    QString ret;
    foreach(RBlock *block, b->children) {
        ret += parse(block);
    }
    return ret;
}

QString RawToHtmlParser::parseXRef(XRefFragment *b)
{
    return "";
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
    myDebug() << block->metaData().type << typeid(block).name();
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
