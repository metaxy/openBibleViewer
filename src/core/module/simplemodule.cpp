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
#include "simplemodule.h"
#include <QMessageBox>
SimpleModule::SimpleModule() : m_settings(nullptr), m_loaded(false), m_failed(false)
{
}
SimpleModule::~SimpleModule()
{

}

void SimpleModule::setSettings(Settings *settings)
{
    m_settings = settings;
}
bool SimpleModule::loaded() const
{
    return m_loaded;
}
bool SimpleModule::failed() const
{
    return m_failed;
}
void SimpleModule::fail(QString title, QString text)
{
    QMessageBox::warning(nullptr, title, text, QMessageBox::Ok);
    m_failed = true;

}
