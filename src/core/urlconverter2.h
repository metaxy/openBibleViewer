#ifndef URLCONVERTER2_H
#define URLCONVERTER2_H
#include "urlconverter.h"
class UrlConverter2
{
public:
    UrlConverter2(UrlConverter::UrlType from, UrlConverter::UrlType to, BibleUrl url);
    UrlConverter2(UrlConverter::UrlType from, UrlConverter::UrlType to, QString url);
    void setFromTo(UrlConverter::UrlType from, UrlConverter::UrlType to);
    void setSM(Settings *settings, ModuleMap *map);
    void setBookNames(QHash<int, QString> bookNames);
    void setUrl(BibleUrl url);
    void convert();

    bool contains(const int &moduleID, const int &bookID, const int &chapterID, const int &verseID) const;
    bool contains(const int &moduleID, const int &bookID, const int &chapterID) const;

    QString bookName() const;
    int moduleID() const;
    int bookID() const;
    int chapterID() const;
    int verseID() const;
    BibleUrl url() const;
private:
    UrlConverter m_urlConverter;
    BibleUrl m_url;
    BibleUrl m_newUrl;

};

#endif // URLCONVERTER2_H
