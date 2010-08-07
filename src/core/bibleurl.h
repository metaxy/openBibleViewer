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
    enum BookLoadParams {
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

    void setBibleID(const int &bibleID);
    void setBible(const BibleLoadParams &param);

    int bibleID();
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
