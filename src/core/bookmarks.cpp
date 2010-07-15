/***************************************************************************
openBibleViewer - Free Bibel Viewer
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
#include "bookmarks.h"
#include <QFile>
#include <QtXml>
#include <QDomAttr>
#include <QDomElement>
#include <QTextStream>

bookmarks::bookmarks()
{
    fileName = "bookmarks.xml";
}
int bookmarks::loadBookmarks(void)
{
    qDebug("bookmarks::loadBookmarks() start");
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug("notes::loadNotes() cant read the file");
        return 1;
    }
    if(!doc.setContent(&file)) {
        qDebug("notes::loadNotes() the file isnt valid");
        return 1;
    }
    file.close();
    qDebug("notes::loadNotes() exit");
    return 0;
}
int bookmarks::readBookmarks()
{
    //read all notes in notesData notesTitle
    QDomNode n = doc.documentElement().firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement();
        if(e.attribute("titel", "") != "") {
            bmData << e.text();
            bmTitel << e.attribute("titel", "(unbenannt)");
        }
        n = n.nextSibling();
    }
    return 0;
}
int bookmarks::saveBookmarks()
{
    /*QDomDocument sdoc;
    QDomElement root = sdoc.createElement("notes");
    sdoc.appendChild(root);
    for(int i=0;i<notesTitel.size();i++)
    {
        QDomElement tag = sdoc.createElement("note");
        tag.setAttribute("titel",notesTitel.at(i));
        tag.setAttribute("id",i);
        root.appendChild(tag);
        QDomText t = sdoc.createTextNode(notesData.at(i));
        tag.appendChild(t);
    }
    QString outtext = "<?xml version=\"1.0\"?>"+sdoc.toString();
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return 0;
    QTextStream out(&file);
    out << outtext;
    file.close();
    doc = sdoc;*/
    return 0;
}



