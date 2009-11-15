#ifndef BIBLEINFO_H
#define BIBLEINFO_H
#include <QtCore/QString>
#include <QtCore/QStringList>
struct bibleInfo
{
    QString bibleName;
    QString path;
    QString folder;
    QStringList bookFullNames;
    QStringList bookShortNames;
    //todo::moduleConfig


};
#endif // BIBLEINFO_H
