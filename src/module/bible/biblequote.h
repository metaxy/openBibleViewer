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
#ifndef BIBLEQUOTE_H
#define BIBLEQUOTE_H
#include <QtCore/QFile>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTextCodec>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtGui/QProgressDialog>
#include <QtGui/QMessageBox>

#include "src/core/verse/chapter.h"
#include "src/module/bible/biblemodule.h"
#include "src/core/dbghelper.h"
#include "src/core/verse/versification/versification_biblequote.h"

/**
 * BibleQuote represents a biblequote module
 * see http://jesuschrist.ru/software/
 */
class BibleQuote : public BibleModule
{


public:
    BibleQuote();
    ~BibleQuote();
    int readBook(const int id);
    int loadBibleData(const int moduleID, const QString &path);
    MetaInfo readInfo(QFile &file);
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

    QStringList booksPath() const;

    void clearData();

private:
    inline QString formatFromIni(const QString &input);
    QString indexPath() const;
    int m_moduleID;
    QString m_verseSign;
    QString m_chapterSign;
    QString m_removeHtml;
    QTextCodec *m_codec;
    QString m_uid;

    VerseBook m_book;
    QString m_modulePath;
    QString m_moduleName;
    QString m_moduleShortName;
    bool m_chapterZero;


    QStringList m_bookPath;

    void loadCodec();

    QString encodingName();
};

#endif // BIBLEQUOTE_H
