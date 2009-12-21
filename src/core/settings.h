#ifndef SETTINGS_H
#define SETTINGS_H
#include "modulesettings.h"
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
};

#endif // SETTINGS_H
