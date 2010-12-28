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
#ifndef MODULE_H
#define MODULE_H
#include "src/core/settings/settings.h"
#include "src/module/zefania-lex.h"
#include "src/module/biblequote-dict.h"
#include <QtCore/QString>
#include "src/module/biblemodule.h"
class Module
{
public:
    Module(Module *parent = 0);
    enum ModuleClass {
        NoneClass = 0,
        FolderClass = 3,
        BibleModuleClass = 1,
        DictionaryModuleClass = 2
    };
    enum ModuleType {
        NoneType = 0,
        BibleQuoteModule = 1,
        ZefaniaBibleModule = 2,
        ZefaniaLexModule = 3,
        BibleQuoteDictModule = 4,
        TheWordBibleModule = 5
    };
    ModuleClass m_moduleClass;
    ModuleType m_moduleType;

    QString m_path;
    QString m_title;
    int m_id;

    BibleModule *m_bibleModule;
    ZefaniaLex *m_zefaniaLex;
    BibleQuoteDict *m_bibleQuoteDict;

    void setSettings(Settings *settings);

    void append(Module *child);
    Module *parent() const;
private:
    Module* m_parent;
    QList<Module *> m_children;
    Settings *m_settings;
};

#endif // MODULE_H
