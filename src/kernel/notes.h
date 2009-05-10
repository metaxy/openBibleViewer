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
#ifndef NOTES_H
#define NOTES_H
#include <QString>
#include <QStringList>
#include <QDomDocument>
class notes
{
	public:
		notes( QString fileName);
		int loadNotes();
		int readNotes();
		int saveNotes();
		QString fileName;
		QStringList notesData;
		QStringList notesTitel;
		QStringList notesPos;
		QDomDocument doc;

};

#endif // NOTES_H
