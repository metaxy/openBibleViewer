/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
    DEBUG_FUNC_NAME
    m_moduleID = id;
    m_modulePath = path;
    m_v11n = QSharedPointer<Versification>(new Versification_KJV());

#ifdef BUILD_WITH_SWORD
    m_library = new SWMgr(new MarkupFilterMgr(FMT_PLAIN));

    m_target = m_library->getModule(path.toStdString().c_str());
    if(!m_target) {
        myWarning() << "could not load " << path;
        return 1;
    }
    myDebug() << "return zero";
    return 0;
#endif
    return 1;

}
int SwordBible::readBook(const int id)
{
    //m_currentBookID = id;
    return 0;
}

MetaInfo SwordBible::readInfo(const QString &fileName)
{
    return MetaInfo();
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
    return m_settings->homePath + "index/" + m_settings->hash(m_modulePath);
}
QString SwordBible::uid() const
{
    return m_modulePath;
}
TextRange SwordBible::rawTextRange(int bookID, int chapterID, int startVerse, int endVerse)
{
    DEBUG_FUNC_NAME;
    TextRange ret;
    std::pair<int,int> minMax = minMaxVerse(bookID, chapterID);
    if(startVerse == -1) {
        startVerse = minMax.first;
    }
    if(endVerse == -1) {
        endVerse = minMax.second;
    }
#ifdef BUILD_WITH_SWORD
    VerseKey mykey;
    QString b = m_v11n->bookName(bookID, true) + " " + QString::number(chapterID + 1) + ":" + QString::number(startVerse + 1);
    mykey = b.toStdString().c_str();

    int v = startVerse;
    for(; v < endVerse; mykey++) {
        m_target->setKey(mykey);
        Verse verse(v, QString::fromLocal8Bit(m_target->RenderText()));
        ret.addVerse(verse);
        v++;
    }
#endif
    ret.setBookID(bookID);
    ret.setChapterID(chapterID);
    ret.setModuleID(m_moduleID);
    return ret;
}

std::pair<int, int> SwordBible::minMaxVerse(int bookID, int chapterID)
{
    std::pair<int, int> ret;
    ret.first = 0;
    ret.second = m_v11n->maxVerse().value(bookID).at(chapterID);
    return ret;
}
QSharedPointer<Versification> SwordBible::versification() const
{
    return m_v11n;
}
void SwordBible::clearData()
{

}
