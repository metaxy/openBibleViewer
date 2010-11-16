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
#ifndef BIBLEQUOTE_H
#define BIBLEQUOTE_H
#include "src/core/bible/book.h"
#include "src/core/bible/chapter.h"
#include "src/module/biblemodule.h"
#include "src/core/search/searchquery.h"
#include "src/core/search/searchresult.h"
#include "src/core/settings/settings.h"
#include "src/core/bible/booknames.h"
#include <QtCore/QFile>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTextCodec>


/**
 * BibleQuote represents a biblequote module
 * see http://jesuschrist.ru/software/
 */
class BibleQuote : public BibleModule
{


public:
    BibleQuote();
    ~BibleQuote();
    void setSettings(Settings *settings);//1
    /*virtual*/ int readBook(const int &id);//3
   /* virtual*/ void loadBibleData(const int &bibleID, const QString &path);//2
   /* virtual*/ QString readInfo(QFile &file);//4
    /*virtual*/ void search(const SearchQuery &query, SearchResult *res);//5
    /*virtual*/ bool hasIndex() const;//6
    /*virtual*/ void buildIndex();//7

    /*virtual*/ int bibleID() const;//8
   /* virtual*/ QString biblePath() const;//9
    /*virtual*/ QString bibleName(bool preferShortName = false) const;//10
    /*virtual*/ QMap<int, int> bookCount();//11
    /*virtual*/ BookNames getBookNames();//12
    /*virtual*/ Book book() const;//13

    bool m_chapterZero;

    QString m_biblePath;
    QString m_bibleName;
    QString m_bibleShortName;

    QStringList m_bookPath;
    QStringList m_bookFullName;
    QList<QStringList> m_bookShortName;
    QMap<int, int> m_bookCount;
    Book m_book;
private:
    inline QString formatFromIni(QString input);
    QString indexPath() const;
    int m_bookID;
    int m_bibleID;
    QString m_verseSign;
    QString m_chapterSign;
    QString m_removeHtml;
    QTextCodec *m_codec;
};

#endif // BIBLEQUOTE_H
