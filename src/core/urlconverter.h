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
#ifndef URLCONVERTER_H
#define URLCONVERTER_H
#include <QtCore/QString>
#include <QtCore/QStringList>
#include "src/module/modulemap.h"
/*!
  UrlConverter is an convrter for urls. Sometimes you have a url from a biblequote module and need to pharse it.

  */
class UrlConverter
{
public:
    enum UrlType {
        None = 0,
        InterfaceUrl = 1,
        PersistentUrl = 2,
        BibleQuoteUrl = 3
    };
    UrlConverter(const UrlType &from, const UrlType &to, const QString &url);

    void setFrom(const UrlType &urlType);
    void setTo(const UrlType &urlType);
    void setUrl(const QString &url);
    int pharse();

    QString convert();
    int m_moduleID;
    QString m_path;
    int m_chapterID;
    int m_bookID;
    int m_verseID;
    QString m_bookName;
    void setModuleMap(ModuleMap *moduleMap);
private:
    UrlType m_from;
    UrlType m_to;
    QString m_url;
    ModuleMap *m_moduleMap;
};

#endif // URLCONVERTER_H
