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
#include "src/core/notes/notes.h"
#include "src/core/link/urlconverter.h"
VerseUrl Notes::getLink(const QString &noteID, QSharedPointer<ModuleMap> map, Settings *settings)
{
    if(m_cache.contains(noteID)) return m_cache[noteID];
    const QString link = getRef(noteID, "link");
    VerseUrl url;
    url.fromStringUrl(link);
    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
    urlConverter.setSettings(settings);
    urlConverter.setModuleMap(map.data());
    const VerseUrl newUrl = urlConverter.convert();
    m_cache[noteID] = newUrl;
    return newUrl;
}
