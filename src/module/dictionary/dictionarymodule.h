#ifndef DICTIONARYMODULE_H
#define DICTIONARYMODULE_H
#include "src/core/settings/settings.h"

class DictionaryModule
{
public:
    DictionaryModule();
    void setSettings(Settings *settings);
    void setID(int moduleID, const QString &path);

    virtual QString getEntry(const QString &entry);
    virtual QStringList getAllKeys();

protected:

    QString m_modulePath;
    int m_moduleID;
    Settings *m_settings;
};

#endif // DICTIONARYMODULE_H
