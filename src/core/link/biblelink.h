#ifndef BIBLELINK_H
#define BIBLELINK_H
#include <QtCore/QRegExp>
#include "src/core/verse/versification.h"
#include "src/core/dbghelper.h"
#include "src/core/link/verseurl.h"

class BibleLink
{
public:
    BibleLink(int moduleID, Versification *v11n);
    bool isBibleLink(const QString &s);
    VerseUrl getUrl(const QString& text);
private:
    int bookNameToBookID(QString name,int *nlev);
    int levenshteinDistance(const QString& s, const QString& t);
    int m_moduleID;
    QStringList m_bookFullName;
    QList<QStringList> m_bookShortName;
    Versification *m_v11n;
};

#endif // BIBLELINK_H
