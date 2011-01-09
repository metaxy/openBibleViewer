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
#ifndef MODULECACHE_H
#define MODULECACHE_H
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
class ModuleCache
{
public:
    ModuleCache();
    QString title;
    QMap<int, int> bookCount;
    QHash<int, QString> bookNames;
    QMap<QString, QVariant> toStringMap() const;
    void setBookCount(QMap<QString, QVariant> count);
};

#endif // MODULECACHE_H
