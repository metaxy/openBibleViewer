#include "webcommentary.h"

Webcommentary::Webcommentary()
{
}
Response* Webcommentary::readRanges(const Ranges &ranges, bool ignoreModuleID = false)
{
    return NULL;
}
int Webcommentary::currentBook()
{
    return 0;
}
int Webcommentary::currentChapter()
{
    return 0;
}
std::pair<int, int> Webcommentary::minMaxVerse(const int bookID, const int chapterID)
{
    std::pair<int, int> ret;
    ret.frist = 0;
    ret.second = 0;
    return ret;
}
