#ifndef THEWORDCOMMENTARY_H
#define THEWORDCOMMENTARY_H
#include "src/module/commentary/commentarymodule.h"
#include <QtSql/QSqlDatabase>
#include "src/module/metainfo.h"
#include "src/core/rtftools.h"
#include <QTextDocument>
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

    CommentaryModule::LinkPolicy linkPolicy() const;

    void readRtf(const QVariant &value, QTextDocument *rtfDocument, const int bi, const int ci, const int fvi, const int tvi);
    void readRvf(const QVariant &value, QString *ret);


private:
    QSqlDatabase m_db;

    int m_book;
    int m_chapter;

    bool m_compressed;
    RtfTools::TheWordContentType m_contentType;

};

#endif // THEWORDCOMMENTARY_H
