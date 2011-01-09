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
#ifndef VERSIFICATION_BIBLEQUOTE_H
#define VERSIFICATION_BIBLEQUOTE_H

#include "src/core/verse/versification.h"

class Versification_BibleQuote : public Versification
{
public:
    Versification_BibleQuote(const QStringList &fullNames, const QList<QStringList> &shortNames, const QMap<int, int> &bookCount);
private:
    bool filter(const int &bookID, VersificationFilterFlags flags) const;
};

#endif // VERSIFICATION_BIBLEQUOTE_H
