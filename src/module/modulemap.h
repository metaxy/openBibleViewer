#ifndef MODULEMAP_H
#define MODULEMAP_H
#include "biblequote.h"
#include "zefania-bible.h"
#include <QtCore/QCache>
class ModuleMap
{
public:
    ModuleMap();
    QMap<int, BibleQuote*> m_bibleQuote;
    QMap<int, ZefaniaBible*> m_zefaniaBible;
};

#endif // MODULEMAP_H
