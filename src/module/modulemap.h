#ifndef MODULEMAP_H
#define MODULEMAP_H
#include "biblequote.h"
#include "zefania-bible.h"
#include "module.h"
#include <QtCore/QCache>
class ModuleMap
{
public:
    ModuleMap();
    QMap<int, Module*> m_map;
};

#endif // MODULEMAP_H
