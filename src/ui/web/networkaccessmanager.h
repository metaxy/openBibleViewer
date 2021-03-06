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
#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H




#include <QNetworkAccessManager>
#include <QMultiHash>
#include <QByteArray>
#include <QUrl>
#include "src/core/blockrules.h"

class QWebFrame;


class NetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT

public:
    explicit NetworkAccessManager(QObject *parent);
    void setBlockRules(const BlockRules &rules);
    BlockRules m_blockRules;
    bool m_doBlock;
protected:
    virtual QNetworkReply *createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice *outgoingData = 0);

private Q_SLOTS:
    void applyHidingBlockedElements(bool);
    
private:
    QMultiHash<QWebFrame*, QUrl> m_blockedRequests;


};

#endif // NETWORKACCESSMANAGER_H
