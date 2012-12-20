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