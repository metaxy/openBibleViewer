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
#include "src/core/settings.h"
#include "src/core/chapter.h"
#include "src/core/searchquery.h"
#include "src/core/searchresult.h"
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QMap>
/*!
 BibleQuote represents a biblequote module

 @author Paul Walger <metaxy@walger.name>
*/
class BibleQuote
{


public:
    BibleQuote();
    int setSettings(Settings *settings);
    int readBook(int id, QString path);
    void loadBibleData(int bibleID, QString path);
    QString readInfo(QFile &file);
    SearchResult search(SearchQuery query);
    bool hasIndex();
    void buildIndex();

    bool m_chapterZero;

    QString m_biblePath;
    QString m_bibleName;
    QString m_bibleShortName;

    QStringList m_bookPath;
    QStringList m_bookFullName;
    QStringList m_bookShortName;
    QMap <int, int> m_bookCount;
    QList<Chapter> m_chapterData;
private:
    Settings *m_settings;
    QString formatFromIni(QString input);
    int m_bookID;
    int m_bibleID;
    QString m_verseSign;
    QString m_chapterSign;
    QString m_removeHtml;
};

#endif // BIBLEQUOTE_H
