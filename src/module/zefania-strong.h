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
#ifndef ZEFANIASTRONG_H
#define ZEFANIASTRONG_H
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include "../core/config.h"
#include "../core/moduleconfig.h"
class zefaniaStrong
{
private:
    struct settings_s zefset;
    struct moduleConfig mConfig;
    QStringList m_id;
    QStringList m_title;
    QStringList m_trans;
    QStringList m_pron;
    QStringList m_desc;
public:
    zefaniaStrong();
    int setSettings(struct settings_s settings, struct moduleConfig mConfig);
    QString loadFile(QString fileData, QString fileName);
    bool loadDataBase(QString fileName);
    QString getStrong(QString strongID);
};

#endif // ZEFANIASTRONG_H
