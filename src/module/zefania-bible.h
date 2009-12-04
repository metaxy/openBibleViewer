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
#include "../core/searchquery.h"
#include "../core/KoXmlReader.h"

class zefaniaBible
{

private:
    struct settings_s zefset;
    struct moduleConfig mConfig;
    QDomElement format(QDomElement e);
public:
    zefaniaBible();
    int setSettings(struct settings_s settings, struct moduleConfig mConfig);
    void readBook(const int &id);
    void loadBibleData(const int &id, const QString &path);
    QString readInfo(QFile &file);
    QString readInfo(const QString &content);
    bool checkForCacheFiles(const QString &path);
    void loadNoCached(const int &id, const QString &path);
    void loadCached(const int &id, const QString &path);
    QDomNode readBookFromHardCache(QString path, int bookID);
    struct stelle search(struct searchQuery query);

    QMap<int, QList<Chapter> > softCacheData;
    long currentBookID, bibleID;
    struct stelle st;
    QString currentBiblePath, lastout, chaptersign, versesign, biblepath, bibleName, lastSearch;
    QStringList bibles, biblesPath, bookFullName, bookShortName, /*chapterText,bookPath*/;
    QMap <int, int> bookCount;
    QList<Chapter> chapterData;

    QMap<int, QList<Chapter> > softCache();
    QList<Chapter> softCache(int bookID);
    bool setSoftCache(QMap<int, QList<Chapter> >);
    bool setSoftCache(int bookID, QList<Chapter> c);
    void clearSoftCache();
    QList<Chapter> fromHardToSoft(int id, QDomNode ncache);
    void removeHardCache(const QString &path);

};

#endif // ZEFANIA_H
