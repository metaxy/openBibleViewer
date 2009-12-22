#include "urlconverter.h"
#include "dbghelper.h"
UrlConverter::UrlConverter(const int &from, const int &to, const QString &url)
{
    m_from = from;
    m_to = to;
    m_url = url;
}
QString UrlConverter::convert()
{
   // DEBUG_FUNC_NAME
    QString ret;
    if (m_to == InterfaceUrl) {
        ret = "bible://" + m_bibleID + "/" + QString::number(m_bookID) + "," + QString::number(m_chapterID) + "," + QString::number(m_verseID);
    } else if (m_to == PersistentUrl) {
        ret = m_biblesIniPath.at(m_bibleID.toInt()) + ";" +QString::number(m_bookID) + ";" + QString::number(m_chapterID) + ";" + QString::number(m_verseID);
        if(!m_bookName.isEmpty()) {
            ret += ";"+m_bookName;//check for invalid charatcers
        }
    } else if (m_to == BibleQuoteUrl) {
    }
    myDebug() << ret << "bibleID = " << m_bibleID << " iniPath = " << m_biblesIniPath;
    return ret;
}
int UrlConverter::pharse()
{
    //DEBUG_FUNC_NAME
    QString bible = "bible://";
    if (m_from == InterfaceUrl) {
        QString url = m_url;
        if (url.startsWith(bible)) {
            url = url.remove(0, bible.size());
            QStringList a = url.split("/");
            if (a.size() == 2) {
                QStringList c = a.at(1).split(",");
                m_bibleID = a.at(0);
                if (c.size() >= 3) {
                    m_bookID = c.at(0).toInt();
                    m_chapterID = c.at(1).toInt();
                    m_verseID = c.at(2).toInt();
                }
                else {
                    myDebug() << "invalid url";
                    return 1;
                }
            }
        } else {
            myDebug() << "invalid url, it must start with bible://";
            return 1;
        }
    } else if (m_from == PersistentUrl) {
        QStringList list = m_url.split(";");
        if (list.size() < 4) {
            return 1;
        }
        QString path = list.at(0);
        QString sbookID = list.at(1);
        QString schapterID = list.at(2);
        QString sverseID = list.at(3);
        if (list.size() == 5) {
            m_bookName = list.at(4);
        }
        else {
            m_bookName = sbookID;//todo: find something better
        }
        m_path = path;
        m_bookID = sbookID.toInt();
        m_chapterID = schapterID.toInt();
        m_verseID = sverseID.toInt();
        //get bibleID
        for (int i = 0; i < m_biblesIniPath.size(); i++) {
            if (m_biblesIniPath.at(i) == path) {
                m_bibleID = QString::number(i);
                break;
            }
        }
    } else if (m_from == BibleQuoteUrl) {
    }
    return 0;
}
void UrlConverter::setFrom(const int &urlType)
{
    m_from = urlType;
}
void UrlConverter::setTo(const int &urlType)
{
    m_to = urlType;
}
void UrlConverter::setUrl(const QString &url)
{
    m_url = url;
}
