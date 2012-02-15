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
#ifndef BIBLEURLRANGE_H
#define BIBLEURLRANGE_H
#include <QtCore/QString>
#include "src/core/verse/range.h"

class VerseUrlRange
{
public:
    VerseUrlRange();
    enum ModuleLoadParams {
        LoadModuleByID,
        LoadCurrentModule,
        LoadModuleNotSet,
        LoadModuleByUID
    };
    enum BookLoadParams {
        LoadBookByID,
        LoadCurrentBook,
        LoadFirstBook,
        LoadLastBook,
        LoadBookNotSet,
        LoadNoBook
    };
    enum ChapterLoadParams {
        LoadChapterByID,
        LoadCurrentChapter,
        LoadFirstChapter,
        LoadLastChapter,
        LoadChapterNotSet,
        LoadNoChapter
    };
    enum VerseLoadParams {
        LoadVerseByID,
        LoadCurrentVerse,
        LoadFirstVerse,
        LoadLastVerse,
        LoadVerseNotSet,
        LoadNoVerse
    };

    /**
     * Sets the bibleID
     */
    void setModule(const int moduleID);
    /**
     * Set how to load the bible
     */
    void setModule(const ModuleLoadParams &param);
    void setModule(const QString &uid);

    /**
     * If set module() == LoadModuleByID then it is the ID of module
     * Else -1
     */
    int moduleID() const;
    /**
     * Returns how to load the bible
     */
    ModuleLoadParams module() const;
    QString moduleUID() const;

    void setBook(const int bookID);
    void setBook(const BookLoadParams &param);

    int bookID() const;
    BookLoadParams book() const;

    void setChapter(const int chapterID);
    void setChapter(const ChapterLoadParams &param);

    int chapterID() const;
    ChapterLoadParams chapter() const;

    void setStartVerse(const int verseID);
    void setStartVerse(const VerseLoadParams &param);
    void setEndVerse(const int verseID);
    void setEndVerse(const VerseLoadParams &param);

    int startVerseID() const;
    VerseLoadParams startVerse() const;
    int endVerseID() const;
    VerseLoadParams endVerse() const;

    void setActiveVerse(const int verseID);
    int activeVerseID() const;
    VerseLoadParams activeVerse() const;

    void setWholeChapter();

    bool containsChapter(const int chapterID) const;
    bool containsVerse(const int verseID) const;

    /**
      * If the urlRange is only some verse, than the frontend will show the whole chapter, if openToTransformation is set
      */
    bool openToTransformation() const;
    void setOpenToTransformation(bool open);

    Range toRange() const;

    void removeNo();

private:
    int m_moduleID;

    ModuleLoadParams m_moduleParam;
    QString m_moduleUID;

    int m_bookID;
    BookLoadParams m_bookParam;

    int m_chapterID;
    ChapterLoadParams m_chapterParam;

    int m_startVerseID;
    int m_endVerseID;
    VerseLoadParams m_startVerseParam;
    VerseLoadParams m_endVerseParam;

    int m_activeVerseID;
    VerseLoadParams m_activeVerseParam;

    bool m_openToTranformation;

};

#endif // BIBLEURLRANGE_H
