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
#ifndef GOTO_H
#define GOTO_H

#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include "src/core/verse/versification.h"
#include "src/core/dbghelper.h"
#include "src/core/verse/verseurl.h"
/**
 * GoTo is a pharser for bible passage into a url
 */
class GoTo
{
public:
    GoTo(int currentBibleID, Versification *v11n);
    VerseUrl getUrl(const QString& text);
private:
    int bookNameToBookID(const QString& name);
    int levenshteinDistance(const QString& s, const QString& t);
    int m_currentBibleID;
    QStringList m_bookFullName;
    QList<QStringList> m_bookShortName;
    Versification *m_v11n;
};

#endif // GOTO_H
