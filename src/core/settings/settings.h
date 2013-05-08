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
#ifndef SETTINGS_H
#define SETTINGS_H
#include "src/core/settings/modulesettings.h"
#include "src/core/settings/session.h"
#include "src/core/verse/versification.h"
#include <QStringList>
#include <QMap>
#include <QHash>
/**
 * Settings represents the global settings.
 */
class Settings
{
public:
    Settings();
    ~Settings();
    enum LayoutEnum {
        None = 0,
        Tile = 1,
        VerticalTile = 2,
        HorizontalTile = 3,
        Cascade = 4
    };

    QString encoding;

    QMap<int, ModuleSettings*> m_moduleSettings;

    QString version;
    QString build;
    QString language;
    LayoutEnum autoLayout;
    bool intelligentLayout;
    bool onClickBookmarkGo;
    bool moreCompactUI;
    bool checkForUpdates;

    bool advancedSearchDock_useCurrentModule;

    QString homePath;// e.g /home/asd/.openbible/
    QSharedPointer<Versification> defaultVersification;

    ModuleSettings * getModuleSettings(int moduleID) const;
    QSharedPointer<Versification>  getV11N(const int moduleID) const;


    /**
      * Attention: Session data is alway up to date.
      */
    Session session;
    QString sessionID;
    QStringList sessionNames;
    QStringList sessionIDs;


    /**
      * Replace homePath and settingsPath to make it more portable.
      */
    QString savableUrl(const QString &url);
    /**
      * Recover Urls which were saved with savableUrl().
      */
    QString recoverUrl(QString url) const;
    /**
     * Generate a hash from a string. Use it only for urls, because they are transformed by savableUrl().
     */
    QString hash(const QString& path);
    /**
      * Returns the path to the versification.
      */
    QString v11nFile(const QString &path);
    /**
      * It generates a new module id.
      */
    int newModuleID() const;

    int getDefaultModule(ModuleTools::ContentType c);
    ModuleSettings* newVirtualFolder(const int parentModuleID);
    ModuleSettings * newModuleSettings(const int parentID);
    ModuleSettings * createRoot();
    
    void removeModule(const int moduleID);
    void removeModule(ModuleSettings* module);

private:
    QHash<QString, QString> m_cache;
};

#endif // SETTINGS_H
