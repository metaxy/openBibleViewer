#ifndef SETTINGSLOADER_H
#define SETTINGSLOADER_H
#include <QtCore/QSettings>
#include "src/core/settings/settings.h"

class SettingsLoader
{

public:
    SettingsLoader(QSettings* settingsFile, const QString homeDataPath);
    ~SettingsLoader();
    void loadSettings();
    void saveSettings();
    
    Settings *settings() const;
private:
    QSettings *m_settingsFile;
    Settings *m_settings;
    QSettings* m_sessionFile;
    QString m_homeDataPath;
};

#endif // SETTINGSLOADER_H
