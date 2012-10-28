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
#include "src/core/link/urlconverter.h"
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
void UrlConverter::setV11n(QSharedPointer<Versification> v11n)
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
                Module *m = m_moduleMap->moduleByUID(range.moduleUID());
                if(m)
                    range.setModule(m->moduleID());
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
        QList<int> moduleIDs;

        foreach(VerseUrlRange range, m_bibleUrl.ranges()) {
            if(range.module() == VerseUrlRange::LoadModuleByID && m_moduleMap->contains(range.moduleID())) {
                range.setModule(m_moduleMap->module(range.moduleID())->moduleUID());
            }
            url.addRange(range);
            bookIDs.append(range.bookID());
            moduleIDs.append(range.moduleID());
        }

        if(m_setBookNames) {
            for(int i = 0; i < bookIDs.size(); i++) {
                QSharedPointer<Versification> v11n;
                if(m_v11n == NULL) {
                    v11n = m_settings->getV11N(moduleIDs.at(i));
                } else {
                    v11n = m_v11n;
                }

                url.setParam("b" + QString::number(i), v11n->bookName(bookIDs.at(i)));
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
