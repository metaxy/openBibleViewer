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
#ifndef ZEFANIABIBLE_H
#define ZEFANIABIBLE_H
#include <QtCore/QFile>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QRegExp>
#include <QtCore/QTimer>

#include <QtGui/QMessageBox>
#include <QtGui/QProgressDialog>

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomAttr>
#include <QtXml/QtXml>

#include "src/core/verse/verseurl.h"
#include "src/core/verse/verse.h"
#include "src/core/verse/chapter.h"
#include "src/core/verse/versification/versification_zefania.h"

#include "src/core/settings/modulesettings.h"

#include "src/core/KoXmlWriter.h"
#include "src/core/dbghelper.h"
#include "src/core/KoXmlReader.h"

#include "src/module/bible/biblemodule.h"


/**
 * ZefaniaBible represents a zefaniaxml(bible) module.
 */
class ZefaniaBible : public BibleModule
{

public:
    ZefaniaBible();
    int readBook(const int id);
    int loadBibleData(const int id, const QString &path);

    QString readInfo(QFile &file);
    /**
      Read the module file and returns the bible name
      \param fileName The fileName of the module.
     */
    QString readInfo(const QString &fileName);

    void search(const SearchQuery &query, SearchResult *res) const;
    bool hasIndex() const;
    void buildIndex();

    int moduleID() const;
    QString modulePath() const;
    QString moduleName(bool preferShortName = false) const;
    Book book() const;
    QString uid() const;
    TextRange rawTextRange(int bookID, int chapterID, int startVerse, int endVerse);
    std::pair<int, int> minMaxVerse(int bookID, int chapterID);

    void removeHardCache(const QString &path);
private:
    QDomElement* format(QDomElement* e);

    /**
      * Checks if there are cache files for a given module. If not it returns false.
      * \param path The path of the module.
     */
    bool checkForCacheFiles(const QString &path) const;
    /**
      * Reads the entire xml file and if caching is enabled, generates cache file.
      * \param id The ID of the module(bible).
      * \param path. Path to the module file.
      */
    int loadNoCached(const int id, const QString &path);
    /**
      * Load only booknames and not every book and his data
      */
    int loadCached(const int id, const QString &path);
    Book fromHardToSoft(const int id, const QDomNode *ncache);

    QHash<int, Book> m_softCacheData;

    void clearSoftCache();
    void setSoftCache(const QHash<int, Book >&softCache);
    void setSoftCache(const int bookID, const Book &b);
    QHash<int, Book> softCache() const;
    Book softCache(const int bookID) const;
    QDomNode readBookFromHardCache(QString path, int bookID);

    QString indexPath() const;

    int m_bookID;
    int m_moduleID;
    QString m_modulePath;
    QString m_moduleName;
    Book m_book;
    QString m_uid;

};

#endif // ZEFANIABIBLE_H
