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
#include "xmlnotes.h"
#include "src/core/dbghelper.h"
#include <QtCore/QFile>
#include <QtXml/QtXml>
#include <QtXml/QDomAttr>
#include <QtXml/QDomElement>
#include <QtCore/QTextStream>
#include <QtCore/QDir>
XmlNotes::XmlNotes()
{
    m_isLoaded = false;
}
Notes::NotesTextType XmlNotes::type() const
{
    return Notes::HtmlNotes;
}
void XmlNotes::init(const QString &fileName)
{
    m_fileName = fileName;
    m_version = "0.4";
    m_isLoaded = false;
    m_oldVersion = "";
}

bool XmlNotes::isLoaded() const
{
    return m_isLoaded;
}

void XmlNotes::loadingNewInstance()
{
    emit saveAll();
}

int XmlNotes::loadNotes()
{
    //DEBUG_FUNC_NAME
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
        } else if(oldVersion == "0.3" && oldVersion != m_version) {
            m_oldVersion = "0.3";
        }
    }
    file.close();
    m_isLoaded = true;
    return 0;
}

QString XmlNotes::getType(const QString &id) const
{
    return notesType.value(id, "");
}

QString XmlNotes::getTitle(const QString &id) const
{
    return notesTitle.value(id, "");
}

QString XmlNotes::getData(const QString &id) const
{
    return notesData.value(id, "");
}

QString XmlNotes::getRef(const QString &id, const QString &refID) const
{
    if(notesRef.contains(id)) {
        QMap<QString, QString> r = notesRef.value(id);
        return r.value(refID, "");
    } else {
        return QString();
    }
}

QMap<QString, QString> XmlNotes::getRef(const QString &id) const
{
    if(notesRef.contains(id))
        return notesRef.value(id);
    else
        return QMap<QString, QString>();
}
/**
  Get all note IDs
  */
QStringList XmlNotes::getIDList() const
{
    return notesID;
}

QStringList XmlNotes::getIDList(const QString &type) const
{
    return notesType.keys(type);
}


void XmlNotes::clearAll()
{
    notesData.clear();
    notesTitle.clear();
    notesRef.clear();
    notesType.clear();
    notesID.clear();
}

void XmlNotes::setType(const QString &id, const QString &type)
{
    notesType.insert(id, type);
}

void XmlNotes::setTitle(const QString &id, const QString &title)
{
    if(notesTitle.value(id) != title) {
        notesTitle.insert(id, title);
        emit titleChanged(id, title);
    }
}

void XmlNotes::setData(const QString &id, const QString &data)
{
    if(notesData.value(id) != data) {
        notesData.insert(id, data);
        emit dataChanged(id, data);
    }
}

void XmlNotes::setRef(const QString &id, const QMap<QString, QString>  &ref)
{
    if(notesRef.value(id) != ref) {
        notesRef.insert(id, ref);
        emit refChanged(id, ref);
    }
}

void XmlNotes::setRef(const QString &id, const QString &key, const QString &value)
{
    QMap<QString, QString>  ref = notesRef.value(id);
    if(ref.value(key) != value) {
        ref.insert(key, value);
        notesRef.insert(id, ref);
        emit refChanged(id, ref);
    }
}

QString XmlNotes::generateNewID() const
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

void XmlNotes::insertID(const QString &id)
{
    notesID << id;
    emit noteAdded(id);
}

void XmlNotes::removeNote(const QString &id)
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

int XmlNotes::readNotes()
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
                    notesData.insert(id, e2.text());
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
                        QString val;
                        if(m_oldVersion == "0.3" || m_oldVersion == "0.2") {
                            val = e3.attribute("id", "");
                        } else {
                            val = e3.text();
                        }

                        if(m_oldVersion == "0.2" && tag == "color") {
                            tag = "style";
                            val = "background-color:" + val + ";";
                        }
                        map.insert(tag, val);
                        n3 = n3.nextSibling();

                    }
                    notesRef.insert(id, map);
                }
                n2 = n2.nextSibling();
            }

            notesTitle.insert(id, e.attribute("title", QObject::tr("(unnamed)")));
            notesType.insert(id, e.attribute("type", "unkown"));
        }
        n = n.nextSibling();
    }
    return 0;
}

int XmlNotes::saveNotes()
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
    return 0;
}

void XmlNotes::search(SearchQuery query, SearchResult *res) const
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

