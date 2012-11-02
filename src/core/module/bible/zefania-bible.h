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
#ifndef ZEFANIABIBLE_H
#define ZEFANIABIBLE_H
#include <QtCore/QFile>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include <QtGui/QProgressDialog>

#include <QtXml/QtXml>

#include "src/core/obvcore.h"
#include "src/core/link/verseurl.h"
#include "src/core/verse/verse.h"
#include "src/core/verse/chapter.h"
#include "src/core/verse/versification/versification_zefania.h"

#include "src/core/raw/verseblock.h"

#include "src/core/settings/modulesettings.h"

#include "src/core/dbghelper.h"
#include "src/core/raw/bookblock.h"
#include "src/core/raw/chapterblock.h"
#include "src/core/raw/blockidgen.h"

#include "src/core/module/bible/biblemodule.h"

/**
 * ZefaniaBible represents a zefaniaxml(bible) module.
 */
class ZefaniaBible : public BibleModule
{

public:
    ZefaniaBible();
    ~ZefaniaBible();
    int readBook(const int id);
    int loadBibleData(const int id, const QString &path);

    MetaInfo readInfo(QFile &file);
    /**
      Read the module file and returns the bible name
      \param fileName The fileName of the module.
     */
    MetaInfo readInfo(const QString &fileName);

    void search(const SearchQuery &query, SearchResult *res) const;
    bool hasIndex() const;
    void buildIndex();

    int moduleID() const;
    QString modulePath() const;
    QString moduleName(bool preferShortName = false) const;

    QString uid() const;
    TextRange rawTextRange(int bookID, int chapterID, int startVerse, int endVerse);
    std::pair<int, int> minMaxVerse(int bookID, int chapterID);

    void removeHardCache(const QString &path);
    bool hasHardCache(const QString &path);

    void clearData();
private:
    QDomElement* format(QDomElement* e);
    QString indexPath() const;

    int m_bookID;
    int m_moduleID;
    QString m_modulePath;
    QString m_moduleName;
    QString m_uid;
    VerseBook m_book;

    QXmlStreamReader *m_xml;

    VerseBook readBook();
    Chapter readChapter();
    Verse readVerse();
    MetaInfo readMetaInfo(MetaInfo ret);

    QString parseStyle();
    QString parseNote();
    QString parseBr();
    QString parseGram();
    QString parseSup();
    QString parseXRef();
    QString parseDiv();
    
    //Raw
    BlockIDGen m_idGen;
    BookBlock rawReadBook(quint32 parent);
    ChapterBlock rawReadChapter(quint32 parent);
    VerseBlock rawReadVerse(quint32 parent);
    bool cmp(const QStringRef &r, const QString &s);
    void getVersification();
    ModuleSettings *m_set;

    QString getPath();
    void cache(const VerseBook &b);
    void generateCache(QList<std::pair<qint64, qint64> > list);
    QMap<int,QString> m_strongsPrefix;
    void genStrongsPrefix();

    bool m_rightToLeft;
};

#endif // ZEFANIABIBLE_H
