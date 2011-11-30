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
#ifndef UPDATESCHECKER_H
#define UPDATESCHECKER_H

#include <QtCore/QObject>
#include "src/core/basicclass.h"
#include <QtNetwork/QNetworkReply>


class UpdatesChecker : public QObject, public BasicClass
{
    Q_OBJECT
public:
    UpdatesChecker(QObject *parent = 0);
    void checkForUpdates();

public slots:

    void replyFinished(QNetworkReply* reply);

private slots:
    void updatesAvailable();



private:
    bool isImportantUpdate;
};

#endif // UPDATESCHECKER_H
