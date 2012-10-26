/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
#include "simplemoduleclass.h"

SimpleModuleClass::SimpleModuleClass()
{
    m_moduleID = -1;
}

SimpleModuleClass::~SimpleModuleClass()
{
}

void SimpleModuleClass::setSettings(Settings *settings)
{
    m_settings = settings;
}

void SimpleModuleClass::setNotes(Notes *notes)
{
    m_notes = notes;
}

void SimpleModuleClass::setModuleMap(QSharedPointer<ModuleMap> map)
{
    m_map = map;
}

void SimpleModuleClass::setModuleType(const ModuleTools::ModuleType &type)
{
    m_moduleType = type;
}

int SimpleModuleClass::moduleID() const
{
    return m_moduleID;
}

void SimpleModuleClass::setModuleID(const int moduleID)
{
    m_moduleID = moduleID;
}

QString SimpleModuleClass::moduleTitle() const
{
    return m_settings->getModuleSettings(m_moduleID)->name(false);
}

QString SimpleModuleClass::moduleShortTitle() const
{
    return m_settings->getModuleSettings(m_moduleID)->name(true);
}

QString SimpleModuleClass::moduleUID() const
{
    return m_settings->getModuleSettings(m_moduleID)->moduleUID;
}

ModuleTools::ModuleType SimpleModuleClass::moduleType() const
{
    return m_moduleType;
}

