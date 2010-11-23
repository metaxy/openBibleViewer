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
#include <QtCore/QMapIterator>
#include <QtCore/QStack>
#include "src/core/dbghelper.h"
template<class T> VerseReplacer<T>::VerseReplacer()
{
}

template<class T> void VerseReplacer<T>::setInsert(const int &verseID, const int &pos, const T &insert)
{
    QMap<int, T> m = m_inserts[verseID];
    m[pos] += insert;
    m_inserts[verseID] = m;
}

template<class T> void VerseReplacer<T>::setPrepend(const int &verseID, const T &prepend)
{
    m_prepends[verseID] = prepend + m_prepends[verseID];
}

template<class T> void VerseReplacer<T>::setAppend(const int &verseID, const T &append)
{
    m_appends[verseID] = m_appends[verseID] + append;
}


template<class T> void VerseReplacer<T>::exec(QList<T> *list)
{
    for(int verse = 0; verse < list->size(); verse++) {
        //myDebug() << "verse = " << verse;
        //first insert
        if(m_appends.contains(verse) || m_prepends.contains(verse) || m_inserts.contains(verse)) {
            QStack<int> posStack;
            QStack<T> stringStack;
            QMapIterator<int, T> i(m_inserts.value(verse));
            while(i.hasNext()) {
                i.next();
                posStack.push(i.key());
                stringStack.push(i.value());
            }
            T vers = list->at(verse);
            while(!posStack.isEmpty()) {
                int pos = posStack.pop();
                T in = stringStack.pop();
                //myDebug() << "pos = " << pos << " in = " << in;
                vers.insert(pos, in);
            }
            //than append and prepend
            vers.append(m_appends.value(verse));
            vers.prepend(m_prepends.value(verse));
            //myDebug() << "vers = " << vers;
            list->replace(verse, vers);
        }
    }
}
