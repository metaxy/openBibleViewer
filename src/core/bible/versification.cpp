#include "versification.h"

Versification::Versification()
{
}
Versification::~Versification()
{
}
BookNames Versification::toBookNames()
{
    QHash<int, QString> bookFullName;
    QHash<int, QStringList> bookShortName;
    QList<int> bookIDs;
    for(int i = 0; i < m_bookNames.size(); i++) {
        bookFullName.insert(i, m_bookNames.at(i));
        bookShortName.insert(i, m_bookShortNames.at(i));
        bookIDs.append(i);
    }

    BookNames ret;
    ret.m_bookFullName = bookFullName;
    ret.m_bookShortName = bookShortName;
    ret.m_bookIDs = bookIDs;
    return ret;
}
