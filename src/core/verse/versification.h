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
#ifndef VERSIFICATION_H
#define VERSIFICATION_H
#include <QtCore/QStringList>
#include <QtCore/QFlags>
#include <QtCore/QMap>
#include "src/core/verse/versification/bookv11n.h"
#include "src/core/verse/versificationextendeddata.h"
class Versification : public QObject
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

    void setFlags(VersificationFilterFlags filter);

    QHash<int, QString> bookNames(VersificationFilterFlags filter) const;
    QHash<int, QStringList> multipleBookShortNames(VersificationFilterFlags filter) const;
    QHash<int, QString> bookShortNames(VersificationFilterFlags filter) const;
    QList<int> bookIDs(VersificationFilterFlags filter) const;
    QHash<int, int> maxChapter(VersificationFilterFlags filter) const;
    QHash<int, QList<int> > maxVerse(VersificationFilterFlags filter) const;
    int bookCount(VersificationFilterFlags filter) const;

    QHash<int, QString> bookNames() const;
    QHash<int, QStringList> multipleBookShortNames() const;
    QHash<int, QString> bookShortNames() const;
    QList<int> bookIDs() const;

    QHash<int, int> maxChapter() const;
    QHash<int, QList<int> > maxVerse() const;
    int bookCount() const;

    QString bookName(const int bookID, bool preferShort = false) const;
    /**
      * Use it with care. It doesn't apply the filter,
      */
    QMap<int, BookV11N> data() const;

    VersificationExtendedData extendedData;

    QString name;

protected:
    QMap<int, BookV11N> m_books;
    virtual bool filter(const int bookID, const Versification::VersificationFilterFlags flags) const;
    VersificationFilterFlags m_filter;

};
Q_DECLARE_OPERATORS_FOR_FLAGS(Versification::VersificationFilterFlags)
#endif // VERSIFICATION_H
