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
#ifndef MODULEMAP_H
#define MODULEMAP_H
#include "module.h"
/**
 * ModuleMap contains all pointers to a module in a map. And it deletes them when modulemap is deleted.
 */
class ModuleMap
{
public:
    ModuleMap();
    ~ModuleMap();
    QMap<int, Module*> data;

    Module* module(const int id);
};

#endif // MODULEMAP_H
