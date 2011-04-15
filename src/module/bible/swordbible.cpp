/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
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
int SwordBible::loadBibleData(const int id, const QString &path)
{
    //DEBUG_FUNC_NAME
    m_moduleID = id;
    return 0;

}
int SwordBible::readBook(const int id)
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
    return false;
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
