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
#ifndef BIBLEURL_H
#define BIBLEURL_H
#include <QtCore/QString>
#include <QtCore/QHash>
#include "bibleurlrange.h"
#include "src/core/url.h"
class BibleUrl : public Url
{
public:
    BibleUrl();
    void addRange(const BibleUrlRange &range);
    void setParam(const QString &name, const QString &value);
    bool hasParam(const QString &name);
    QString getParam(const QString &name);


    QString toString();
    bool fromString(QString url);
    QList<BibleUrlRange> ranges();
private:
    QHash<QString, QString> m_params;
    QList<BibleUrlRange> m_ranges;
};

#endif // BIBLEURL_H
