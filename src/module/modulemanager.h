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
#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H
#include "src/core/notes.h"
#include "src/core/settings.h"
#include "src/module/bible.h"
#include "src/module/biblelist.h"
#include "src/module/dictionary.h"
#include "src/module/module.h"
#include "src/module/modulemap.h"

#include <QtCore/QFileInfoList>
#include <QtCore/QList>
#include <QtGui/QStandardItemModel>


class ModuleManager
{

public:
    ModuleManager();
    ~ModuleManager();
    int loadAllModules();
    void setSettings(Settings *settings);
    void setNotes(Notes *notes);
    void setBibleDisplaySettings(BibleDisplaySettings *bibleDisplaySettings);
    bool bibleLoaded();
    bool strongLoaded();
    Bible* bible();
    void initBible(Bible *b = 0);
    BibleList* bibleList();
    Dictionary* dictionary();
    QString notePos2Link(const QString &pos);
    QString notePos2Text(const QString &pos);

    Module * getModule(const int &moduleID);
    bool contains(const int &moduleID);

    QStandardItemModel *m_moduleModel;
    Settings *m_settings;
    Notes *m_notes;
    ModuleMap *m_moduleMap;

    BibleList *m_bibleList;
    Dictionary *m_dictionary;

    QStringList getBibleTitles();
    QStringList getBiblePaths();
    QList<int> getBibleIDs();
    void checkCache(const int &moduleID);
    Bible * newBible(const int &moduleID, QPoint p);
    BibleDisplaySettings *m_bibleDisplaySettings;

private:
    QFileInfoList scan(const QString &path, int level);

};

#endif // MODULEMANAGER_H
