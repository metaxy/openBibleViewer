#ifndef BOOKV11N_H
#define BOOKV11N_H
#include <QtCore/QStringList>
class BookV11N
{
public:
    BookV11N();
    QString name;
    QStringList shortNames;
    int bookID;
    int maxChapter;
    QList<int> maxVerse;
};

#endif // BOOKV11N_H
