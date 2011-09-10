#include "biblelink.h"
#include <QtCore/QRegExp>
const QStringList REGS = QStringList() << "(.*)"
                                       << "(.*)(\\s+)(\\d+)"
                                       << "(.*)(\\s+)(\\d+),(\\d+)" << "(.*)(\\s+)(\\d+):(\\d+)"
                                       << "(.*)(\\s+)(\\d+),(\\d+)-(\\d+)" << "(.*)(\\s+)(\\d+):(\\d+)-(\\d+)";

BibleLink::BibleLink(int moduleID, Versification *v11n)
{
    m_moduleID = moduleID;
    m_v11n = v11n;
}
bool BibleLink::isBibleLink(const QString &s)
{
    QStringList reg = REGS;
    int found = -1;
    QRegExp foundRegExp;
    for(int i = reg.size() - 1; i >= 0; --i) {
        QRegExp r(reg.at(i));
        int pos = r.indexIn(s);
        if(pos > -1) {
            found = i;
            foundRegExp = r;
            break;
        }
    }
    if(found > 0)
        return true;

    if(found == 0) {
        int lev = 0;
        bookNameToBookID(foundRegExp.cap(1),&lev);
        myDebug() << "lev = " << lev;
        if(lev < 2)
            return true;
        else
            return false;
    }
    return false;
}

VerseUrl BibleLink::getUrl(const QString& s)
{
    if(s.isEmpty())
        return VerseUrl();

    QStringList reg = REGS;
    QRegExp foundRegExp;
    int found = -1;
    for(int i = reg.size() - 1; i >= 0; --i) {
        QRegExp r(reg.at(i));
        int pos = r.indexIn(s);
        if(pos > -1) {
            found = i;
            foundRegExp = r;
            break;
        }
    }
    int lev = 0;

    VerseUrlRange range;
    range.setModule(m_moduleID);
    range.setOpenToTransformation(true);
    if(found == 0) {  //example: Hiob
        const int bookID = bookNameToBookID(foundRegExp.cap(1),&lev);
        range.setBook(bookID);
        range.setChapter(VerseUrlRange::LoadFirstChapter);
        range.setWholeChapter();
    } else if(found == 1) {  //Hiob 4
        const int bookID = bookNameToBookID(foundRegExp.cap(1),&lev);
        const int chapterID = foundRegExp.cap(3).toInt() - 1;
        range.setBook(bookID);
        range.setChapter(chapterID);
        range.setWholeChapter();
    } else if(found == 2 || found == 3) {  //Hiob 4:9
        myDebug() << "found one verse";
        const int bookID = bookNameToBookID(foundRegExp.cap(1),&lev);
        const int chapterID = foundRegExp.cap(3).toInt() - 1;
        const int verseID = foundRegExp.cap(4).toInt() - 1;
        range.setBook(bookID);
        range.setChapter(chapterID);
        range.setStartVerse(verseID);
        range.setEndVerse(verseID);
    }
    else if(found == 4 || found == 5) {  //Hiob 4:9-10
        myDebug() << "found range";
        const int bookID = bookNameToBookID(foundRegExp.cap(1),&lev);
        const int chapterID = foundRegExp.cap(3).toInt() - 1;
        const int startVerseID = foundRegExp.cap(4).toInt() - 1;
        const int endVerseID = foundRegExp.cap(5).toInt() - 1;
        range.setBook(bookID);
        range.setChapter(chapterID);
        range.setStartVerse(startVerseID);
        range.setEndVerse(endVerseID);
    }
    VerseUrl url(range);
    myDebug() << "url = " <<url.toString();
    return url;
}
int BibleLink::bookNameToBookID(const QString& name, int *nlev)
{
    int min = -1, bookID = -1;
    QHash<int, QString> full = m_v11n->bookNames();
    QHash<int, QStringList> shortNames = m_v11n->multipleBookShortNames();
    {
        QHashIterator<int, QString> i(full);
        while(i.hasNext()) {
            i.next();
            if(i.value() == name) {
                *nlev = 0;
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
                    *nlev = 0;
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
                *nlev = 3;
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
                    *nlev = 3;
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
                const int lev = levenshteinDistance(name, i.value());
                if(lev < min || min < 0) {
                    *nlev = lev;
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
                    const int lev = levenshteinDistance(name, n);
                    if(lev < min || min < 0) {
                        *nlev = lev;
                        bookID = i.key();
                        min = lev;
                    }
                }
            }
        }
    }

    return bookID;
}
int BibleLink::levenshteinDistance(const QString& s, const QString& t)
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
