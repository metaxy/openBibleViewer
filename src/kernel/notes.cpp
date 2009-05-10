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
#include "notes.h"

#include <QFile>
#include <QtXml>
#include <QDomAttr>
#include <QDomElement>
#include <QTextStream>
notes::notes( QString fileName_)
{
	fileName=fileName_;
}
int notes::loadNotes( void )
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug("notes::loadNotes() cant read the file");
		return 1;
	}
	if (!doc.setContent(&file))
	{
		qDebug("notes::loadNotes() the file isnt valid");
		return 1;
	}
	file.close();
	return 0;
}
int notes::readNotes()
{
	//read all notes in notesData notesTitle
	QDomNode n = doc.documentElement().firstChild();
	while(!n.isNull())
	{
		QDomElement e = n.toElement();
		if(e.attribute("titel","") != "")
		{
			notesData << e.text();
			notesTitel << e.attribute("titel",QObject::tr("(unnamed)"));
			notesPos << e.attribute("pos","");
		}
		n = n.nextSibling();
	}
	//qDebug() << "notes::readNotes() notesData = " << notesData;
	return 0;
}
int notes::saveNotes()
{
	//qDebug() << "notes::saveNotes() notesData = " << notesData;
	QDomDocument sdoc;
	QDomElement root = sdoc.createElement("notes");
	sdoc.appendChild(root);
	for(int i=0;i<notesTitel.size();i++)
	{
		QDomElement tag = sdoc.createElement("note");
		tag.setAttribute("titel",notesTitel.at(i));
		tag.setAttribute("pos",notesPos.at(i));
		tag.setAttribute("id",i);
		root.appendChild(tag);
		QDomText t = sdoc.createTextNode(notesData.at(i));
		tag.appendChild(t);
	}
	QString outtext = "<?xml version=\"1.0\"?>"+sdoc.toString();
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return 1;
	QTextStream out(&file);
	out << outtext;
	file.close();
	doc = sdoc;
	return 0;
}

