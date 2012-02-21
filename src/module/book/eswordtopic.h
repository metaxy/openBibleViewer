#ifndef ESWORDTOPIC_H
#define ESWORDTOPIC_H
#include "treebookmodule.h"
#include <QtSql/QSqlDatabase>
#include <QMap>
class ESwordTopic : public TreeBookModule
{
public:
    ESwordTopic();
    ~ESwordTopic();
    MetaInfo readInfo(const QString &name);

    void search(const SearchQuery &query, SearchResult *res) const;
    bool hasIndex() const;
    void buildIndex();

    int moduleID() const;
    QString modulePath() const;
    QString moduleName(bool preferShortName = false) const;

    int loadModuleData(const int moduleID, const QString &path);


    Response* readChapter(const int chapterID);
    BookTree * bookTree();
private:
    QSqlDatabase m_db;

    QMap<int, QString> m_chapterData;
    BookTree *m_bookTree;
};

#endif // ESWORDTOPIC_H
