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
#include "versereplacer.h"
#include <QtCore/QMutableMapIterator>
#include <QtCore/QStack>
#include "src/core/dbghelper.h"
#include "src/core/bible/verse.h"
template<class T> VerseReplacer<T>::VerseReplacer()
{
}

template<class T> void VerseReplacer<T>::setInsert(const int &verseID, const int &pos, const QString &insert)
{
    QMap<int, T> m = m_inserts[verseID];
    m[pos] += insert;
    m_inserts[verseID] = m;
}

template<class T> void VerseReplacer<T>::setPrepend(const int &verseID, const QString &prepend)
{
    m_prepends[verseID] = prepend + m_prepends[verseID];
}

template<class T> void VerseReplacer<T>::setAppend(const int &verseID, const QString &append)
{
    m_appends[verseID] = m_appends[verseID] + append;
}


template<class T> void VerseReplacer<T>::exec(QMap<int, T> *map)
{
    QMutableMapIterator<int, T> i(map);
    while (i.hasNext()) {
        i.next();
        qDebug() << i.key() << ": " << i.value();
        if(m_appends.contains(i.key()) || m_prepends.contains(i.key()) || m_inserts.contains(i.key())) {
            QStack<int> posStack;
            QStack<T> stringStack;
            QMapIterator<int, T> it(m_inserts.value(i.key()));
            while(it.hasNext()) {
                it.next();
                posStack.push(it.key());
                stringStack.push(it.value());
            }
            T vers = i.value();
            while(!posStack.isEmpty()) {
                int pos = posStack.pop();
                T in = stringStack.pop();
                vers.insert(pos, in);
            }
            vers.append(m_appends.value(i.key()));
            vers.prepend(m_prepends.value(i.key()));
            map->insert(i.key(), vers);
        }
    }
}
