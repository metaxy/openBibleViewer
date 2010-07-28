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
#ifndef ZefaniaLex_H
#define ZefaniaLex_H
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include "src/core/settings.h"
/*!
 ZefaniaLex represents a zefaniaxml(lex) module

 @author Paul Walger <metaxy@walger.name>
*/
class ZefaniaLex
{
private:
    Settings *m_settings;
    QStringList m_id;
    QStringList m_title;
    QStringList m_trans;
    QStringList m_pron;
    QStringList m_desc;
public:
    ZefaniaLex();
    void setSettings(Settings *settings);
    QString loadFile(QString fileData, QString fileName);
    QString getStrong(const QString &strongID);
    QString m_modulePath;
};

#endif // ZefaniaLex_H
