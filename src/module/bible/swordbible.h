#ifndef SWORDBIBLE_H
#define SWORDBIBLE_H
#include "src/core/dbghelper.h"
#include "src/module/bible/biblemodule.h"

#include "CLucene.h"
#include "CLucene/_clucene-config.h"

class SwordBible : public BibleModule
{
public:
    SwordBible();
    void setSettings(Settings *settings);
    void loadBibleData(const int &id, const QString &uid);
    int readBook(const int &id);

    QString readInfo(QFile &file);
    QString readInfo(const QString &fileName);
    void search(const SearchQuery &query, SearchResult *res) const;
    bool hasIndex() const;
    void buildIndex();

    int moduleID() const;
    QString modulePath() const;
    QString moduleName(bool preferShortName = false) const;
    QString uid() const;
private:
    int m_moduleID;
    QString m_modulePath;
    QString indexPath() const;
};

#endif // SWORDBIBLE_H
