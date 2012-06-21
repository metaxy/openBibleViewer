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
#ifndef BIBLELINK_H
#define BIBLELINK_H
#include "src/core/dbghelper.h"
#include <QSharedPointer>
#include "src/core/verse/versification.h"
#include "src/core/link/verseurl.h"
class BibleLink
{
public:
    BibleLink(int moduleID, QSharedPointer<Versification> v11n);
    bool isBibleLink(const QString &s);
    VerseUrl getUrl(const QString& text);
    static bool fastIsBibleLink(const QString &s);
private:
    int bookNameToBookID(QString name, int *nlev);
    int levenshteinDistance(const QString& s, const QString& t);
    int m_moduleID;
    QStringList m_bookFullName;
    QList<QStringList> m_bookShortName;
    QSharedPointer<Versification> m_v11n;
};

#endif // BIBLELINK_H
