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
#ifndef ZEFANIA_H
#define ZEFANIA_H
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QMap>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include "../core/config.h"
#include "../core/stelle.h"
#include "../core/chapter.h"
#include "../core/moduleconfig.h"
#include "../core/KoXmlReader.h"

class zefaniaBible
{

private:
    struct settings_s zefset;
    struct moduleConfig mConfig;
    QString formatfromini(QString input);
    QDomElement format(QDomElement e);
public:
    zefaniaBible();
    int setSettings(struct settings_s settings, struct moduleConfig mConfig);
    void readBook(int id);
    void loadBibleData(int id, QString path);
    QString readInfo(QFile &file);
    QString readInfo(QString content);
    void generateCacheFile(QString path, QStringList bookFullName_, QMap<int, KoXmlElement> cache);
    bool checkForCacheFiles(QString path);
    void loadNoCached(int id, QString path);
    void loadCached(int id, QString path);
    QDomNode readBookFromHardCache(QString path, int bookID);
    struct stelle search(QString searchstring, bool regexp, bool whole, bool casesen);

    QMap<int, QList<chapter> > softCacheData;
    QMap<int, bool> softCacheAvi;
    long currentBookID, bibletype, /*book_ccount,*/ bibleID;
    struct stelle st;
    QString currentBiblePath, lastout, chaptersign, versesign, biblepath, bibleName, lastSearch;
    QStringList bibles, biblesPath, bookFullName, bookShortName, /*chapterText,bookPath*/;
    QMap <int, int> bookCount;
    QList<chapter> chapterData;
    QMap<int, QList<chapter> > softCache();
    QList<chapter>  softCache(int bookID);
    bool setSoftCache(QMap<int, QList<chapter> >);
    bool setSoftCache(int bookID, QList<chapter> c);
    bool clearSoftCache();
    QList<chapter> fromHardToSoft(int id, QDomNode ncache);

};

#endif // ZEFANIA_H
