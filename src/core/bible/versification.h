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
#ifndef VERSIFICATION_H
#define VERSIFICATION_H
#include <QtCore/QStringList>
#include <QtCore/QFlags>
#include <QtCore/QMap>
#include "src/core/bible/booknames.h"
class Versification: public QObject
{
    Q_OBJECT
public:
    Versification();
    virtual ~Versification();
    enum VersificationFilter {
        ReturnAll = 0x0,
        ReturnOT = 0x1,
        ReturnNT = 0x2
    };
    Q_DECLARE_FLAGS(VersificationFilterFlags, VersificationFilter)

    virtual QStringList getBookNames(VersificationFilterFlags filter) const = 0;
    virtual QList<QStringList> multipleBookShortNames(VersificationFilterFlags filter) const = 0;
    virtual QStringList bookShortNames(VersificationFilterFlags filter) const = 0;
    virtual QList<int> maxChapter(VersificationFilterFlags filter) const = 0;
    virtual QList< QList<int> > maxVerse(VersificationFilterFlags filter) const = 0;
    BookNames toBookNames(VersificationFilterFlags filter);
    QMap<int, int> toBookCount(VersificationFilterFlags filter);

protected:
    QStringList m_bookNames;
    QList<QStringList> m_bookShortNames;
    QList<int> m_maxChapter;
    QList<QList<int> > m_maxVerse;

};
Q_DECLARE_OPERATORS_FOR_FLAGS(Versification::VersificationFilterFlags)
#endif // VERSIFICATION_H
