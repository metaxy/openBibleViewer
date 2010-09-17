/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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
#include "notes.h"
#include "dbghelper.h"
#include <QtCore/QFile>
#include <QtXml/QtXml>
#include <QtXml/QDomAttr>
#include <QtXml/QDomElement>
#include <QtCore/QTextStream>
#include <QtCore/QDir>
Notes::Notes()
{
    m_isLoaded = false;
}
/**
  Init the notes;
  \param fileName The path to the notes xml file.
  */
void Notes::init(const QString &fileName)
{
    m_fileName = fileName;
    m_version = "0.3";
    m_isLoaded = false;
    m_oldVersion = "";
}
/**
  Returns true if in this instance the notes are loaded.
  */
bool Notes::isLoaded() const
{
    return m_isLoaded;
}
/**
  Emits a signal to save data in all other instances.
  */
void Notes::loadingNewInstance()
{
    emit saveAll();
}

/*!
    Load notes data from an xml-file
  */
int Notes::loadNotes()
{
    DEBUG_FUNC_NAME
    QFile file(m_fileName);
    doc.clear();
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        myWarning() << "cannot read the file " << file.fileName() << "error = " << file.errorString();
        return 1;
    }
    if(!doc.setContent(&file)) {
        myWarning() << "the file isn't valid";
        file.close();
        return 1;
    }
    if(doc.documentElement().isElement() && !doc.documentElement().isNull()) {
        QDomElement e = doc.documentElement();
        const QString oldVersion = e.attribute("version", "0.1");
        if(oldVersion == "0.1") {
            //errror to old version
            myWarning() << "too old version " << e.attribute("version", "0.1") << " current is " << m_version;
            file.close();
            //make backup
            QDir dir(m_fileName);
            dir.rename(m_fileName, m_fileName + ".bak");

            loadNotes();
            return 2;
        } else if(oldVersion == "0.2" && oldVersion != m_version) {
            m_oldVersion = "0.2";
        }
    }
    file.close();
    m_isLoaded = true;
    return 0;
}
/*!
  Get the type of a note(eg. text, mark, tag)
  \param id Note ID
  */
QString Notes::getType(const QString &id)
{
    if(!notesType[id].isEmpty())
        return notesType[id];
    else
        return QString();
}
/*!
  Get the note title
  \param id Note ID
  */
QString Notes::getTitle(const QString &id)
{
    if(!notesTitle[id].isEmpty())
        return notesTitle[id];
    else
        return QString();
}
/*!
  Get the note data
  \param id Note ID
  */
QString Notes::getData(const QString &id)
{
    if(!notesData[id].isEmpty())
        return notesData[id];
    else
        return QString();
}
/*!
  Get the note reference
  \param id note ID
  \param refID reference ID
  */
QString Notes::getRef(const QString &id, const QString &refID)
{
    if(!notesRef[id].isEmpty()) {
        QMap<QString, QString> r = notesRef[id];
        if(!r[refID].isEmpty())
            return r[refID];
        else
            return QString();
    } else {
        return QString();
    }
}
/**
  Get all references of a note
  \param id note ID
  */
QMap<QString, QString> Notes::getRef(const QString &id)
{
    if(!notesRef[id].isEmpty())
        return notesRef[id];
    else
        return QMap<QString, QString>();
}
/**
  Get all note IDs
  */
QStringList Notes::getIDList() const
{
    return notesID;
}
/**
  Get all notes IDs with a given type.
  \param type The needed type.
  */
QStringList Notes::getIDList(const QString &type) const
{
    return notesType.keys(type);
}

/**
  Clear all data
  */
void Notes::clearAll()
{
    notesData.clear();
    notesTitle.clear();
    notesRef.clear();
    notesType.clear();
    notesID.clear();
}
/*!
  Set note type
  \param id note ID
  \param type note type
  */
void Notes::setType(const QString &id, const QString &type)
{
    notesType[id] = type;
}
/*!
  Set note title
  \param id note ID
  \param title note title
  */
void Notes::setTitle(const QString &id, const QString &title)
{
    if(notesTitle[id] != title) {
        notesTitle[id] = title;
        emit titleChanged(id, title);
    }
}
/*!
  Set note data
  \param id note ID
  \param data note data
  */
void Notes::setData(const QString &id, const QString &data)
{
    if(notesData[id] != data) {
        notesData[id] = data;
        emit dataChanged(id, data);
    }
}
/**
  Set note reference
  \param id note ID
  \param ref note reference
  */
void Notes::setRef(const QString &id, const QMap<QString, QString>  &ref)
{
    if(notesRef[id] != ref) {
        notesRef[id] = ref;
        emit refChanged(id, ref);
    }
}
/**
  Set note reference
  \param id The note ID.
  \param key The key.
  \param value The new value.
  */
void Notes::setRef(const QString &id, const QString &key, const QString &value)
{
    QMap<QString, QString>  ref = notesRef[id];
    if(ref.value(key) != value) {
        ref[key] = value;
        notesRef[id] = ref;
        emit refChanged(id, ref);
    }
}
/**
    Generate a new note ID, without colliding with other IDs
 */
QString Notes::generateNewID() const
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
/*!
 Insert a new ID, and to add a new note you have to set title for this id
 \param id the new note id
 */
void Notes::insertID(const QString &id)
{
    notesID << id;
    emit noteAdded(id);
}
/*!
 Remove Note
 \param id note id
  */
void Notes::removeNote(const QString &id)
{
    //DEBUG_FUNC_NAME
    QMap<QString, QString>  ref = notesRef.value(id);
    notesType.remove(id);
    notesTitle.remove(id);
    notesData.remove(id);
    notesRef.remove(id);
    notesID.removeOne(id);
    emit noteRemoved(id, ref);
}
/*!
  Pharse note data from the xml-file
  */
int Notes::readNotes()
{
    //read all notes in notesData
    //DEBUG_FUNC_NAME
    QDomNode n = doc.documentElement().firstChild();
    while(!n.isNull()) {
        if(!n.isElement()) {
            n = n.nextSibling();
            continue;
        }
        QDomElement e = n.toElement();
        if(e.hasAttribute("id")) {
            const QString id = e.attribute("id", "");
            if(id == "")
                continue;
            notesID << id;
            QDomNode n2 = e.firstChild();
            while(!n2.isNull()) {
                if(!n2.isElement()) {
                    n2 = n2.nextSibling();
                    continue;
                }
                QDomElement e2 = n2.toElement();
                if(e2.tagName() == "data") {
                    notesData[id] = e2.text();
                } else if(e2.tagName() == "ref") {
                    QMap<QString, QString> map;
                    QDomNode n3 = e2.firstChild();
                    while(!n3.isNull()) {
                        if(!n3.isElement()) {
                            n3 = n3.nextSibling();
                            continue;
                        }
                        QDomElement e3 = n3.toElement();
                        QString tag = e3.tagName();
                        QString val = e3.attribute("id", "");
                        if(m_oldVersion == "0.2" && tag == "color") {
                            tag = "style";
                            val = "background-color:" + val + ";";
                        }
                        map[tag] = val;
                        n3 = n3.nextSibling();

                    }
                    notesRef[id] = map;
                }
                n2 = n2.nextSibling();
            }

            notesTitle[id] = e.attribute("title", QObject::tr("(unnamed)"));
            notesType[id] = e.attribute("type", "unkown");
        }
        n = n.nextSibling();
    }
    return 0;
}
/*!
  Save the data in a xml file
  */
int Notes::saveNotes()
{
    //DEBUG_FUNC_NAME
    QDomDocument sdoc;
    QDomElement root = sdoc.createElement("notes");
    root.setAttribute("version", m_version);
    sdoc.appendChild(root);
    QMapIterator<QString, QString> i(notesType);
    while(i.hasNext()) {
        i.next();
        const QString id = i.key();
        if(id == "")
            continue;
        QDomElement tag = sdoc.createElement("note");
        tag.setAttribute("title", notesTitle[id]);
        tag.setAttribute("type", notesType[id]);
        tag.setAttribute("id", id);
        root.appendChild(tag);
        QDomElement data = sdoc.createElement("data");
        QDomCDATASection text = sdoc.createCDATASection(notesData[id]);//todo: use cdata section
        data.appendChild(text);
        tag.appendChild(data);
        QDomElement ref = sdoc.createElement("ref");
        QMap<QString, QString> map = notesRef[id];
        QMapIterator<QString, QString> i(map);
        while(i.hasNext()) {
            i.next();
            QDomElement e = sdoc.createElement(i.key());
            e.setAttribute("id", i.value());
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
    return 0;
}
/**
  Search in Notes.
  \param query The Search Query
  \param res The Results.
  */
void Notes::search(SearchQuery query, SearchResult *res) const
{
    DEBUG_FUNC_NAME
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

    }
}

