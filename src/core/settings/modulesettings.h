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
#ifndef MODULESETTINGS_H
#define MODULESETTINGS_H
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QSettings>
#include "src/core/verse/versification.h"
#include "src/core/verse/versification/versification_kjv.h"
#include "src/core/verse/versification/versification_cache.h"
#include "src/core/obvcore.h"
#include "src/core/dbghelper.h"
/**
  * ModuleSettings represents a settings class for modules.
  */
class ModuleSettings
{
public:
    ModuleSettings();
    ModuleSettings(ModuleSettings *parent);
    ~ModuleSettings();

    void setParent(ModuleSettings *parent);
    ModuleSettings *parent() const;

    QList<ModuleSettings *> children();
    void appendChild(ModuleSettings* child);
    void removeChild(ModuleSettings* child);


    int moduleID;
    QString modulePath;
    QString moduleName;
    QString moduleShortName;
    OBVCore::ModuleType moduleType;

    QString encoding;
    bool useParentsSettings;

    enum ZefBible_TextFormating {
        NewLine = 0,//Neue Zeile nach Vers
        Block = 1//Unformatierter Textblock
    };

    ZefBible_TextFormating zefbible_textFormatting;

    bool zefbible_hardCache;
    bool zefbible_softCache;
    bool zefbible_showStrong;
    bool zefbible_showStudyNote;
    bool biblequote_removeHtml;

    QString styleSheet;

    /**
      * It can be a path to a versification file
      * or a name of a versification e.g kjv
      */
    QString versificationFile;
    QString versificationName;
    bool hasVersfication;
    Versification *v11n;
    /**
      * If the is no settingsfile or it is empty v11n remains the same
      */
    void loadVersification();
    void saveVersification();

    int parentID;

private:

    ModuleSettings *m_parent;
    QList<ModuleSettings *> m_children;
};

#endif // MODULESETTINGS_H
