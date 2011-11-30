/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger <metaxy@walger.name>
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
#ifndef METAINFO_H
#define METAINFO_H
#include <QtCore/QString>
#include "src/core/obvcore.h"
class MetaInfo
{
public:
    MetaInfo();
    QString name() const;
    QString shortName() const;
    QString uid() const;
    OBVCore::DefaultModule defaultModule() const;

    void setName(const QString &name);
    void setShortName(const QString &shortName);
    void setUID(const QString &uid);
    void setDefaultModule(const OBVCore::DefaultModule d);

    QString publisher;
    QString contributors;
    QString date;
    QString type;
    QString format;
    QString identifier;
    QString source;
    QString language;
    QString coverage;
    QString rights;
    QString subject;
    QString description;
    QString creator;

private:
    QString m_name;
    QString m_shortName;
    QString m_uid;
    OBVCore::DefaultModule m_defaultModule;
};

#endif // METAINFO_H
