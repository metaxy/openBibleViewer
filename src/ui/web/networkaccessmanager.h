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
