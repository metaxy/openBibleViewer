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
#include "notesmanager.h"
#include "src/core/dbghelper.h"
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

void NotesManager::newCustomColorMark(VerseSelection selection, QColor color)
{
    DEBUG_FUNC_NAME
    m_notesDockWidget->newStyleMark(selection,  "background-color: " + color.name() + ";");
    m_actions->reloadChapter();
}

void NotesManager::newBoldMark(VerseSelection selection)
{
    m_notesDockWidget->newStyleMark(selection, "font-weight: bold;");
    m_actions->reloadChapter();
}

void NotesManager::newItalicMark(VerseSelection selection)
{
    m_notesDockWidget->newStyleMark(selection, "font-style: italic;");
    m_actions->reloadChapter();

}

void NotesManager::newUnderlineMark(VerseSelection selection)
{
    m_notesDockWidget->newStyleMark(selection, "text-decoration:underline;");
    m_actions->reloadChapter();

}

void NotesManager::removeMark(VerseSelection selection)
{
    m_notesDockWidget->removeMark(selection);
    m_actions->reloadChapter();
}

void NotesManager::newNoteWithLink(VerseSelection selection)
{
    m_notesDockWidget->newNoteWithLink(selection);
    m_actions->reloadChapter();
}

void NotesManager::openNote(const QString &noteID)
{
    if(!m_notesDockWidget->isVisible()) {
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
