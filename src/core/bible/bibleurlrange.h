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
#ifndef BIBLEURLRANGE_H
#define BIBLEURLRANGE_H
#include <QtCore/QString>
class BibleUrlRange
{
public:
    BibleUrlRange();
    enum BibleLoadParams {
        LoadBibleByID,
        LoadCurrentBible,
        LoadBibleNotSet,
        LoadBibleByUID
    };
    enum BookLoadParams {
        LoadBookByID,
        LoadCurrentBook,
        LoadFirstBook,
        LoadLastBook,
        LoadBookNotSet
    };
    enum ChapterLoadParams {
        LoadChapterByID,
        LoadCurrentChapter,
        LoadFirstChapter,
        LoadLastChapter,
        LoadChapterNotSet
    };
    enum VerseLoadParams {
        LoadVerseByID,
        LoadCurrentVerse,
        LoadFirstVerse,
        LoadLastVerse,
        LoadVerseNotSet
    };

    /**
     * Sets the bibleID
     */
    void setBible(const int &bibleID);
    /**
     * Set how to load the bible
     */
    void setBible(const BibleLoadParams &param);
    void setBible(const QString &uid);

    /**
     * If set bible() == LoadBibleByID then it is the id of bible
     * Else is not definied
     */
    int bibleID() const;
    /**
     * Returns how to load the bible
     */
    BibleLoadParams bible() const;
    QString bibleUID() const;

    void setBook(const int &bookID);
    void setBook(const BookLoadParams &param);

    int bookID() const;
    BookLoadParams book() const;

    void setChapter(const int &chapterID);
    void setChapter(const ChapterLoadParams &param);

    void setStartChapter(const int &chapterID);
    void setStartChapter(const ChapterLoadParams &param);
    void setEndChapter(const int &chapterID);
    void setEndChapter(const ChapterLoadParams &param);

    int startChapterID() const;
    ChapterLoadParams startChapter() const;
    int endChapterID() const;
    ChapterLoadParams endChapter() const;

    void setStartVerse(const int &verseID);
    void setStartVerse(const VerseLoadParams &param);
    void setEndVerse(const int &verseID);
    void setEndVerse(const VerseLoadParams &param);

    int startVerseID() const;
    VerseLoadParams startVerse() const;
    int endVerseID() const;
    VerseLoadParams endVerse() const;

    void setActiveVerse(const int &verseID);
    int activeVerseID() const;
    VerseLoadParams activeVerse() const;

    void setWholeChapter();

    bool containsChapter(const int &chapterID) const;
    bool containsVerse(const int &verseID) const;

private:
    int m_bibleID;
    BibleLoadParams m_bibleParam;
    QString m_bibleUID;

    int m_bookID;
    BookLoadParams m_bookParam;


    int m_startChapterID;
    int m_endChapterID;
    ChapterLoadParams m_startChapterParam;
    ChapterLoadParams m_endChapterParam;

    int m_startVerseID;
    int m_endVerseID;
    VerseLoadParams m_startVerseParam;
    VerseLoadParams m_endVerseParam;

    int m_activeVerseID;
    VerseLoadParams m_activeVerseParam;

};

#endif // BIBLEURLRANGE_H
