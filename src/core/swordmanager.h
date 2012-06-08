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
#ifndef SWORDMANAGER_H
#define SWORDMANAGER_H
#include "src/core/dbghelper.h"
#include "src/core/singleton.h"
#ifdef BUILD_WITH_SWORD
#include <swmgr.h>

class SwordManager : public Singleton<SwordManager>
{
    friend class Singleton<SwordManager>;
public:
   ~SwordManager();
    sword::SWMgr * getManager() const;
protected:
    SwordManager();
private:
    sword::SWMgr *m_library;
};
#else
class SwordManager
{
public:
    SwordManager();
};

#endif //BUILD_WITH_SWORD

#endif // SWORDMANAGER_H
