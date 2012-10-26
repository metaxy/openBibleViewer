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
#ifndef URLCONVERTER_H
#define URLCONVERTER_H
#include "src/core/module/modulemap.h"
#include "src/core/link/verseurl.h"
/**
  * UrlConverter is an converter for urls. Sometimes you have a url from a biblequote module and need to parse it.
  */
class UrlConverter
{
public:
    enum UrlType {
        None = 0,
        InterfaceUrl = 1,
        PersistentUrl = 2
    };
    UrlConverter();
    UrlConverter(const UrlType &from, const UrlType &to, const VerseUrl &url);

    void setFrom(const UrlType &urlType);
    void setTo(const UrlType &urlType);
    void setUrl(const VerseUrl &url);

    VerseUrl convert();
    void setModuleMap(ModuleMap *moduleMap);
    void setSettings(Settings *settings);
    void setV11n(QSharedPointer<Versification> v11n);
private:
    UrlType m_from;
    UrlType m_to;
    ModuleMap *m_moduleMap;
    Settings *m_settings;
    VerseUrl m_bibleUrl;
    bool m_setBookNames;
    QSharedPointer<Versification> m_v11n;
};

#endif // URLCONVERTER_H
