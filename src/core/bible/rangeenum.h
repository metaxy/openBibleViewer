#ifndef RANGEENUM_H
#define RANGEENUM_H

class RangeEnum
{
public:
    RangeEnum();


    enum BookRange {//todo: add LoadLastBook
        BookByID,
        FirstBook,
        LastBook,
        CurrentBook
    };
    enum ChapterRange {
        ChapterByID,
        FirstChapter,
        LastChapter,
        CurrentChapter
    };
    enum VerseRange {
        VerseByID,
        FirstVerse,
        LastVerse,
        CurrentVerse
    };
};

#endif // RANGEENUM_H
