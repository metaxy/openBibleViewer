#include "blocktools.h"
RBlock * BlockTools::create(quint32 id, quint32 parentID, RMetaData::RType type)
{
    RMetaData meta(parentID,type);
    switch(type) {
    case RMetaData::BookBlock:
        return new BookBlock(id,meta);
    case RMetaData::BrFragment:
        return new BrFragment(id,meta);
        break;
    case RMetaData::CaptionBlock:
        return new CaptionBlock(id,meta);
        break;
    case RMetaData::ChapterBlock:
        return new ChapterBlock(id,meta);
        break;
    case RMetaData::DivBlock:
        return new DivBlock(id,meta);
        break;
    case RMetaData::GramBlock:
        return new GramBlock(id,meta);
        break;
    case RMetaData::MediaBlock:
        return new MediaBlock(id,meta);
        break;
    case RMetaData::NoteBlock:
        return new NoteBlock(id,meta);
        break;
    case RMetaData::PrologBlock:
        return new PrologBlock(id,meta);
        break;
    case RMetaData::RemarksBlock:
        return new RemarksBlock(id,meta);
        break;
    case RMetaData::StyleBlock:
        return new StyleBlock(id,meta);
        break;
    case RMetaData::SupBlock:
        return new SupBlock(id,meta);
        break;
    case RMetaData::TextFragment:
        return new TextFragment(id,meta);
        break;
    case RMetaData::VerseBlock:
        return new VerseBlock(id,meta);
        break;
    case RMetaData::XRefFragment:
        return new XRefFragment(id,meta);
        break;
    default:
        return new RBlock(id,meta);
    }
}
