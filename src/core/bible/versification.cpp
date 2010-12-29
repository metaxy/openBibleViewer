#include "versification.h"
#include "src/core/dbghelper.h"
Versification::Versification()
{
}
Versification::~Versification()
{
}
BookNames Versification::toBookNames(VersificationFilterFlags filter)
{
    DEBUG_FUNC_NAME
    QHash<int, QString> bookFullName;
    QHash<int, QStringList> bookShortName;
    QList<int> bookIDs;

    QStringList bNames = getBookNames(filter);
    QList<QStringList> bShortNames = multipleBookShortNames(filter);

    myDebug() << bNames;
    for(int i = 0; i < bNames.size(); i++) {
        bookFullName.insert(i, bNames.at(i));
        bookShortName.insert(i, bShortNames.at(i));
        bookIDs.append(i);
    }

    BookNames ret;
    ret.m_bookFullName = bookFullName;
    ret.m_bookShortName = bookShortName;
    ret.m_bookIDs = bookIDs;
    return ret;
}
QMap<int, int> Versification::toBookCount(VersificationFilterFlags filter)
{
    DEBUG_FUNC_NAME
    QMap<int, int> ret;
    QList<int> mChapter = maxChapter(filter);
    for(int i = 0; i < mChapter.size(); i++) {
        ret.insert(i, mChapter.at(i));
    }
    return ret;
}
