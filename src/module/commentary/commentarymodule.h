#ifndef COMMENTARYMODULE_H
#define COMMENTARYMODULE_H
#include "src/module/response/response.h"
#include "src/core/verse/range.h"
#include "src/core/verse/versification.h"
#include "src/core/settings/settings.h"
#include "src/core/search/searchquery.h"
#include "src/core/search/searchresult.h"
#include <QtCore/QSharedPointer>
class CommentaryModule
{
public:
    CommentaryModule();
    void setSettings(Settings *settings);
    QSharedPointer<Versification> versification() const;
    virtual Response * readVerseRange(const int bookID,const int chapterID, const int startVerseID, const int endVerseID) = 0;
    virtual Response * readChapter(const int bookID, const int chapterID) = 0;
    virtual Response * readBook(const int bookID) = 0;

    virtual int loadModuleData(const int moduleID, const QString &fileName) = 0;

    virtual void search(SearchQuery query, SearchResult *result) = 0;

    virtual int currentBook() = 0;
    virtual int currentChapter() = 0;
    virtual std::pair<int, int> minMaxVerse(const int bookID, const int chapterID) = 0;
protected:
    QSharedPointer<Versification> m_versification;
    Settings *m_settings;
};

#endif // COMMENTARYMODULE_H
