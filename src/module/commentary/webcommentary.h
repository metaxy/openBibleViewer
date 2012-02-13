#ifndef WEBCOMMENTARY_H
#define WEBCOMMENTARY_H
#include "src/module/versemodule.h"
#include <QtScript/QScriptEngine>
#include <QtCore/QUrl>
struct WebCommentaryBooksData
{
    bool bookID;
    QString key;
};

class WebCommentary : public VerseModule
{
public:
    WebCommentary();
    Response* readRanges(const Ranges &ranges, bool ignoreModuleID = false);
    QString pharseUrl(const QUrl &url);
    MetaInfo readInfo(const QString &name);
    void search(SearchQuery query, SearchResult *result);
    void clearData();
    bool loaded();
private:
    void loadModuleData(const int moduleID, const QString &fileName = "");
    int currentBook();
    int currentChapter();
    std::pair<int, int> minMaxVerse(const int bookID, const int chapterID);

    bool m_loaded;


    QString m_pharseBookScript;
    QString m_pharseChapterScript;
    QString m_pharseVerseScript;
    QString m_pharseOutScript;

    QString m_name;
    QString m_shortName;
    QString m_desc;
    QString m_type;
    QString m_v11nName;

    QString m_url;


    int m_loadedModuleID;

    QScriptEngine myEngine;
    QString m_modulePath;
    QHash<int, WebCommentaryBooksData> m_books;
};

#endif // WEBCOMMENTARY_H
