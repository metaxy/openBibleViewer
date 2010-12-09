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
        LoadFirstVerse
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
    int bibleID();
    /**
     * Returns how to load the bible
     */
    BibleLoadParams bible();

    void setStartBook(const int &bookID);
    void setStartBook(const BookLoadParams &param);
    void setEndBook(const int &bookID);
    void setEndBook(const BookLoadParams &param);

    int startBookID();
    BookLoadParams startBook();
    int endBookID();
    BookLoadParams endBook();

    void setStartChapter(const int &chapterID);
    void setStartChapter(const ChapterLoadParams &param);
    void setEndChapter(const int &chapterID);
    void setEndChapter(const ChapterLoadParams &param);

    int startChapterID();
    ChapterLoadParams startChapter();
    int endChapterID();
    ChapterLoadParams endChapter();

    void setStartVerse(const int &verseID);
    void setStartVerse(const VerseLoadParams &param);
    void setEndVerse(const int &verseID);
    void setEndVerse(const VerseLoadParams &param);

    int startVerseID();
    BookLoadParams startVerse();
    int endVerseID();
    BookLoadParams endVerse();


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
