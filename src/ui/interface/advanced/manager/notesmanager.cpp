/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include "notesmanager.h"

NotesManager::NotesManager(QObject *parent) :
    QObject(parent)
{
}

void NotesManager::setWidget(QWidget *p)
{
    m_p = p;
}

void NotesManager::createDocks()
{
    m_notesDockWidget = new NotesDockWidget(m_p);
    setAll(m_notesDockWidget);
    m_notesDockWidget->init();
    m_notesDockWidget->hide();
}

QHash<DockWidget*, Qt::DockWidgetArea> NotesManager::docks()
{
    //DEBUG_FUNC_NAME
    QHash<DockWidget *, Qt::DockWidgetArea> ret;
    ret.insert(m_notesDockWidget, Qt::RightDockWidgetArea);
    return ret;
}

void NotesManager::newCustomColorMark(VerseSelection selection, QColor color, QSharedPointer<Versification> v11n)
{
    DEBUG_FUNC_NAME
    m_notesDockWidget->newStyleMark(selection,  "background-color: " + color.name() + ";", v11n);
}

void NotesManager::newBoldMark(VerseSelection selection, QSharedPointer<Versification> v11n)
{
    m_notesDockWidget->newStyleMark(selection, "font-weight: bold;", v11n);
}

void NotesManager::newItalicMark(VerseSelection selection, QSharedPointer<Versification> v11n)
{
    m_notesDockWidget->newStyleMark(selection, "font-style: italic;", v11n);

}

void NotesManager::newUnderlineMark(VerseSelection selection, QSharedPointer<Versification> v11n)
{
    m_notesDockWidget->newStyleMark(selection, "text-decoration:underline;", v11n);
}

void NotesManager::removeMark(VerseSelection selection, QSharedPointer<Versification> v11n)
{
    m_notesDockWidget->removeMark(selection, v11n);
}

void NotesManager::newNoteWithLink(VerseSelection selection, QSharedPointer<Versification> v11n)
{
    DEBUG_FUNC_NAME;
    if(m_notesDockWidget->isHidden()) {
        m_notesDockWidget->show();
    }
    m_notesDockWidget->newNoteWithLink(selection, v11n);
}

void NotesManager::openNote(const QString &noteID)
{
    if(!m_notesDockWidget->isHidden()) {
        m_notesDockWidget->show();
    }
    m_notesDockWidget->showNote(noteID);
}

void NotesManager::save()
{
    m_notesDockWidget->saveNote();
}

NotesDockWidget *NotesManager::notesDockWidget()
{
    return m_notesDockWidget;
}
