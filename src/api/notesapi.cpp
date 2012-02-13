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

#include "notesapi.h"
#include <QtCore/QDateTime>
NotesApi::NotesApi(QObject* parent) :
    QObject(parent)
{

}

void NotesApi::init()
{
    m_notes->loadingNewInstance();
}

NotesApi::~NotesApi()
{

}

QString NotesApi::addTextNote(const QString &title, const QString &text)
{
    const QString newID = m_notes->generateNewID();
    m_notes->setData(newID, text);
    m_notes->setTitle(newID, title);
    m_notes->setType(newID, "text");

    QMap<QString, QString> noteRef;
    const QDateTime t = QDateTime::currentDateTime();
    noteRef["created"] = t.toString(Qt::ISODate);

    m_notes->setRef(newID, noteRef);
    m_notes->insertID(newID);
    return newID;

}

void NotesApi::removeNote(const QString &noteID)
{
    m_notes->removeNote(noteID);
}


