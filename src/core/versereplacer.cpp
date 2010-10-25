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
VerseReplacer::VerseReplacer()
{
}
void VerseReplacer::setInsert(const int &verseID, const int &pos, const QString &insert)
{
    QMap<int, QString> m = m_inserts[verseID];
    m[pos] = m[pos] + insert;
    m_inserts[verseID] = m;
}

void VerseReplacer::setPrepend(const int &verseID, const QString &prepend)
{
    m_prepends[verseID] = prepend + m_prepends[verseID];
}
void VerseReplacer::setAppend(const int &verseID, const QString &append)
{
    m_appends[verseID] = m_appends[verseID] + append;
}

void VerseReplacer::exec(QStringList *list)
{
    for(int verse = 0; verse < list->size(); verse++) {
        //myDebug() << "verse = " << verse;
        //first insert
        if(m_appends.contains(vers) || m_prepends.contains(vers) || m_inserts.contains(vers)) {
            QStack<int> posStack;
            QStack<QString> stringStack;
            QMapIterator<int, QString> i(m_inserts.value(verse));
            while (i.hasNext()) {
                i.next();
                posStack.push(i.key());
                stringStack.push(i.value());
            }
            QString vers = list->at(verse);
            while (!posStack.isEmpty()) {
                int pos = posStack.pop();
                QString in = stringStack.pop();
                //myDebug() << "pos = " << pos << " in = " << in;
                vers.insert(pos,in);
            }
            //than append and prepend
            vers.append(m_appends.value(verse));
            vers.prepend(m_prepends.value(verse));
            //myDebug() << "vers = " << vers;
            list->replace(verse,vers);
        }
    }
}
