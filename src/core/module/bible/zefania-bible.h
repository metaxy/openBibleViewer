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
#include <QFile>
#include <QMap>
#include <QString>
#include <QStringList>

#include <QProgressDialog>

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
#include "src/core/raw/mediablock.h"
#include "src/core/raw/prologblock.h"
#include "src/core/raw/remarksblock.h"
#include "src/core/raw/xreffragment.h"
#include "src/core/raw/captionblock.h"
#include "src/core/raw/textfragment.h"
#include "src/core/raw/brfragment.h"
#include "src/core/raw/styleblock.h"
#include "src/core/raw/gramblock.h"
#include "src/core/raw/supblock.h"
#include "src/core/raw/divblock.h"
#include "src/core/raw/noteblock.h"
#include "src/core/module/bible/biblemodule.h"

/**
 * ZefaniaBible represents a zefaniaxml(bible) module.
 */
//class ZefaniaXmlReader;
class ZefaniaBible : public BibleModule
{

public:
    ZefaniaBible();
    ~ZefaniaBible();
    int readBook(const int id);
    int loadBibleData(const int id, const QString &path);


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
    MetaInfo readInfo(QFile &file);
    QString indexPath() const;

    int m_moduleID;
    QString m_modulePath;
    QString m_moduleName;
    QString m_uid;
    
    void getVersification();
    ModuleSettings *m_set;

    QString getPath();
    void generateCache(QList<std::pair<qint64, qint64> > list);
    QString path(const int book);
};

class ZefaniaXmlReader
{
public:
    ZefaniaXmlReader(const QString &fileName, QSharedPointer<Versification> v11n);
    ZefaniaXmlReader(const QString &fileName);
    ~ZefaniaXmlReader();
    MetaInfo readMetaInfo();
    static bool cmp(const QStringRef &r, const QString &s);
    BookBlock readBookBlock(const int bookID);
    ChapterBlock readChapterBlock(const int bookID, const int chapterID);
    void buildIndex(const QString &indexPath);

private:
    BlockIDGen m_idGen;
    enum TagName {
        Biblebook,
        Br,
        Caption,
        Chapter,
        Gram,
        Information,
        Media,
        Note,
        Prolog,
        Remark,
        Style,
        Sup,
        Vers,
        XmlBible,
        XRef
        
    };
    bool cmp(const QStringRef &r, const TagName n);
    MetaInfo readMetaInfo(MetaInfo ret);

    BookBlock* rawReadBook(quint32 parent);
    ChapterBlock* rawReadChapter(quint32 parent);
    VerseBlock* rawReadVerse(quint32 parent);
    PrologBlock* rawReadProlog(quint32 parent);
    CaptionBlock* rawReadCaption(quint32 parent);
    RemarksBlock* rawReadRemarks(quint32 parent);
    XRefFragment* rawReadXRef(quint32 parent);
    MediaBlock* rawReadMedia(quint32 parent);
    BrFragment* rawReadBr(quint32 parent);
    StyleBlock* rawReadStyle(quint32 parent);
    GramBlock* rawReadGram(quint32 parent);
    SupBlock* rawReadSup(quint32 parent);
    DivBlock* rawReadDiv(quint32 parent);
    NoteBlock* rawReadNote(quint32 parent);
    TextFragment* rawReadText(quint32 parent);
    TextFragment* rawReadChildText(quint32 parent);
    QMap<int, QString> m_strongsPrefix;
    void genStrongsPrefix(const int bookID);

    QString m_fileName;
    QSharedPointer<Versification> m_versification;
    QXmlStreamReader *m_xml;
    QFile *m_file;

    void create();
    void destroy();

    QString m_strongAdd;
};

#endif // ZEFANIABIBLE_H
