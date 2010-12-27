#ifndef THEWORDBIBLE_H
#define THEWORDBIBLE_H
#include "src/module/biblemodule.h"
#include <QtCore/QFileInfo>
#include <utility>
class TheWordBible: public BibleModule
{
public:
    TheWordBible();
    //~TheWordBible();
    void setSettings(Settings *settings);
    void loadBibleData(const int &id, const QString &path);
    int readBook(const int &id);

    QString readInfo(QFile &file);
    void search(const SearchQuery &query, SearchResult *res) const;
    bool hasIndex() const;
    void buildIndex();

    int moduleID() const;
    QString modulePath() const;
    QString moduleName(bool preferShortName = false) const;
    QMap<int, int> bookCount();
    BookNames getBookNames();
    Book book() const;


private:
    Book m_book;
    QString indexPath() const;
    int m_moduleID;
    QString m_modulePath;

    bool hasNT() const;
    bool hasOT() const;
    bool hasONT() const;

};

#endif // THEWORDBIBLE_H
