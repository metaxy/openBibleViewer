#include "biblemodule.h"
#include <QtCore/QString>
#include <QtCore/QFile>
#include "src/core/search/searchquery.h"
#include "src/core/search/searchresult.h"
#include "src/core/dbghelper.h"
BibleModule::BibleModule()
{
}
void BibleModule::setSettings(Settings *settings)//1
{
    DEBUG_FUNC_NAME
    m_settings = settings;
}
int BibleModule::readBook(const int &id)//4
{
    return 0;
}

void BibleModule::loadBibleData(const int &bibleID, const QString &path)//2
{

}

QString BibleModule::readInfo(QFile &file)//3
{
    return "";
}

void BibleModule::search(const SearchQuery &query, SearchResult *res)
{
}

bool BibleModule::hasIndex() const
{
    return false;
}
void BibleModule::buildIndex()
{

}
int BibleModule::bibleID() const
{
    return 0;
}
QString BibleModule::biblePath() const
{
    return "";
}
QString BibleModule::bibleName(bool preferShortName) const
{
    return "";
}
QMap<int, int> BibleModule::bookCount() const
{
    return QMap<int, int> ();
}
BookNames BibleModule::getBookNames()
{
    BookNames m;
    return m;
}

Book BibleModule::book() const
{
    Book b;
    return b;
}
