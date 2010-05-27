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
#include "interface.h"

Interface::Interface(QWidget *parent) :
        QWidget(parent)
{
}
bool Interface::hasMenuBar()
{
    return false;
}
QMenuBar* Interface::menuBar()
{
    return new QMenuBar();
}

bool Interface::hasToolBar()
{
    return false;
}
QToolBar * Interface::toolBar()
{
    return new QToolBar();
}
void Interface::closing()
{
}

void Interface::setBookDockWidget(BookDockWidget *bookDockWidget)
{
    Q_UNUSED(bookDockWidget);
}
void Interface::setModuleDockWidget(ModuleDockWidget *moduleDockWidget)
{
    Q_UNUSED(moduleDockWidget);
}
void Interface::setSearchResultDockWidget(SearchResultDockWidget *searchResultDockWidget)
{
    Q_UNUSED(searchResultDockWidget);
}
void Interface::setNotesDockWidget(NotesDockWidget *notesDockWidget)
{
    Q_UNUSED(notesDockWidget);

}
void Interface::setBookmarksDockWidget(BookmarksDockWidget *bookmarksDockWidget)
{
    Q_UNUSED(bookmarksDockWidget);
}
void Interface::setStrongDockWidget(StrongDockWidget *strongDockWidget)
{
    Q_UNUSED(strongDockWidget);
}
void Interface::setQuickJumpDockWidget(QuickJumpDockWidget *quickJumpDockWidget)
{
    Q_UNUSED(quickJumpDockWidget);
}
void Interface::init()
{

}
