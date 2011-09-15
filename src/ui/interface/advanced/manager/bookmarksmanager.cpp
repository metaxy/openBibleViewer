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
#include "bookmarksmanager.h"

BookmarksManager::BookmarksManager(QObject *parent) :
    QObject(parent)
{
}

void BookmarksManager::setWidget(QWidget *p)
{
    m_p = p;
}

void BookmarksManager::createDocks()
{
    m_bookmarksDockWidget = new BookmarksDockWidget(m_p);
    setAll(m_bookmarksDockWidget);
    m_bookmarksDockWidget->init();
    m_bookmarksDockWidget->hide();
}

QHash<DockWidget*, Qt::DockWidgetArea> BookmarksManager::docks()
{
    QHash<DockWidget *, Qt::DockWidgetArea> ret;
    ret.insert(m_bookmarksDockWidget,  Qt::RightDockWidgetArea);
    return ret;
}
void BookmarksManager::newBookmark(VerseSelection selection,Versification * v11n)
{
    if(m_bookmarksDockWidget->isHidden()) {
        m_bookmarksDockWidget->show();
    }
    m_bookmarksDockWidget->newBookmark(selection,v11n);
}
void BookmarksManager::save()
{
    m_bookmarksDockWidget->saveBookmarks();
}
BookmarksDockWidget* BookmarksManager::bookmarksDockWidget()
{
    return m_bookmarksDockWidget;
}
