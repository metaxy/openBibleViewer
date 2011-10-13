#ifndef VERSEMODULECACHE_H
#define VERSEMODULECACHE_H
#include <QtCore/QCache>
#include "src/module/versemodule.h"
class VerseModuleCache
{
public:
    VerseModuleCache(int count = 5);

    Versification *getV11N(const int moduleID);

private:
    void loadModule(const int moduleID);
    QCache<int, VerseModule*> m_cache;
};

#endif // VERSEMODULECACHE_H
