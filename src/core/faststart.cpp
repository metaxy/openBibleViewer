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
#include "faststart.h"
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include "src/core/dbghelper.h"
FastStart::FastStart()
{
    m_changed = false;
}
void FastStart::setSettings(Settings *settings)
{
    m_settings = settings;
}

bool FastStart::load()
{
    DEBUG_FUNC_NAME
    QFile file(filePath());
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> m_data;
    file.close();
    return true;
}

bool FastStart::save()
{
    QFile file(filePath());
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << m_data;
    file.close();
    return true;
}

bool FastStart::remove()
{
    QDir dir(m_settings->homePath);
    return dir.remove(filePath());
}

void FastStart::setFileNames(const QString &dir, const QStringList &files)
{
    m_data[dir] = files;
    m_changed = true;
}

QStringList FastStart::getFileNames(const QString &dir) const
{
    return m_data.value(dir);
}

bool FastStart::hasCache(const QString &dir) const
{
    return m_data.contains(dir);
}

bool FastStart::changed() const
{
    return m_changed;
}

QString FastStart::filePath() const
{
    return m_settings->homePath + "/faststart";
}
