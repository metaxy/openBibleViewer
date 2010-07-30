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
#ifndef ZEFANIABIBLE_H
#define ZEFANIABIBLE_H
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QMap>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include "src/core/settings.h"
#include "src/core/chapter.h"
#include "src/core/modulesettings.h"
#include "src/core/searchquery.h"
#include "src/core/searchresult.h"
#include "src/core/KoXmlReader.h"
/*!
 ZefaniaBible represents a zefaniaxml(bible) module

 @author Paul Walger <metaxy@walger.name>
*/
class ZefaniaBible
{

public:
    ZefaniaBible();
    void setSettings(Settings *settings);
    void readBook(const int &id);
    void loadBibleData(const int &id, const QString &path);
    QString readInfo(QFile &file);
    QString readInfo(const QString &content);

    void search(SearchQuery query, SearchResult *res);
    bool hasIndex();
    void buildIndex();

    int m_bookID;
    int m_bibleID;
    QString m_biblePath;
    QString m_bibleName;
    QStringList m_bookFullName;
    QStringList m_bookShortName;
    QMap <int, int> m_bookCount;
    QList<Chapter> m_chapterData;

    void removeHardCache(const QString &path);

private:
    Settings *m_settings;
    QDomElement format(QDomElement e);
    bool checkForCacheFiles(const QString &path);
    void loadNoCached(const int &id, const QString &path);
    void loadCached(const int &id, const QString &path);
    QList<Chapter> fromHardToSoft(int id, QDomNode ncache);

    QMap<int, QList<Chapter> > m_softCacheData;

    void clearSoftCache();
    void setSoftCache(QMap<int, QList<Chapter> >);
    void setSoftCache(int bookID, QList<Chapter> c);
    QMap<int, QList<Chapter> > softCache();
    QList<Chapter> softCache(int bookID);
    QDomNode readBookFromHardCache(QString path, int bookID);

};

#endif // ZEFANIABIBLE_H
