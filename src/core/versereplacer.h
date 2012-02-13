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
#ifndef VERSEREPLACER_H
#define VERSEREPLACER_H
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QStringList>
#include "src/core/verse/verse.h"
class VerseReplacer
{
public:
    VerseReplacer();
    void setInsert(const int verseID, const int pos, const QString &insert);
    void setPrepend(const int verseID, const QString &prepend);
    void setAppend(const int verseID, const QString &append);

    void exec(QMap<int, Verse> *list);
private:
    QMap<int, QMap<int, QString> > m_inserts;
    QMap<int, QString> m_prepends;
    QMap<int, QString> m_appends;
};

#endif // VERSEREPLACER_H
