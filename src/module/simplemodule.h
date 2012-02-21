#ifndef SIMPLEMODULE_H
#define SIMPLEMODULE_H
#include "src/module/metainfo.h"
#include "src/core/settings/settings.h"
class SimpleModule
{
public:
    SimpleModule();
    void setSettings(Settings *settings);
    virtual MetaInfo readInfo(const QString &name) = 0;
    virtual bool loaded() const;
protected:
    Settings *m_settings;
    bool m_loaded;
};

#endif // SIMPLEMODULE_H
