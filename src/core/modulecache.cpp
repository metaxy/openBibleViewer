#include "modulecache.h"

ModuleCache::ModuleCache()
{
}

QMap<QString,QVariant> ModuleCache::toStringMap()
{
    QMap<QString,QVariant> n;
    QMapIterator<int, int> i(bookCount);
    while (i.hasNext()) {
        i.next();
        n[QString::number(i.key())] = QVariant(i.value());
    }
    return n;
}
void ModuleCache::setBookCount(QMap<QString,QVariant> count)
{
    QMapIterator<QString,QVariant> i(count);
    while (i.hasNext()) {
        i.next();
        bookCount[i.key().toInt()] = i.value().toInt();
    }
}
