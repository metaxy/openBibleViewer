#include "biblelink.h"
#include <QtCore/QRegExp>
const QStringList REGS = QStringList() << "(.*)"
                                       << "(.*)(\\s+)(\\d+)"
                                       << "(.*)(\\s+)(\\d+),(\\d+)" << "(.*)(\\s+)(\\d+):(\\d+)"
                                       << "(.*)(\\s+)(\\d+),(\\d+)-(\\d+)" << "(.*)(\\s+)(\\d+):(\\d+)-(\\d+)"
                                       << "(.*)(\\s+)(\\d+)-(\\d+)";

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
    VerseUrl url;
    if(found == 0) {  //example: Hiob
        const int bookID = bookNameToBookID(foundRegExp.cap(1),&lev);
        range.setBook(bookID);
        range.setChapter(VerseUrlRange::LoadFirstChapter);
        range.setWholeChapter();
        url.addRange(range);
    } else if(found == 1) {  //Hiob 4
        const int bookID = bookNameToBookID(foundRegExp.cap(1),&lev);
        const int chapterID = foundRegExp.cap(3).toInt() - 1;
        range.setBook(bookID);
        range.setChapter(chapterID);
        range.setWholeChapter();
        url.addRange(range);
    } else if(found == 2 || found == 3) {  //Hiob 4:9
        const int bookID = bookNameToBookID(foundRegExp.cap(1),&lev);
        const int chapterID = foundRegExp.cap(3).toInt() - 1;
        const int verseID = foundRegExp.cap(4).toInt() - 1;
        range.setBook(bookID);
        range.setChapter(chapterID);
        range.setStartVerse(verseID);
        range.setEndVerse(verseID);
        url.addRange(range);
    }
    else if(found == 4 || found == 5) {  //Hiob 4:9-10
        const int bookID = bookNameToBookID(foundRegExp.cap(1),&lev);
        const int chapterID = foundRegExp.cap(3).toInt() - 1;
        const int startVerseID = foundRegExp.cap(4).toInt() - 1;
        const int endVerseID = foundRegExp.cap(5).toInt() - 1;
        range.setBook(bookID);
        range.setChapter(chapterID);
        range.setStartVerse(startVerseID);
        range.setEndVerse(endVerseID);
        url.addRange(range);
    }
    else if(found == 6) {  //Hiob 4-5
        const int bookID = bookNameToBookID(foundRegExp.cap(1),&lev);
        const int startChapterID = foundRegExp.cap(3).toInt() - 1;
        const int endChapterID = foundRegExp.cap(4).toInt() - 1;
        for(int i=startChapterID; i<=endChapterID;i++) {
            VerseUrlRange range;
            range.setModule(m_moduleID);
            range.setOpenToTransformation(true);
            range.setBook(bookID);
            range.setChapter(i);
            range.setWholeChapter();
            url.addRange(range);
        }
    }

    return url;
}
int BibleLink::bookNameToBookID(QString name, int *nlev)
{
    int min = -1, bookID = -1;
    name = name.toLower();

    QHash<int, QString> full = m_v11n->bookNames();
    QHash<int, QStringList> shortNames = m_v11n->multipleBookShortNames();

    QMutableHashIterator<int, QString> it(full);
    while (it.hasNext()) {
        it.next();
        it.setValue(it.value().toLower());
    }

    QMutableHashIterator<int, QStringList> it2(shortNames);
    while (it2.hasNext()) {
        it2.next();
        QStringList newList;
        foreach(QString n, it2.value()) {
            newList << n.toLower();
        }
        it2.setValue(newList);

    }

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
