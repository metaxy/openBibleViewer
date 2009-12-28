#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H
#include "src/module/module.h"
#include "src/core/settings.h"
#include "src/module/bible.h"
#include <QtCore/QList>
#include <QtGui/QTreeWidgetItem>
class ModuleManager
{
public:
    ModuleManager();
    void setSettings(Settings *settings);
    int loadAllModules();
    QList<Module> m_moduleList;
    QList<QTreeWidgetItem *> m_items;
    Settings *m_settings;
    Bible m_bible;

};

#endif // MODULEMANAGER_H
