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
#ifndef NOTESMANAGER_H
#define NOTESMANAGER_H

#include <QObject>
#include <QtCore/QHash>
#include "src/core/basicclass.h"
#include "src/core/verseselection.h"
#include "src/ui/dock/notesdockwidget.h"
class NotesManager : public QObject, public BasicClass
{
    Q_OBJECT
public:
    explicit NotesManager(QObject *parent = 0);
    void setWidget(QWidget *p);
    void createDocks();
    QHash<DockWidget *, Qt::DockWidgetArea> docks();
    void openNote(const QString &noteID);

signals:

public slots:

    void newCustomColorMark(VerseSelection selection, QColor color);
    void newBoldMark(VerseSelection selection);
    void newItalicMark(VerseSelection selection);
    void newUnderlineMark(VerseSelection selection);

    void removeMark(VerseSelection selection);
    void newNoteWithLink(VerseSelection selection);

    void save();
private:
    QWidget *m_p;
    NotesDockWidget *m_notesDockWidget;
};

#endif // NOTESMANAGER_H
