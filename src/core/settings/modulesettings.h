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
#include "src/core/verse/versification/versification_cache.h"
/**
  * ModuleSettings represents a settings class for modules.
  */
class ModuleSettings
{
public:
    ModuleSettings();
    ~ModuleSettings();
    QString modulePath;
    QString moduleName;
    QString moduleShortName;
    //todo: Use somthing like Module::ModuleType
    QString moduleType;
    QString encoding;

    int zefbible_textFormatting;//0 = Neue Zeile nach Vers, 1 = Unformatierter Textblock
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
    QString versification;
    bool hasVersfication;
    Versification *v11n;
    /**
      * If the is no settingsfile or it is empty v11n remains the same
      */
    void loadVersification();
    void saveVersification();


};

#endif // MODULESETTINGS_H
