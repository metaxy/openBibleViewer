#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H
#include "src/module/module.h"
#include "src/core/settings.h"
#include "src/module/bible.h"
#include "src/module/strong.h"
#include <QtCore/QList>
#include <QtGui/QTreeWidgetItem>
class ModuleManager
{
public:
    ModuleManager();
    void setSettings(Settings *settings);
    int loadAllModules();
    QList<Module> m_moduleList;
    QList<QTreeWidgetItem *> m_bibleItems;
    Settings *m_settings;
    Bible m_bible;
    Strong m_strong;
    bool bibleLoaded();

};

#endif // MODULEMANAGER_H
