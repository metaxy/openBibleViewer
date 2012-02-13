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
#include "textnotes.h"
#include "src/core/dbghelper.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDir>
TextNotes::TextNotes()
{
    DEBUG_FUNC_NAME
    m_isLoaded = false;
}
Notes::NotesTextType TextNotes::type() const
{
    return Notes::TextNotes;
}
void TextNotes::init(const QString &fileName)
{
    DEBUG_FUNC_NAME
    m_fileName = fileName;
    m_version = "0.1";
    m_isLoaded = false;
    m_oldVersion = "";
}

bool TextNotes::isLoaded() const
{
    return m_isLoaded;
}

void TextNotes::loadingNewInstance()
{
    DEBUG_FUNC_NAME
    emit saveAll();
}

int TextNotes::loadNotes()
{
    DEBUG_FUNC_NAME;
    QDir d(m_fileName);
    const QStringList l = d.entryList(QDir::Files);
    foreach(const QString fileName, l) {
        notesID << fileName;
        notesType[fileName] = "text";
        notesTitle[fileName] = fileName;
        QFile file(m_fileName + "/" + fileName);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            notesData[fileName] = in.readAll();
        }
    }
    m_isLoaded = true;
    return 0;
}

QString TextNotes::getType(const QString &id) const
{
    return notesType.value(id, "");
}

QString TextNotes::getTitle(const QString &id) const
{
    return notesTitle.value(id, "");
}

QString TextNotes::getData(const QString &id) const
{
    return notesData.value(id, "");
}

QString TextNotes::getRef(const QString &id, const QString &refID) const
{
    //loadNote(id);
    if(notesRef.contains(id)) {
        QMap<QString, QString> r = notesRef.value(id);
        return r.value(refID, "");
    } else {
        return QString();
    }
    return "";
}

QMap<QString, QString> TextNotes::getRef(const QString &id) const
{
    //loadNote(id);
    if(notesRef.contains(id))
        return notesRef.value(id);
    else
        return QMap<QString, QString>();
}
/**
  Get all note IDs
  */
QStringList TextNotes::getIDList() const
{
    return notesID;
}

QStringList TextNotes::getIDList(const QString &type) const
{
    return notesType.keys(type);
}


void TextNotes::clearAll()
{
    notesData.clear();
    notesTitle.clear();
    notesRef.clear();
    notesType.clear();
    notesID.clear();
}

void TextNotes::setType(const QString &id, const QString &type)
{
    notesType.insert(id, type);
}

void TextNotes::setTitle(const QString &id, const QString &title)
{
    //loadNote(id);
    if(notesTitle.value(id) != title) {
        notesTitle.insert(id, title);
        emit titleChanged(id, title);
    }
}

void TextNotes::setData(const QString &id, const QString &data)
{
    //loadNote(id);
    if(notesData.value(id) != data) {
        notesData.insert(id, data);
        emit dataChanged(id, data);
    }
}

void TextNotes::setRef(const QString &id, const QMap<QString, QString>  &ref)
{
    //loadNote(id);
    if(notesRef.value(id) != ref) {
        notesRef.insert(id, ref);
        emit refChanged(id, ref);
    }
}

void TextNotes::setRef(const QString &id, const QString &key, const QString &value)
{
    //loadNote(id);
    QMap<QString, QString>  ref = notesRef.value(id);
    if(ref.value(key) != value) {
        ref.insert(key, value);
        notesRef.insert(id, ref);
        emit refChanged(id, ref);
    }
}

QString TextNotes::generateNewID() const
{
    //DEBUG_FUNC_NAME
    QMapIterator<QString, QString> i(notesType);
    long biggest = 0;
    while(i.hasNext()) {
        i.next();
        const int id = i.key().toLong();
        if(id > biggest)
            biggest = id;
    }
    return QString::number(biggest + 1);
}

void TextNotes::insertID(const QString &id)
{
    notesID << id;
    emit noteAdded(id);
}

void TextNotes::removeNote(const QString &id)
{
    //DEBUG_FUNC_NAME
    QMap<QString, QString>  ref = notesRef.value(id);
    notesType.remove(id);
    notesTitle.remove(id);
    notesData.remove(id);
    notesRef.remove(id);
    notesID.removeOne(id);
    QFile f(id);
    f.remove();
    emit noteRemoved(id, ref);
}

int TextNotes::readNotes()
{
    //read all notes in notesData
    return 0;
}

int TextNotes::saveNotes()
{
    //DEBUG_FUNC_NAME
    /* QDomDocument sdoc;
     QDomElement root = sdoc.createElement("notes");
     root.setAttribute("version", m_version);
     sdoc.appendChild(root);
     QMapIterator<QString, QString> i(notesType);
     while(i.hasNext()) {
         i.next();
         const QString id = i.key();
         if(id.isEmpty())
             continue;
         QDomElement tag = sdoc.createElement("note");
         tag.setAttribute("title", notesTitle.value(id));
         tag.setAttribute("type", notesType.value(id));
         tag.setAttribute("id", id);
         root.appendChild(tag);
         QDomElement data = sdoc.createElement("data");
         QDomCDATASection text = sdoc.createCDATASection(notesData.value(id));
         data.appendChild(text);
         tag.appendChild(data);
         QDomElement ref = sdoc.createElement("ref");
         QMap<QString, QString> map = notesRef.value(id);
         QMapIterator<QString, QString> i(map);
         while(i.hasNext()) {
             i.next();
             QDomElement e = sdoc.createElement(i.key());
             QDomText t = doc.createTextNode(i.value());
             e.appendChild(t);
             ref.appendChild(e);
         }
         tag.appendChild(ref);
     }
     QFile file(m_fileName);
     if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
         return 1;
     const int IndentSize = 4;
     QTextStream out(&file);
     sdoc.save(out, IndentSize);
     file.close();
     doc = sdoc;
     return 0;*/
    return 0;
}

void TextNotes::search(SearchQuery query, SearchResult *res) const
{
    /*  DEBUG_FUNC_NAME
      QStringList f;
      QMapIterator<QString, QString> i(notesTitle);
      while(i.hasNext()) {
          i.next();
          if(i.value().contains(query.searchText)) {
              //add hit
              SearchHit hit;
              hit.setType(SearchHit::NoteHit);
              hit.setValue(SearchHit::NoteID, i.key());
              res->addHit(hit);
              f << i.key();
          }
      }
      QMapIterator<QString, QString > i2(notesData);
      while(i2.hasNext()) {
          i2.next();
          if(!f.contains(i2.key())) {
              if(i2.value().contains(query.searchText)) {
                  SearchHit hit;
                  hit.setType(SearchHit::NoteHit);
                  hit.setValue(SearchHit::NoteID, i2.key());
                  res->addHit(hit);
              }
          }

      }*/
}

