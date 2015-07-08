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
#ifndef SIMPLEMODULE_H
#define SIMPLEMODULE_H
#include "src/core/module/metainfo.h"
#include "src/core/settings/settings.h"
class SimpleModule
{
public:
    SimpleModule();
    virtual ~SimpleModule();
    void setSettings(Settings *settings);
    virtual MetaInfo readInfo(const QString &name) = 0;
    virtual bool loaded() const;
    virtual bool failed() const;
    virtual void fail(QString title, QString text);
protected:
    Settings *m_settings;
    bool m_loaded;
    bool m_failed;
};

#endif // SIMPLEMODULE_H
