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
#ifndef WEBDICTIONARY_H
#define WEBDICTIONARY_H
#include "src/module/dictionary/dictionarymodule.h"
#include "src/module/simplemoduleclass.h"
#include "src/core/settings/moduledisplaysettings.h"
#include "src/module/searchablemodule.h"
#include <QtScript/QScriptEngine>
#include <QtCore/QUrl>

class WebDictionary : public SearchableModule, public DictionaryModule
{
public:
    WebDictionary();
    ~WebDictionary();
    void loadModuleData(const int moduleID, const QString &name = "");

    Response *getEntry(const QString &entry);
    QStringList getAllKeys();

    void search(SearchQuery query, SearchResult *result);

    MetaInfo readInfo(const QString &name);

    QString pharseUrl(const QUrl &url);
    Response::ResponseType responseType() const;
private:
    bool m_loaded;

    QString m_pharseScript;
    QString m_pharseOutScript;

    QString m_name;
    QString m_shortName;
    QString m_desc;
    QString m_type;

    QString m_url;

    bool loaded();
    int m_loadedModuleID;

    QScriptEngine myEngine;
};

#endif // WEBDICTIONARY_H
