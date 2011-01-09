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
    return 0;
}

bool Interface::hasToolBar()
{
    return false;
}
QList<QToolBar *> Interface::toolBars()
{
    return QList<QToolBar *>();
}
void Interface::closing()
{
}

void Interface::init()
{
}
QHash<DockWidget*, Qt::DockWidgetArea> Interface::docks()
{
    QHash<DockWidget *, Qt::DockWidgetArea> ret;
    return ret;

}
void Interface::createDocks()
{
}

void Interface::createToolBars()
{
}

void Interface::createMenu()
{
}
