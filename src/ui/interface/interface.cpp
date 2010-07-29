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
QList<QToolBar *> Interface::toolBars()
{
    QToolBar * t = new QToolBar();
    QList<QToolBar*> l;
    l.append(t);
    return l;
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
void Interface::setAdvancedSearchResultDockWidget(AdvancedSearchResultDockWidget *advancedSearchResultDockWidget)
{
    Q_UNUSED(advancedSearchResultDockWidget);
}
void Interface::setNotesDockWidget(NotesDockWidget *notesDockWidget)
{
    Q_UNUSED(notesDockWidget);

}
void Interface::setBookmarksDockWidget(BookmarksDockWidget *bookmarksDockWidget)
{
    Q_UNUSED(bookmarksDockWidget);
}
void Interface::setDictionaryDockWidget(DictionaryDockWidget *dictionaryDockWidget)
{
    Q_UNUSED(dictionaryDockWidget);
}
void Interface::setQuickJumpDockWidget(QuickJumpDockWidget *quickJumpDockWidget)
{
    Q_UNUSED(quickJumpDockWidget);
}
void Interface::init()
{

}
