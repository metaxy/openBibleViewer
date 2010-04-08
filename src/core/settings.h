/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
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
#include "modulesettings.h"
#include "session.h"
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
    QList<ModuleSettings> module;
    QMap<int, int> moduleID;
    qreal zoomstep;
    QString version;
    QString build;
    bool removeHtml;
    QString language;
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
    void setBookCount(const int &bibleID, QMap<int, int> bookCount);
    void setBookNames(const int &bibleID, QStringList bookNames);
    void setBibleName(const int &bibleID, QString bibleName);
    void setBiblePath(const int &bibleID, QString biblePath);
    int getCacheID(int bibleID);//Names Cache
    QList<QMap<int, int> > getBookCount();
    QList<QStringList> getBookNames();
    QStringList getBibleName();
    QStringList getBiblePath();
    void clearNamesCache(const int &id);
    Session session;
    QString sessionID;
    QStringList sessionNames;
    QStringList sessionIDs;
};

#endif // SETTINGS_H
