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
#include "urlconverter.h"
#include "dbghelper.h"
#include "src/core/verse/verseurl.h"
UrlConverter::UrlConverter(const UrlType &from, const UrlType &to, const VerseUrl &url)
{
    m_from = from;
    m_to = to;
    m_bibleUrl = url;
    m_setBookNames = false;
}
UrlConverter::UrlConverter()
{
    m_setBookNames = false;
}
void UrlConverter::setModuleMap(ModuleMap *moduleMap)
{
    m_moduleMap = moduleMap;
}
void UrlConverter::setSettings(Settings *settings)
{
    m_settings = settings;
}
void UrlConverter::setV11n(Versification *v11n)
{
    m_v11n = v11n;
    m_setBookNames = true;
}

VerseUrl UrlConverter::convert()
{
    VerseUrl url = m_bibleUrl;
    if(m_to == InterfaceUrl) {
        //myDebug() << "to interface url";
        //myDebug() << m_bibleUrl.toString();
        url.clearRanges();
        foreach(VerseUrlRange range, m_bibleUrl.ranges()) {
            if(range.module() == VerseUrlRange::LoadModuleByUID) {
                foreach(Module * module, m_moduleMap->m_map) {
                    if(m_settings->savableUrl(module->path()) == range.moduleUID())  {
                        range.setModule(module->moduleID());
                        break;
                    }
                }
            }
            url.addRange(range);
        }
        for(int i = 0; i < m_bibleUrl.ranges().size(); i++) {
            url.unsetParam("b" + QString::number(i));
        }
    } else if(m_to == PersistentUrl) {

        //todo: catch errors
        url.clearRanges();
        QList<int> bookIDs;

        foreach(VerseUrlRange range, m_bibleUrl.ranges()) {
            if(range.module() == VerseUrlRange::LoadModuleByID && m_moduleMap->m_map.contains(range.moduleID())) {
                range.setModule(m_settings->savableUrl(m_moduleMap->m_map.value(range.moduleID())->path()));
            }
            url.addRange(range);
            bookIDs.append(range.bookID());
        }

        if(m_setBookNames) {
            for(int i = 0; i < bookIDs.size(); i++) {
                url.setParam("b" + QString::number(i), m_v11n->bookName(bookIDs.at(i)));
            }
        }

    }
    return url;
}
void UrlConverter::setFrom(const UrlType &urlType)
{
    m_from = urlType;
}
void UrlConverter::setTo(const UrlType &urlType)
{
    m_to = urlType;
}
void UrlConverter::setUrl(const VerseUrl &url)
{
    m_bibleUrl = url;
}
