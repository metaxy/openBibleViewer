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
#ifndef VERSIFICATION_ZEFANIA_H
#define VERSIFICATION_ZEFANIA_H
#include "src/core/verse/versification.h"
class Versification_Zefania : public Versification
{
    Q_OBJECT
public:
    Versification_Zefania(const QStringList &fullNames, const QStringList &shortNames, const QStringList &bookIDs);
    void setMaxChapter(const int bookID, const int maxChapter);
private:
    bool filter(const int bookID, const VersificationFilterFlags flags) const;
};

#endif // VERSIFICATION_ZEFANIA_H
