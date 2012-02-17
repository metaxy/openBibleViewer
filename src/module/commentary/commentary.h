#ifndef COMMENTARY_H
#define COMMENTARY_H
#include "src/module/versemodule.h"
#include "src/module/commentary/commentarymodule.h"
class Commentary : public VerseModule
{
public:
    Commentary();
    Response* readRanges(const Ranges &ranges, bool ignoreModuleID = false);
    void search(SearchQuery query, SearchResult *result);
    void clearData();
    bool loaded();
    QSharedPointer<CommentaryModule> m_commentaryModule;
private:
    int currentBook();
    int currentChapter();
    std::pair<int, int> minMaxVerse(const int bookID, const int chapterID);



    int loadModuleData(const int moduleID);

    bool m_loaded;
    int m_loadedModuleID;

};

#endif // COMMENTARY_H
