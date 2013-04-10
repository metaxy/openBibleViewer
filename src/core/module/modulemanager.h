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
#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H
#include <QFileInfoList>
#include <QList>
#include <QDir>
#include <QObject>
#include <QSharedPointer>

#include <QStandardItemModel>
#include <QProgressDialog>
#include <QApplication>

#include "src/core/settings/settings.h"

#include "src/core/module/bible/bible.h"
#include "src/core/module/versetable.h"
#include "src/core/module/dictionary/dictionary.h"
#include "src/core/module/module.h"
#include "src/core/module/modulemap.h"

#include "src/core/notes/notes.h"
#include "src/core/dbghelper.h"
#include "src/core/link/urlconverter.h"
/**
 * ModuleManager is universal module manager for all kind of module types. It loads them and creates Module.
 * It is part of the BasicClass.
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

    bool dictionaryLoaded(const Dictionary *dict);
    bool metaModuleLoaded(const SimpleModuleClass *m) const;
    bool verseTableLoaded(const VerseTable *table) const;

    void initSimpleModule(SimpleModuleClass *m) const;
    void initVerseModule(VerseModule *m) const;

    QString notePos2Link(const QString &pos);
    QString notePos2Text(const QString &pos);

    Module * getModule(const int moduleID);
    bool contains(const int moduleID);


    Notes *m_notes;
    QSharedPointer<ModuleMap> m_moduleMap;

    QStringList getBibleTitles();
    QStringList getBiblePaths();
    QList<int> getBibleIDs();
    void checkCache(const int moduleID);
    TextRangesVerseModule * newTextRangesVerseModule(const int moduleID, QPoint p, VerseTable *table);
    TextRangesVerseModule * newTextRangesVerseModule(const int moduleID);


    void newDisplaySettings();
    ModuleDisplaySettings * moduleDisplaySetings();

    /**
      * Returns a new VerseTable with a Bible.
      */
    VerseTable * newVerseTable();

    static bool sortModuleByPop(const ModuleSettings* s1, const ModuleSettings* s2);

private:

    void loadModule(Module *moduleParent, ModuleSettings *setttings);
    void makeSureItHasLoaded(ModuleSettings *settings);

    Module *m_rootModule;
    Settings *m_settings;
    ModuleDisplaySettings *m_moduleDisplaySettings;
};

#endif // MODULEMANAGER_H
