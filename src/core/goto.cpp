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
#include "src/core/goto.h"

GoTo::GoTo(int currentBibleID, Versification *v11n)
{
    m_currentBibleID = currentBibleID;
    m_v11n = v11n;
}
VerseUrl GoTo::getUrl(const QString& text)
{
    if(text.size() < 1)
        return VerseUrl();

    QStringList reg;
    QRegExp foundRegExp;
    reg << "(.*)" << "(.*)(\\s+)(\\d+)"  << "(.*)(\\s+)(\\d+),(\\d+)" << "(.*)(\\s+)(\\d+):(\\d+)";
    int found = -1;
    for(int i = reg.size() - 1; i >= 0; --i) {
        QRegExp r(reg.at(i));
        int pos = r.indexIn(text);
        if(pos > -1) {
            found = i;
            foundRegExp = r;
            break;
        }
    }

    VerseUrlRange range;
    if(found == 0) {  //example: Hiob
        const int bookID = bookNameToBookID(foundRegExp.cap(1));
        range.setBook(bookID);
        range.setChapter(VerseUrlRange::LoadFirstChapter);
        range.setWholeChapter();
    } else if(found == 1) {  //Hiob 4
        const int bookID = bookNameToBookID(foundRegExp.cap(1));
        const int chapterID = foundRegExp.cap(3).toInt() - 1;
        range.setBook(bookID);
        range.setChapter(chapterID);
        range.setWholeChapter();
    } else if(found == 2 || found == 3) {  //Hiob 4:9
        const int bookID = bookNameToBookID(foundRegExp.cap(1));
        const int chapterID = foundRegExp.cap(3).toInt() - 1;
        const int verseID = foundRegExp.cap(4).toInt() - 1;
        range.setBook(bookID);
        range.setChapter(chapterID);
        range.setActiveVerse(verseID);
    }
    VerseUrl url(range);
    return url;
}
int GoTo::bookNameToBookID(const QString& name)
{
    int min = -1, bookID = -1;
    QHash<int, QString> full = m_v11n->bookNames();
    QHash<int, QStringList> shortNames = m_v11n->multipleBookShortNames();
    {
        QHashIterator<int, QString> i(full);
        while(i.hasNext()) {
            i.next();
            if(i.value() == name) {
                bookID = i.key();
                break;
            }
        }
    }
    if(bookID == -1) {
        QHashIterator<int, QStringList> i(shortNames);
        while(i.hasNext() && bookID != -1) {
            i.next();
            foreach(const QString & n, i.value()) {
                if(name == n) {
                    bookID = i.key();
                    break;
                }
            }
        }
    }
    if(bookID == -1) {
        QHashIterator<int, QString> i(full);
        while(i.hasNext()) {
            i.next();
            if(i.value().startsWith(name, Qt::CaseInsensitive)) {
                bookID = i.key();
                break;
            }
        }
    }
    if(bookID == -1) {
        QHashIterator<int, QStringList> i(shortNames);
        while(i.hasNext() && bookID != -1) {
            i.next();
            foreach(const QString & n, i.value()) {
                if(n.startsWith(name, Qt::CaseInsensitive)) {
                    bookID = i.key();
                    break;
                }
            }
        }
    }
    if(bookID == -1) {
        {
            QHashIterator<int, QString> i(full);
            while(i.hasNext()) {
                i.next();
                int lev = levenshteinDistance(name, i.value());
                if(lev < min || min < 0) {
                    bookID = i.key();
                    min = lev;
                }
            }
        }
        {
            QHashIterator<int, QStringList> i(shortNames);
            while(i.hasNext() && bookID != -1) {
                i.next();
                foreach(const QString & n, i.value()) {
                    int lev = levenshteinDistance(name, n);
                    if(lev < min || min < 0) {
                        bookID = i.key();
                        min = lev;
                    }
                }
            }
        }
    }

    return bookID;
}
int GoTo::levenshteinDistance(const QString& s, const QString& t)
{
    QByteArray array;
    const int m = s.length();
    const int n = t.length();
    const int dWidth = m + 1;

    // make sure the matrix is big enough

    if(array.size() < (m + 1) *(n + 1)) {
        array.resize((m + 1) *(n + 1));
    }

    int i;

    int j;

    // init 0..m, 0..n as starting values - distance to ""

    for(i = 0; i <= m; i++) {
        array[i + 0*dWidth] = i;
    }
    for(j = 0; j <= n; j++) {
        array[0 + j*dWidth] = j;
    }
    int cost;
    for(i = 1; i <= m; i++) {
        for(j = 1; j <= n; j++) {
            if(s[i-1] == t[j-1]) {
                // if current char is equal, no cost for substitution
                cost = 0;
            } else {
                cost = 1;
            }

            array[i + j*dWidth] = qMin(qMin(// min of three possibilities
                                           array[i-1 + (j)*dWidth] + 1,// deletion
                                           array[i   + (j-1)*dWidth] + 1),// insertion
                                       array[i-1 + (j-1)*dWidth] + cost);// substitution
        }
    }

    return array[m + n*dWidth];
}
