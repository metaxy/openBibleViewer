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
#include "session.h"

Session::Session()
{
}
void Session::setData(QString key, QVariant value)
{
    m_data.insert(key, value);
}
QVariant Session::getData(QString key, QVariant defaultValue = QVariant()) const
{
    return m_data.value(key, defaultValue);
}
QVariant Session::getData(QString key) const
{
    return m_data.value(key);
}
bool Session::isSet(QString key) const
{
    if(m_data.value(key).isNull())
        return false;
    return true;
}

QMapIterator<QString, QVariant> Session::getInterator() const
{
    QMapIterator<QString, QVariant> i(m_data);
    return i;
}

