/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
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
#include <QtCore/QDebug>
Notes::Notes(QString newFileName)
{
    fileName = newFileName;
    m_version = "0.2";
}
int Notes::loadNotes(void)
{
    DEBUG_FUNC_NAME
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "notes::loadNotes() cannot read the file";
        return 1;
    }
    if (!doc.setContent(&file)) {
        qDebug() << "notes::loadNotes() the file isnt valid";
        file.close();
        return 1;
    }
    if (doc.documentElement().isElement() && !doc.documentElement().isNull()) {
        QDomElement e = doc.documentElement();
        if (e.attribute("version", "0.1") != m_version) {
            //errror to old version
            qDebug() << "notes::loadNotes() too old version " << e.attribute("version", "0.1") << " current is " << m_version;
            file.close();
            return 1;
        }
    }
    file.close();
    return 0;
}
QString Notes::getType(const QString &id)
{
    // DEBUG_FUNC_NAME
    if (!notesType[id].isEmpty())
        return notesType[id];
    else
        return QString();
}
QString Notes::getTitle(const QString &id)
{
    // DEBUG_FUNC_NAME
    if (!notesTitle[id].isEmpty())
        return notesTitle[id];
    else
        return QString();
}
QString Notes::getData(const QString &id)
{
    //DEBUG_FUNC_NAME
    myDebug() << " id = " << id;
    if (!notesData[id].isEmpty())
        return notesData[id];
    else
        return QString();
}
QString Notes::getRef(const QString &id, const QString &refID)
{
    // DEBUG_FUNC_NAME
    // myDebug() << " id = " << id;
    if (!notesRef[id].isEmpty()) {
        QMap<QString, QString> r = notesRef[id];
        if (!r[refID].isEmpty())
            return r[refID];
        else
            return QString();
    } else {
        return QString();
    }
}
QMap<QString, QString> Notes::getRef(const QString &id)
{
    //DEBUG_FUNC_NAME
    //  myDebug() << " id = " << id << " notesRef = " << notesRef;
    if (!notesRef[id].isEmpty())
        return notesRef[id];
    else
        return QMap<QString, QString>();
}
QStringList Notes::getIDList()
{
    //DEBUG_FUNC_NAME
    return notesID;
}
void Notes::clearAll()
{
    //DEBUG_FUNC_NAME
    notesData.clear();
    notesTitle.clear();
    notesRef.clear();
    notesType.clear();
    notesID.clear();
}
void Notes::setType(const QString &id, const QString &type)
{
    //DEBUG_FUNC_NAME
    notesType[id] = type;
}
void Notes::setTitle(const QString &id, const QString &title)
{
    //DEBUG_FUNC_NAME
    notesTitle[id] = title;
}
void Notes::setData(const QString &id, const QString &data)
{
    //DEBUG_FUNC_NAME
    notesData[id] = data;
}
void Notes::setRef(const QString &id, const QMap<QString, QString>  &ref)
{
    //DEBUG_FUNC_NAME
    notesRef[id] = ref;
}
QString Notes::generateNewID()
{
    //DEBUG_FUNC_NAME
    QMapIterator<QString, QString> i(notesType);
    int biggest = 0;
    while (i.hasNext()) {
        i.next();
        myDebug() << " i.key() = " << i.key();
        int id = i.key().toInt();
        if (id > biggest)
            biggest = id;
    }
    myDebug() << "biggest = " << biggest;
    return QString::number(biggest + 1);
}
void Notes::insertID(const QString &id)
{
    // DEBUG_FUNC_NAME
    // myDebug() << " id = " << id;
    notesID << id;
}
void Notes::removeNote(const QString &id)
{
    //DEBUG_FUNC_NAME
    notesType.remove(id);
    notesTitle.remove(id);
    notesData.remove(id);
    notesRef.remove(id);
    notesID.removeOne(id);
}
int Notes::readNotes()
{
    //read all notes in notesData
    DEBUG_FUNC_NAME
    QDomNode n = doc.documentElement().firstChild();
    while (!n.isNull()) {
        if (!n.isElement()) {
            n = n.nextSibling();
            continue;
        }
        QDomElement e = n.toElement();
        if (e.hasAttribute("id")) {
            QString id = e.attribute("id", "");
            if (id == "")
                continue;
            notesID << id;
            QDomNode n2 = e.firstChild();
            while (!n2.isNull()) {
                if (!n2.isElement()) {
                    n2 = n2.nextSibling();
                    continue;
                }
                QDomElement e2 = n2.toElement();
                if (e2.tagName() == "data") {
                    notesData[id] = e2.text();
                } else if (e2.tagName() == "ref") {
                    QMap<QString, QString> map;
                    QDomNode n3 = e2.firstChild();
                    while (!n3.isNull()) {
                        if (!n3.isElement()) {
                            n3 = n3.nextSibling();
                            continue;
                        }
                        QDomElement e3 = n3.toElement();
                        map[e3.tagName()] = e3.attribute("id", "");
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
int Notes::saveNotes()
{
    DEBUG_FUNC_NAME
    QDomDocument sdoc;
    QDomElement root = sdoc.createElement("notes");
    root.setAttribute("version", m_version);
    sdoc.appendChild(root);
    QMapIterator<QString, QString> i(notesType);
    while (i.hasNext()) {
        i.next();
        QString id = i.key();
        if (id == "")
            continue;
        QDomElement tag = sdoc.createElement("note");
        tag.setAttribute("title", notesTitle[id]);
        tag.setAttribute("type", notesType[id]);
        tag.setAttribute("id", id);
        root.appendChild(tag);
        QDomElement data = sdoc.createElement("data");
        QDomText text = sdoc.createTextNode(notesData[id]);
        data.appendChild(text);
        tag.appendChild(data);
        QDomElement ref = sdoc.createElement("ref");
        QMap<QString, QString> map = notesRef[id];
        QMapIterator<QString, QString> i(map);
        while (i.hasNext()) {
            i.next();
            QDomElement e = sdoc.createElement(i.key());
            e.setAttribute("id", i.value());
            ref.appendChild(e);
        }
        tag.appendChild(ref);
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return 1;
    QTextStream out(&file);
    out << "<?xml version=\"1.0\"?>\n" + sdoc.toString();
    file.close();
    doc = sdoc;
    return 0;
}

