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
#include "src/module/module.h"

Module::Module(Module *parent)
{
    m_bibleQuote = 0;
    m_zefaniaBible = 0;
    m_zefaniaStrong = 0;
    m_parent = parent;

}
void Module::setSettings(Settings *settings)
{
    m_settings = settings;
}
Module* Module::parent() const
{
    return m_parent;
}

void Module::append(Module* child)
{
    m_children.append(child);
}
