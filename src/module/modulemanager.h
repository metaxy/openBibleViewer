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
#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H
#include <QtCore/QFileInfoList>
#include <QtCore/QList>
#include <QtCore/QDir>
#include <QtCore/QObject>

#include <QtGui/QStandardItemModel>
#include <QtGui/QProgressDialog>
#include <QtGui/QApplication>

#include "src/core/settings/settings.h"

#include "src/module/bible/bible.h"
#include "src/module/versetable.h"
#include "src/module/dictionary/dictionary.h"
#include "src/module/module.h"
#include "src/module/modulemap.h"

#include "src/core/notes/notes.h"
#include "src/core/dbghelper.h"
#include "src/core/urlconverter.h"
#include "src/ui/modulemodel.h"
/**
 * ModuleManager is universal module manager for all kind of module types. It loads them and creates Module.
 */
class ModuleManager
{

public:
    ModuleManager();
    ~ModuleManager();
    int loadAllModules();

    void setSettings(Settings *settings);
    void setNotes(Notes *notes);
    void setModuleDisplaySettings(ModuleDisplaySettings *moduledisplaysettings);

    bool dictionaryLoaded();
    bool dictionaryLoaded(const Dictionary *dict);

    bool metaModuleLoaded(const SimpleModuleClass *m) const;
    bool verseTableLoaded(const VerseTable *table) const;

    void initSimpleModule(SimpleModuleClass *m) const;
    void initVerseModule(VerseModule *m) const;

    Dictionary* dictionary();

    QString notePos2Link(const QString &pos);
    QString notePos2Text(const QString &pos);

    Module * getModule(const int moduleID);
    bool contains(const int moduleID);

    QStandardItemModel *m_moduleModel;


    Notes *m_notes;
    ModuleMap *m_moduleMap;

    Dictionary *m_dictionary;

    QStringList getBibleTitles();
    QStringList getBiblePaths();
    QList<int> getBibleIDs();
    void checkCache(const int moduleID);
    VerseModule * newVerseModule(const int moduleID, QPoint p, VerseTable *table);
    ModuleDisplaySettings *m_moduleDisplaySettings;
    static OBVCore::ModuleType recognizeModuleType(const QString &fileName);

private:

    void loadModule(Module *moduleParent, ModuleSettings *setttings);
    void makeSureItHasLoaded(ModuleSettings *settings);

    Module *m_rootModule;
    Settings *m_settings;
};

#endif // MODULEMANAGER_H
