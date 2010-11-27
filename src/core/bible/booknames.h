#ifndef BOOKNAMES_H
#define BOOKNAMES_H
#include <QtCore/QHash>
#include <QtCore/QStringList>
class BookNames
{
public:
    BookNames();
    QHash<int, QString> m_bookFullName;
    QHash<int, QStringList> m_bookShortName;
    QList<int> m_bookIDs;
};

#endif // BOOKNAMES_H
