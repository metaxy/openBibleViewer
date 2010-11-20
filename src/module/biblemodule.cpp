#include "biblemodule.h"
#include <QtCore/QString>
#include <QtCore/QFile>
#include "src/core/search/searchquery.h"
#include "src/core/search/searchresult.h"
#include "src/core/dbghelper.h"
BibleModule::BibleModule()
{
}
void BibleModule::setSettings(Settings *settings)
{
    DEBUG_FUNC_NAME
    m_settings = settings;
}
int BibleModule::readBook(const int &id)
{
    myWarning() << "calling BibleModule";
    return 0;
}

void BibleModule::loadBibleData(const int &bibleID, const QString &path)
{
    myWarning() << "calling BibleModule";
}

QString BibleModule::readInfo(QFile &file)//3
{
    myWarning() << "calling BibleModule";
    return "";
}

void BibleModule::search(const SearchQuery &query, SearchResult *res) const
{
    myWarning() << "calling BibleModule";
}

bool BibleModule::hasIndex() const
{
    myWarning() << "calling BibleModule";
    return false;
}
void BibleModule::buildIndex()
{
    myWarning() << "calling BibleModule";
}
int BibleModule::bibleID() const
{
    myWarning() << "calling BibleModule";
    return 0;
}
QString BibleModule::biblePath() const
{
    myWarning() << "calling BibleModule";
    return "";
}
QString BibleModule::bibleName(bool preferShortName) const
{
    myWarning() << "calling BibleModule";
    return "";
}
QMap<int, int> BibleModule::bookCount() const
{
    myWarning() << "calling BibleModule";
    return QMap<int, int> ();
}
BookNames BibleModule::getBookNames()
{
    myWarning() << "calling BibleModule";
    BookNames m;
    return m;
}

Book BibleModule::book() const
{
    myWarning() << "calling BibleModule";
    Book b;
    return b;
}
