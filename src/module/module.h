/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
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
#include <QtCore/QString>
class Module
{
public:
    Module();
    enum ModuleClass {
        NoneType = 0,
        BibleModule = 1,
        StrongModule = 2
    };
    enum ModuleType {
        NoneClass = 0,
        BibleQuoteModule = 1,
        ZefaniaBibleModule = 2,
        ZefaniaStrongModule = 3
    };
    int m_moduleClass;
    int m_moduleType;
    QString m_path;
    QString m_title;
    int m_id;
    void setSettings(Settings *settings);
    Settings *m_settings;
};

#endif // MODULE_H
