#ifndef WEBCOMMENTARY_H
#define WEBCOMMENTARY_H
#include "src/module/versemodule.h"
class Webcommentary : public VerseModule
{
public:
    Webcommentary();
    Response* readRanges(const Ranges &ranges, bool ignoreModuleID = false);
private:
    int currentBook();
    int currentChapter();
    std::pair<int, int> minMaxVerse(const int bookID, const int chapterID);
};

#endif // WEBCOMMENTARY_H
