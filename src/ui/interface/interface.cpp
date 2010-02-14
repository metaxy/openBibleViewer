/***************************************************************************
openBibleViewer - Bible Study Tool
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
#include "interface.h"

Interface::Interface(QWidget *parent) :
        QWidget(parent)
{
}
void Interface::setModuleManager(ModuleManager *manager)
{
    m_moduleManager = manager;
}
void Interface::setBibleDisplay(BibleDisplay *bibleDisplay)
{
    m_bibleDisplay = bibleDisplay;
}
void Interface::setSettings(Settings *settings)
{
    m_settings = settings;
}
void Interface::setNotes(Notes *notes)
{
    m_notes = notes;
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

