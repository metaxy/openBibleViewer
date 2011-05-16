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
#ifndef SIMPLEMODULECLASS_H
#define SIMPLEMODULECLASS_H
#include "src/core/notes/notes.h"
#include "src/core/settings/settings.h"
#include "src/module/modulemap.h"
#include "src/module/module.h"
#include "src/core/obvcore.h"
/**
 * It is an abstract class for classes like bible or dictionary, which are meta modules.
 * They contains other similiar modules to provide an simple access to them.
 * These are not the real modules, they only link to the real modules.
 */

class SimpleModuleClass
{
public:
    SimpleModuleClass();
    virtual ~SimpleModuleClass();
    virtual void setSettings(Settings *settings);
    virtual void setNotes(Notes *n);
    void setModuleType(const OBVCore::ModuleType &type);
    OBVCore::ModuleType moduleType() const;

    void setModuleMap(ModuleMap *map);
    int moduleID() const;
    void setModuleID(const int moduleID);

    virtual QString moduleTitle() const;
    virtual QString moduleShortTitle() const;
    virtual QString moduleUID() const;

protected:
    Module *m_module;
    Settings *m_settings;
    Notes *m_notes;
    ModuleMap *m_map;

    int m_moduleID;
    OBVCore::ModuleType m_moduleType;
private:

};

#endif // SIMPLEMODULECLASS_H
