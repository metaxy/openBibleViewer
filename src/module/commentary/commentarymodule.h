#ifndef COMMENTARYMODULE_H
#define COMMENTARYMODULE_H
#include "src/module/response/response.h"
#include "src/core/verse/range.h"
class CommentaryModule
{
public:
    CommentaryModule();

    virtual Response * readVerseRange(const int bookID,const int chapterID, const int startVerseID, const int endVerseID) = 0;
    virtual Response * readChapter(const int bookID, const int chapterID) = 0;
    virtual Response * readBook(const int bookID) = 0;

    virtual int loadModuleData(const int moduleID, const QString &fileName) = 0;

    virtual void search(SearchQuery query, SearchResult *result) = 0;

    virtual int currentBook();
    virtual int currentChapter();
    virtual std::pair<int, int> minMaxVerse(const int bookID, const int chapterID) = 0;

};

#endif // COMMENTARYMODULE_H
