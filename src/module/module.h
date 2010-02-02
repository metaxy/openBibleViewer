#ifndef MODULE_H
#define MODULE_H
#include "src/core/settings.h"
#include <QtCore/QString>
class Module
{
public:
    Module();
    enum ModuleClass {
        NoneType = 0,
        BibleModule = 1,
        StrongModule = 2
    };
    enum ModuleType {
        NoneClass = 0,
        BibleQuoteModule = 1,
        ZefaniaBibleModule = 2,
        ZefaniaStrongModule = 3
    };
    int m_moduleClass;
    int m_moduleType;
    QString m_path;
    QString m_title;
    int m_id;
    void setSettings(Settings *settings);
    Settings *m_settings;
};

#endif // MODULE_H
