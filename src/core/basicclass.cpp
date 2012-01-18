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
#include "basicclass.h"

BasicClass::BasicClass()
{
}
BasicClass::BasicClass(BasicClass *parent)
{
    setModuleManager(parent->m_moduleManager);
    setSettings(parent->m_settings);
    setNotes(parent->m_notes);
    setActions(parent->m_actions);
}

void BasicClass::setModuleManager(ModuleManager *manager)
{
    m_moduleManager = manager;
}
void BasicClass::setSettings(Settings *settings)
{
    m_settings = settings;
}
void BasicClass::setNotes(Notes *notes)
{
    m_notes = notes;
}
void BasicClass::setActions(Actions *actions)
{
    m_actions = actions;
}

void BasicClass::setAll(BasicClass *c)
{
    c->setModuleManager(m_moduleManager);
    c->setSettings(m_settings);
    c->setNotes(m_notes);
    c->setActions(m_actions);
}
