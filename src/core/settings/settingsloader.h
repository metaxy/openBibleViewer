/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
#ifndef SETTINGSLOADER_H
#define SETTINGSLOADER_H
#include <QSettings>
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
