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
#include "src/module/module.h"

Module::Module(Module *parent)
{
    m_bibleModule = 0;
    m_zefaniaLex = 0;
    m_bibleQuoteDict = 0;
    m_parent = parent;
}
Module::~Module()
{
}


void Module::setSettings(Settings *settings)
{
    m_settings = settings;
}
Module* Module::parent() const
{
    return m_parent;
}

void Module::append(Module* child)
{
    m_children.append(child);
}

QString Module::path() const
{
    return m_path;
}
QString Module::title() const
{
    return m_title;
}
int Module::moduleID() const
{
    return m_id;
}
CORE::ModuleClass Module::moduleClass() const
{
    return m_moduleClass;
}
CORE::ModuleType Module::moduleType() const
{
    return m_moduleType;
}

void Module::setPath(const QString &path)
{
    m_path = path;
}

void Module::setTitle(const QString &title)
{
    m_title = title;
}

void Module::setModuleID(const int moduleID)
{
    m_id = moduleID;
}

void Module::setModuleClass(const CORE::ModuleClass &c)
{
    m_moduleClass = c;
}

void Module::setModuleType(const CORE::ModuleType &t)
{
    m_moduleType = t;
}
