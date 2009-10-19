/***************************************************************************
openBibleViewer - Free Bibel Viewer
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
#include "../core/config.h"
#include "../core/stelle.h"
#include "../core/chapter.h"
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QMap>
class biblequote
{
private:
    struct settings_s bqset;
    QString formatfromini(QString input);


public:
    biblequote();
    int setSettings(struct settings_s settings);
    void readBook(int id, QString path);
    void loadBibleData(int bibleID, QString path);
    QString readInfo(QFile &file);
    struct stelle search(QString searchstring, bool regexp, bool whole, bool casesen);
    int currentBookID,/*book_ccount,*/currentBibleID;
    bool chapterZero, bible, oldTestament, newTestament, apocrypha, strongNumbers, greek;
    QString currentBiblePath, lastout, chaptersign, versesign, bibleName, removeHtml, lastSearch;
    QString bibles, biblesPath, biblesIniPath;
    QStringList chapterText, bookPath, bookFullName, bookShortName;
    QMap <int, int> bookCount;
    QList<chapter> chapterData;
    struct stelle st;

};

#endif // BIBLEQUOTE_H
