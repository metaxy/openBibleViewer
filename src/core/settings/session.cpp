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
#include "session.h"

Session::Session()
{
}

void Session::setID(const QString &id)
{
    m_id = id;
}

QString Session::id() const
{
    return m_id;
}

void Session::setFile(QSettings *file)
{
    m_sessionFile = file;
}

QSettings * Session::file()
{
    return m_sessionFile;
}

void Session::setData(const QString &key, const QVariant &value)
{
    m_sessionFile->setValue(m_id + "/" + key, value);
}

QVariant Session::getData(const QString &key, const QVariant &defaultValue)
{
    return m_sessionFile->value(m_id + "/" + key, defaultValue);
}

bool Session::isSet(const QString &key)
{
    return !m_sessionFile->value(m_id + "/" + key).isNull();
}

void Session::clearGroup(const QString &key)
{
    m_sessionFile->beginGroup(m_id + "/" + key);
    m_sessionFile->remove("");
    m_sessionFile->endGroup();
}
