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
#ifndef SEARCHAPI_H
#define SEARCHAPI_H
#include <QObject>
#include "src/core/basicclass.h"
#include <QtWebKit/QWebFrame>
#include "src/core/search/search.h"

class SearchApi : public QObject, public BasicClass
{
    Q_OBJECT
public:
    explicit SearchApi(QObject *parent = 0);
    QString name() const;
signals:
    
public slots:
    QVariantList search(const int moduleID, const QString &query);
};

#endif // SEARCHAPI_H
