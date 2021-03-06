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
#ifdef BUILD_WITH_SWORD
#include "swordmanager.h"
#include <encfiltmgr.h>
#include <markupfiltmgr.h>
#include <swmgr.h>
#include <swmodule.h>
using namespace::sword;

SwordManager::SwordManager()
{
    m_library = new SWMgr(new MarkupFilterMgr(FMT_PLAIN));
}
SwordManager::~SwordManager()
{
    if(m_library != NULL) {
        delete m_library;
    }
}

sword::SWMgr * SwordManager::getManager() const
{

    return m_library;
}
#endif
