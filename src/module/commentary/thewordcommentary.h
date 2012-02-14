#ifndef THEWORDCOMMENTARY_H
#define THEWORDCOMMENTARY_H
#include "src/module/commentary/commentarymodule.h"
class TheWordCommentary : public CommentaryModule
{
public:
    TheWordCommentary();

    Response * readVerseRange(const int bookID,const int chapterID, const int startVerseID, const int endVerseID);
    Response * readChapter(const int bookID, const int chapterID);
    Response * readBook(const int bookID);

    int loadModuleData(const int moduleID, const QString &fileName);

    void search(SearchQuery query, SearchResult *result);

    int currentBook();
    int currentChapter();
    std::pair<int, int> minMaxVerse(const int bookID, const int chapterID);
};

#endif // THEWORDCOMMENTARY_H
