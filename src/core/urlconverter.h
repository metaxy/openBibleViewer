#ifndef URLCONVERTER_H
#define URLCONVERTER_H
#include <QtCore/QString>
#include <QtCore/QStringList>
class UrlConverter
{
public:
    UrlConverter(const int &from, const int &to, const QString &url);
    enum urlType {
        None = -1,
        InterfaceUrl = 0,
        PersistentUrl = 1,
        BibleQuoteUrl = 2
    };
    void setFrom(const int &urlType);
    void setTo(const int &urlType);
    void setUrl(const QString &url);
    int pharse();

    QString convert();
    QString m_bibleID;
      QString m_path;
    int m_chapterID;
    int m_bookID;
    int m_verseID;
    QStringList m_biblesIniPath;
    QString m_bookName;
private:
    int m_from;
    int m_to;
    QString m_url;
};

#endif // URLCONVERTER_H
