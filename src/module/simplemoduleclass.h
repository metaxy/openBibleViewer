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
#ifndef SIMPLEMODULECLASS_H
#define SIMPLEMODULECLASS_H
#include "src/core/notes.h"
#include "src/core/settings/settings.h"
#include "src/module/modulemap.h"
#include "src/module/module.h"
class SimpleModuleClass
{
public:
    SimpleModuleClass();
    virtual void setSettings(Settings *settings);
    virtual void setNotes(Notes *n);
    void setModuleType(const Module::ModuleType &type);
    void setModuleMap(ModuleMap *map);
    int moduleID();

protected:
    int m_moduleID;
    Settings *m_settings;
    Notes *m_notes;
    ModuleMap *m_map;
    Module::ModuleType m_moduleType;
private:

};

#endif // SIMPLEMODULECLASS_H
