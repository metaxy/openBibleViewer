#ifndef MODULECACHE_H
#define MODULECACHE_H
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
class ModuleCache {
public:
    ModuleCache();
    QString title;
    QMap<int,int> bookCount;
    QStringList bookNames;
    QMap<QString,QVariant> toStringMap();
    void setBookCount(QMap<QString,QVariant> count);
};

#endif // MODULECACHE_H
