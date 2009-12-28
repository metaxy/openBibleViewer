#ifndef MODULE_H
#define MODULE_H
#include "src/core/settings.h"
#include <QtCore/QString>
class Module
{
public:
    Module();
    enum ModuleType {
        NoneType = 0,
        BibleModule = 1,
        StrongModule = 2
    };
    enum ModuleClass {
        NoneClass = 0,
        BibleQuoteModule = 1,
        ZefaniaBibleModule = 2,
        ZefaniaStrongModule = 3
    };
    int m_moduleType;
    int m_moduleClass;
    QString m_iniPath;
    QString m_name;
    int m_id;
    void setSettings(Settings *settings);
    Settings *m_settings;
};

#endif // MODULE_H
