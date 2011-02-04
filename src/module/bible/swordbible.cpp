#include "swordbible.h"

SwordBible::SwordBible()
{
}
void SwordBible::setSettings(Settings *set)
{
    m_settings = set;
}
/**
  * Loads the Data.
  */
void SwordBible::loadBibleData(const int &id, const QString &path)
{
    //DEBUG_FUNC_NAME
    m_moduleID = id;


}
int SwordBible::readBook(const int &id)
{
    //m_currentBookID = id;
    return 0;
}

QString SwordBible::readInfo(QFile &file)
{
    return "";
}
QString SwordBible::readInfo(const QString &fileName)
{
    return "";
}

void SwordBible::search(const SearchQuery &query, SearchResult *res) const
{

}
bool SwordBible::hasIndex() const
{

}
void SwordBible::buildIndex()
{

}

int SwordBible::moduleID() const
{
    return m_moduleID;
}

QString SwordBible::modulePath() const
{
    return m_modulePath;
}
QString SwordBible::moduleName(bool preferShortName) const
{
    return "";
    /*if(preferShortName) {
        if(!m_shortModuleName.isEmpty()) {
            return m_shortModuleName;
        } else {
            return m_moduleName;
        }
    } else {
        if(!m_moduleName.isEmpty()) {
            return m_moduleName;
        } else {
            return m_shortModuleName;
        }
    }*/
}

/**
  * Returns the path, where all indexed files are stored.
  */
QString SwordBible::indexPath() const
{
    return "";
    // return m_settings->homePath + "index/" + m_settings->hash(m_modulePath);
}
QString SwordBible::uid() const
{
    return "";
    //  return m_uID;
}
