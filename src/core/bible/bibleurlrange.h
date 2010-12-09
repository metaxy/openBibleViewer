#ifndef BIBLEURLRANGE_H
#define BIBLEURLRANGE_H

class BibleUrlRange
{
public:
    BibleUrlRange();
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
        LoadFirstVerse,
        LoadLastVerse
    };

    /**
     * Sets the bibleID
     */
    void setBible(const int &bibleID);
    /**
     * Set how to load the bible
     */
    void setBible(const BibleLoadParams &param);

    /**
     * If set bible() == LoadBibleByID then it is the id of bible
     * Else is not definied
     */
    int bibleID() const;
    /**
     * Returns how to load the bible
     */
    BibleLoadParams bible() const;

    void setBook(const int &bookID);
    void setBook(const BookLoadParams &param);
    void setStartBook(const int &bookID);
    void setStartBook(const BookLoadParams &param);
    void setEndBook(const int &bookID);
    void setEndBook(const BookLoadParams &param);

    int startBookID() const;
    BookLoadParams startBook() const;
    int endBookID() const;
    BookLoadParams endBook() const;

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
    void setWholeChapter();

private:
    int m_bibleID;
    BibleLoadParams m_bibleParam;

    int m_startBookID;
    int m_endBookID;
    BookLoadParams m_startBookParam;
    BookLoadParams m_endBookParam;

    int m_startChapterID;
    int m_endChapterID;
    ChapterLoadParams m_startChapterParam;
    ChapterLoadParams m_endChapterParam;

    int m_startVerseID;
    int m_endVerseID;
    VerseLoadParams m_startVerseParam;
    VerseLoadParams m_endVerseParam;

};

#endif // BIBLEURLRANGE_H
