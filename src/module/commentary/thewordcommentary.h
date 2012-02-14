#ifndef THEWORDCOMMENTARY_H
#define THEWORDCOMMENTARY_H
#include "src/module/commentary/commentarymodule.h"
#include <QtSql/QSqlDatabase>
#include "src/module/metainfo.h"
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

    MetaInfo readInfo(const QString &name);


private:
    QSqlDatabase m_db;

    bool m_loaded;
};

#endif // THEWORDCOMMENTARY_H
