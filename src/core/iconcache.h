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
#ifndef ICONCACHE_H
#define ICONCACHE_H
#include "src/core/singleton.h"
#include <QStyle>
#include <QApplication>
class IconCache : public Singleton<IconCache>
{
    friend class Singleton<IconCache>;

public:
    IconCache();

    QIcon bibleIcon;
    QIcon dictionayIcon;
    QIcon folderIcon;
    QIcon bookIcon;
};

#endif // ICONCACHE_H
