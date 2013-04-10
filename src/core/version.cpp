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

#include "version.h"
#include "src/core/dbghelper.h"
#include <QStringList>

Version::Version(const QString& versionString)
{
    QStringList split = versionString.split(".");
    if(split.size() == 3) {
        m_major = split.at(0).toInt();
        m_minor = split.at(1).toInt();
        m_maintenance = split.at(2).toInt();
    } else if(split.size() == 2) {
        m_major = split.at(0).toInt();
        m_minor = split.at(1).toInt();
        m_maintenance = 0;
    }
}
int Version::maintenanceVersion() const
{
    return m_maintenance;
}
int Version::majorVersion() const
{
    return m_major;
}
int Version::minorVersion() const
{
    return m_minor;
}

QString Version::toString()
{
    return QString::number(majorVersion()) + "." + QString::number(minorVersion()) + "." + QString::number(maintenanceVersion());
}
