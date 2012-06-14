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
#ifndef BASICCLASS_H
#define BASICCLASS_H
#include "src/core/notes/notes.h"
#include "src/module/modulemanager.h"
#include "src/core/settings/settings.h"
#include "src/ui/actions.h"
class BasicClass
{
public:
    BasicClass();
    BasicClass(const BasicClass *parent);

    void copy(const BasicClass *parent);

    void setModuleManager(ModuleManager *manager);
    void setSettings(Settings *settings);
    void setNotes(Notes *notes);
    void setActions(Actions *actions);
    void setAll(BasicClass *c);
protected:
    ModuleManager *m_moduleManager;
    Settings *m_settings;
    Notes *m_notes;
    Actions *m_actions;
};

#endif // BASICCLASS_H
