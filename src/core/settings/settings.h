/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include "src/core/settings/modulecache.h"
#include "src/core/verse/versification.h"
#include <QtCore/QStringList>
#include <QtCore/QMap>

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
    QHash<int, ModuleSettings *> m_moduleSettings;

    qreal zoomstep;
    QString version;
    QString build;
    bool removeHtml;
    QString language;
    LayoutEnum autoLayout;
    bool onClickBookmarkGo;

    int textFormatting;//0 = Neue Zeile nach Vers, 1 = Unformatierter Textblock
    bool zefaniaBible_hardCache;
    bool zefaniaBible_softCache;
    QString homePath;
    Versification *defaultVersification;

    ModuleSettings * getModuleSettings(int moduleID) const;

    Session session;
    QString sessionID;
    QStringList sessionNames;
    QStringList sessionIDs;

    QString savableUrl(QString url) const;
    QString recoverUrl(QString url) const;

    QString hash(const QString& path) const;
private:

};

#endif // SETTINGS_H
