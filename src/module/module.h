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
#ifndef MODULE_H
#define MODULE_H
#include <QtCore/QString>
#include "src/core/settings/settings.h"
#include "src/module/dictionary/dictionarymodule.h"
#include "src/module/bible/biblemodule.h"
#include "src/module/moduletools.h"
/**
 * Module is represents a module. Every avaible module has an instance of this class.
 * But m_bibleModule, m_dictionaryModule can be NULL. They are loaded only if needed.
 * That means every module with a moduleID is represented by a instance of Module.
 * But all the visible modules in the windows or tabs links only to one of that instances.
 */
class Module
{
public:
    Module(Module *parent = 0);
    virtual ~Module();

    QString path() const;
    int moduleID() const;
    QString moduleUID() const;
    ModuleTools::ModuleClass moduleClass() const;
    ModuleTools::ModuleType moduleType() const;

    void setPath(const QString &path);
    void setModuleID(const int moduleID);
    void setModuleClass(const ModuleTools::ModuleClass &c);
    void setModuleType(const ModuleTools::ModuleType &t);

    QWeakPointer<BibleModule> m_bibleModule;
    QWeakPointer<DictionaryModule> m_dictionaryModule;

    QSharedPointer<BibleModule> newBibleModule(const ModuleTools::ModuleType type);
    QSharedPointer<DictionaryModule> newDictionaryModule(const ModuleTools::ModuleType type);

    void setSettings(Settings *settings);


    Module *parent() const;

    static QString moduleTypeName(ModuleTools::ModuleType type);
private:
    Module* m_parent;
    QList<Module *> m_children;
    Settings *m_settings;

    QString m_path;
    int m_id;
    ModuleTools::ModuleClass m_moduleClass;
    ModuleTools::ModuleType m_moduleType;
    void append(Module *child);
};

#endif // MODULE_H
