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
#include "versereplacer.h"
#include <QStack>
VerseReplacer::VerseReplacer()
{
}

void VerseReplacer::setInsert(const int verseID, const int pos, const QString &insert)
{
    QMap<int, QString> m = m_inserts[verseID];
    m[pos] += insert;
    m_inserts[verseID] = m;
}

void VerseReplacer::setPrepend(const int verseID, const QString &prepend)
{
    m_prepends[verseID] = prepend + m_prepends[verseID];
}

void VerseReplacer::setAppend(const int verseID, const QString &append)
{
    m_appends[verseID] = m_appends[verseID] + append;
}

void VerseReplacer::exec(QMap<int, Verse> *map)
{
    QMutableMapIterator<int, Verse> i(*map);
    while(i.hasNext()) {
        i.next();
        if(m_appends.contains(i.key()) || m_prepends.contains(i.key()) || m_inserts.contains(i.key())) {
            QStack<int> posStack;
            QStack<QString> stringStack;
            QMapIterator<int, QString> it(m_inserts.value(i.key()));
            while(it.hasNext()) {
                it.next();
                posStack.push(it.key());
                stringStack.push(it.value());
            }
            while(!posStack.isEmpty()) {
                const int pos = posStack.pop();
                const QString in = stringStack.pop();
                i.value().insert(pos, in);
            }
            i.value().append(m_appends.value(i.key()));
            i.value().prepend(m_prepends.value(i.key()));

        }
    }
}
