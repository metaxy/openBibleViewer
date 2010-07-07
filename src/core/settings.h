/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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
#include "src/core/modulesettings.h"
#include "src/core/session.h"
#include "src/core/modulecache.h"
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMap>


/*!
 Settings represents the gobal settings

 @author Paul Walger <metaxy@walger.name>
*/
class Settings
{
public:
    Settings();
    QString encoding;
    QList<ModuleSettings> m_moduleSettings;
    QMap<int, int> moduleID;
    qreal zoomstep;
    QString version;
    QString build;
    bool removeHtml;
    QString language;
    //todo: use enum
    int autoLayout;// 0 = NONE, 1 = vertical tile ,2 = horizontal tile, 3 = cascade
    bool onClickBookmarkGo;
    int textFormatting;//0 = Neue Zeile nach Vers, 1 = Unformatierter Textblock
    bool zefaniaBible_hardCache;
    bool zefaniaBible_softCache;
    QString homePath;
    QStringList bookNames;
    QString lastPlaceSave;


    ModuleSettings getModuleSettings(const int &bibleID);
    void replaceModule(const int &bibleID, ModuleSettings m);

    void setTitle( QString path, QString title);
    void setBookCount(QString path, QMap<int, int> count);
    void setBookNames(QString path, QStringList names);
    ModuleCache getModuleCache(const QString &path);
    QMap<QString,ModuleCache> m_moduleCache;

    Session session;
    QString sessionID;
    QStringList sessionNames;
    QStringList sessionIDs;

    QString savableUrl(QString url);
    QString recoverUrl(QString url);
private:

};

#endif // SETTINGS_H
