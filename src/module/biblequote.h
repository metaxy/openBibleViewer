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
#ifndef BIBLEQUOTE_H
#define BIBLEQUOTE_H
#include "../core/settings.h"
#include "../core/stelle.h"
#include "../core/chapter.h"
#include "../core/searchquery.h"
#include "../core/searchresult.h"
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
private:
    Settings *m_settings;
    QString formatfromini(QString input);

public:
    BibleQuote();
    int setSettings(Settings *settings);
    void readBook(int id, QString path);
    void loadBibleData(int bibleID, QString path);
    QString readInfo(QFile &file);
    SearchResult search(SearchQuery query);

    int currentBookID, currentBibleID;
    bool chapterZero, bible, oldTestament, newTestament, apocrypha, strongNumbers, greek;
    QString currentBiblePath, lastout, chaptersign, versesign, bibleName, removeHtml;
    QString bibles;
    QStringList bookPath, bookFullName, bookShortName;
    QMap <int, int> bookCount;
    SearchQuery lastSearchQuery;
    QList<Chapter> chapterData;
};

#endif // BIBLEQUOTE_H
