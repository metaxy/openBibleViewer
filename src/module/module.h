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
#include "src/core/settings.h"
#include "src/module/biblequote.h"
#include "src/module/zefania-bible.h"
#include "src/module/zefania-strong.h"
#include <QtCore/QString>
class Module
{
public:
    Module(Module *parent=0);
    enum ModuleClass {
        NoneClass = 0,
        FolderClass = 3,
        BibleModule = 1,
        StrongModule = 2
    };
    enum ModuleType {
        NoneType = 0,
        BibleQuoteModule = 1,
        ZefaniaBibleModule = 2,
        ZefaniaStrongModule = 3
    };
    ModuleClass m_moduleClass;
    ModuleType m_moduleType;

    QString m_path;
    QString m_title;
    int m_id;
    BibleQuote *m_bibleQuote;
    ZefaniaBible *m_zefaniaBible;
    ZefaniaStrong m_zefaniaStrong;

    void setSettings(Settings *settings);


    void append(Module *child);
    Module *parent() const;
private:
    Module* m_parent;
    QList<Module *> m_children;
    Settings *m_settings;
};

#endif // MODULE_H
