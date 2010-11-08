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
#ifndef BIBLEURL_H
#define BIBLEURL_H
#include <QtCore/QString>
#include <QtCore/QHash>
#include "src/core/url.h"
class BibleUrl : public Url
{
public:
    BibleUrl();
    enum BibleLoadParams {
        LoadBibleByID,
        LoadCurrentBible,
        ReloadActive
    };
    enum BookLoadParams {//todo: add LoadLastBook
        LoadBookByID,
        LoadCurrentBook,
        LoadFirstBook
    };
    enum ChapterLoadParams {
        LoadChapterByID,
        LoadCurrentChapter,
        LoadFirstChapter
    };
    enum VerseLoadParams {
        LoadVerseByID,
        LoadCurrentVerse,
        LoadFirstVerse
    };

    /**
     * Sets the bibleID
     */
    void setBibleID(const int &bibleID);
    /**
     * Set how to load the bible
     */
    void setBible(const BibleLoadParams &param);

    /**
     * If set bible() == LoadBibleByID then it is the id of bible
     * Else is not definied
     */
    int bibleID();
    /**
     * Returns how to load the bible
     */
    BibleLoadParams bible();

    void setBookID(const int &bookID);
    void setBook(const BookLoadParams &param);
    int bookID();
    BookLoadParams book();

    void setChapterID(const int &chapterID);
    void setChapter(const ChapterLoadParams &param);
    int chapterID();
    ChapterLoadParams chapter();

    void setVerseID(const int &verseID);
    void setVerse(const VerseLoadParams &param);
    int verseID();
    VerseLoadParams verse();
    void setParam(const QString &name, const QString &value);
    bool hasParam(const QString &name);
    QString getParam(const QString &name);


    QString toString();
    bool fromString(QString url);
private:
    int m_bibleID;
    int m_bookID;
    int m_chapterID;
    int m_verseID;

    BibleLoadParams m_bibleParam;
    BookLoadParams m_bookParam;
    ChapterLoadParams m_chapterParam;
    VerseLoadParams m_verseParam;
    QHash<QString, QString> m_params;


};

#endif // BIBLEURL_H
