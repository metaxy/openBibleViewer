#ifndef ICONCACHE_H
#define ICONCACHE_H
#include "src/core/singleton.h"
#include <QtGui/QStyle>
#include <QtGui/QApplication>
class IconCache : public Singleton<IconCache>
{
    friend class Singleton<IconCache>;

public:
    IconCache();


    QIcon bibleIcon;
    QIcon dictionayIcon;
    QIcon folderIcon;
    QIcon bookIcon;
};

#endif // ICONCACHE_H
