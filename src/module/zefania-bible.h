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
#include <QtCore/QFile>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include "src/core/book.h"
#include "src/core/chapter.h"
#include "src/core/KoXmlReader.h"
#include "src/core/modulesettings.h"
#include "src/core/searchquery.h"
#include "src/core/searchresult.h"
#include "src/core/settings.h"

/*!
 ZefaniaBible represents a zefaniaxml(bible) module.
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
    bool hasIndex() const;
    void buildIndex();

    int m_bookID;
    int m_bibleID;
    QString m_biblePath;
    QString m_bibleName;
    QStringList m_bookFullName;
    QStringList m_bookShortName;
    QMap<int, int> m_bookCount;
    QStringList m_bookIDs;
    Book m_book;

    void removeHardCache(const QString &path);

private:
    Settings *m_settings;
    QDomElement* format(QDomElement* e);
    bool checkForCacheFiles(const QString &path);
    void loadNoCached(const int &id, const QString &path);
    void loadCached(const int &id, const QString &path);
    Book fromHardToSoft(const int &id, const QDomNode *ncache);

    QHash<int, Book> m_softCacheData;

    void clearSoftCache();
    void setSoftCache(const QHash<int, Book >&softCache);
    void setSoftCache(const int &bookID, const Book &b);
    QHash<int, Book> softCache() const;
    Book softCache(const int &bookID) const;
    QDomNode readBookFromHardCache(QString path, int bookID);

    QString indexPath() const;

};

#endif // ZEFANIABIBLE_H
