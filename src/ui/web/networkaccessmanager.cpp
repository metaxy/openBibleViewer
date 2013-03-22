#include "networkaccessmanager.h"

#include <QNetworkReply>
#include <QTimer>
#include <QWebElement>
#include <QWebFrame>
#include <QWidget>
#include "src/core/dbghelper.h"

class NullNetworkReply : public QNetworkReply
{
public:
    NullNetworkReply(const QNetworkRequest &req, QObject* parent = 0)
        : QNetworkReply(parent)
    {
        setRequest(req);
        setUrl(req.url());
        setHeader(QNetworkRequest::ContentLengthHeader, 0);
        setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
        setError(QNetworkReply::ContentAccessDenied, "Blocked by ad filter");
        setAttribute(QNetworkRequest::User, QNetworkReply::ContentAccessDenied);
        QTimer::singleShot(0, this, SIGNAL(finished()));
    }

    virtual void abort() {}
    virtual qint64 bytesAvailable() const
    {
        return 0;
    }

protected:
    virtual qint64 readData(char*, qint64)
    {
        return -1;
    }
};


// ----------------------------------------------------------------------------------------------


#define     HIDABLE_ELEMENTS    "audio,img,embed,object,iframe,frame,video"


static void hideBlockedElements(const QUrl& url, QWebElementCollection& collection)
{
    for (QWebElementCollection::iterator it = collection.begin(); it != collection.end(); ++it)
    {
        const QUrl baseUrl((*it).webFrame()->baseUrl());
        QString src = (*it).attribute("src");

        if (src.isEmpty())
            src = (*it).evaluateJavaScript("this.src").toString();

        if (src.isEmpty())
            continue;
        const QUrl resolvedUrl(baseUrl.resolved(src));
        if (url == resolvedUrl)
        {
            (*it).removeFromDocument();
        }
    }
}


// ----------------------------------------------------------------------------------------------


NetworkAccessManager::NetworkAccessManager(QObject *parent)
    : QNetworkAccessManager(parent), m_doBlock(false)
{
}

void NetworkAccessManager::setBlockRules(const BlockRules &rules)
{
    m_blockRules = rules;
    m_doBlock = true;
}

QNetworkReply *NetworkAccessManager::createRequest(Operation op, const QNetworkRequest &req, QIODevice *outgoingData)
{
    if(!m_doBlock)  return QNetworkAccessManager::createRequest(op, req, outgoingData);

    bool blocked = false;

    // Handle GET operations with AdBlock
    if (op == QNetworkAccessManager::GetOperation) {
        const QString url = req.url().toString().toLower();
        foreach(const QString &m, m_blockRules.m_blackListUrl) {
            if(url.contains(m)) blocked = true;
        }
    }

    if (!blocked)
    {
        return QNetworkAccessManager::createRequest(op, req, outgoingData);
    }
    // if we blocked something then we should hide it
    QWebFrame* frame = qobject_cast<QWebFrame*>(req.originatingObject());
    if (frame)
    {
        if (!m_blockedRequests.contains(frame))
            connect(frame, SIGNAL(loadFinished(bool)), this, SLOT(applyHidingBlockedElements(bool)));
        m_blockedRequests.insert(frame, req.url());
    }
    myDebug() << "blocked " << req.url();
    return new NullNetworkReply(req, this);
}


void NetworkAccessManager::applyHidingBlockedElements(bool ok)
{
    if (!ok)
        return;

    QWebFrame* frame = qobject_cast<QWebFrame*>(sender());
    if (!frame)
        return;

    QList<QUrl> urls = m_blockedRequests.values(frame);
    if (urls.isEmpty())
        return;

    QWebElementCollection collection = frame->findAllElements(HIDABLE_ELEMENTS);
    if (frame->parentFrame())
        collection += frame->parentFrame()->findAllElements(HIDABLE_ELEMENTS);

    Q_FOREACH(const QUrl & url, urls)
        hideBlockedElements(url, collection);
}
