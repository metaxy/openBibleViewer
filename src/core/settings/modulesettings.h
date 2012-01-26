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
#include <QtCore/QSharedPointer>
#include "src/core/verse/versification.h"
#include "src/core/verse/versification/versification_kjv.h"
#include "src/core/verse/versification/versification_cache.h"
#include "src/module/moduletools.h"
#include "src/core/dbghelper.h"
#include "src/core/settings/moduledisplaysettings.h"
/**
  * ModuleSettings represents a settings class for modules.
  * Creating ModuleSetting is verry tricky. You have to set the parents on your own, also not not forget to append it the parent.
  */
class ModuleSettings
{
public:
    ModuleSettings();
    ModuleSettings(ModuleSettings *parent);
    ~ModuleSettings();

    enum ZefBible_TextFormating {
        NewLine = 0,//Neue Zeile nach Vers
        Block = 1//Unformatierter Textblock
    };

    void setParent(ModuleSettings *parent);
    ModuleSettings *parent() const;

    QSharedPointer<ModuleDisplaySettings> displaySettings() const;
    void setDisplaySettings(QSharedPointer<ModuleDisplaySettings> settings);
    void removeDisplaySettings();

    QList<ModuleSettings *> children() const;
    void appendChild(ModuleSettings* child);
    void removeChild(ModuleSettings* child);
    void clearChildren();
    QString name(bool preferShort = false) const;

    int moduleID;
    QString moduleUID;
    QString modulePath;
    QString moduleName;
    QString moduleShortName;
    ModuleTools::ModuleType moduleType;

    QString moduleLanguage;//ISO 639-2

    QString encoding;
    bool useParentSettings;

    ZefBible_TextFormating zefbible_textFormatting;

    bool zefbible_hardCache;
    bool zefbible_softCache;
    bool biblequote_removeHtml;

    QString styleSheet;

    ModuleTools::DefaultModule defaultModule;
    ModuleTools::ContentType contentType;

    /**
      * It can be a path to a versification file
      * or a name of a versification e.g kjv
      */
    QString versificationFile;
    QString versificationName;
    bool hasVersfication;
    QWeakPointer<Versification> v11n;

    /**
      * If the is no settingsfile or it is empty, v11n remains the same(mostly NULL)
      */
    QSharedPointer<Versification> loadVersification();
    void saveVersification();

    QSharedPointer<Versification> getV11n();

    bool noV11n();

    int parentID;

    int stats_usageCount;
    int stats_timesOpend;


private:
    QSharedPointer<ModuleDisplaySettings> m_displaySettings;
    ModuleSettings *m_parent;
    QList<ModuleSettings *> m_children;
};

#endif // MODULESETTINGS_H
