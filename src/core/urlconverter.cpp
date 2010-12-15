/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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
#include "urlconverter.h"
#include "dbghelper.h"
#include "src/core/bible/bibleurl.h"
UrlConverter::UrlConverter(const UrlType &from, const UrlType &to, const QString &url)
{
    m_from = from;
    m_to = to;
    m_url = url;

    m_moduleID = -1;
    m_path = "";
    m_chapterID = -1;
    m_bookID = -1;
    m_verseID = -1;
    m_bookName = "";
}
void UrlConverter::setModuleMap(ModuleMap *moduleMap)
{
    m_moduleMap = moduleMap;
}
void UrlConverter::setSettings(Settings *settings)
{
    m_settings = settings;
}

QString UrlConverter::convert()
{
    QString ret;
    //todo:
    if(m_to == InterfaceUrl) {
        BibleUrl url;
        BibleUrlRange range;
        range.setBible(m_moduleID);
        range.setStartBook(m_bookID);
        range.setStartChapter(m_chapterID);
        range.setWholeChapter();
        range.setActiveVerse(m_verseID);
        url.addRange(range);
        ret = url.toString();
    } else if(m_to == PersistentUrl) {
        if(!m_moduleMap->m_map.contains(m_moduleID)) {
            //myDebug() << "moduleID = " << m_moduleID;
            return "";
        }
        ret = m_settings->savableUrl(m_moduleMap->m_map.value(m_moduleID)->m_path) + ";" + QString::number(m_bookID) + ";" + QString::number(m_chapterID) + ";" + QString::number(m_verseID);
        if(!m_bookName.isEmpty()) {
            ret += ";" + m_bookName;//check for invalid charatcers
        }
    } else if(m_to == BibleQuoteUrl) {
    }
    return ret;
}
int UrlConverter::pharse()
{
    //todo: this won't work
    QString bible = "bible://";
    if(m_from == InterfaceUrl) {
        //pharse with the help of BibleUrL
        BibleUrl url;
        url.fromString(m_url);
        BibleUrlRange r = url.ranges().first();
        m_bookID = r.startBookID();
        m_chapterID = r.startChapterID();
        m_verseID = r.activeVerseID();

    } else if(m_from == PersistentUrl) {
        QStringList list = m_url.split(";");
        if(list.size() < 4) {
            return 1;
        }
        QString path = m_settings->recoverUrl(list.at(0));
        QString sbookID = list.at(1);
        QString schapterID = list.at(2);
        QString sverseID = list.at(3);
        if(list.size() == 5) {
            m_bookName = list.at(4);
        } else {
            m_bookName = sbookID;//todo: find something better
        }
        m_path = path;
        m_bookID = sbookID.toInt();
        m_chapterID = schapterID.toInt();
        m_verseID = sverseID.toInt();
        //get bibleID
        QMapIterator<int, Module *> i(m_moduleMap->m_map);
        while(i.hasNext()) {
            i.next();
            if(i.value()->m_path == path) {
                m_moduleID = i.key();
            }
        }

    } else if(m_from == BibleQuoteUrl) {
    }
    return 0;
}
void UrlConverter::setFrom(const UrlType &urlType)
{
    m_from = urlType;
}
void UrlConverter::setTo(const UrlType &urlType)
{
    m_to = urlType;
}
void UrlConverter::setUrl(const QString &url)
{
    m_url = url;
}
