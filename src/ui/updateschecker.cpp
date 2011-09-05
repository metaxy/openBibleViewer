#include "updateschecker.h"
#include <QtNetwork/QNetworkAccessManager>
#include "src/core/dbghelper.h"
#include <QtXml/QDomDocument>
#include "config.h"

UpdatesChecker::UpdatesChecker()
{
}
void UpdatesChecker::checkForUpdates()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("https://raw.github.com/metaxy/openBibleViewer/next/UPDATES")));
}
void UpdatesChecker::replyFinished(QNetworkReply* reply)
{
    QDomDocument doc;
    doc.setContent(reply->readAll());
    const QDomNodeList l = doc.childNodes();
    const QString versionType = QString(OBV_VERSION_STATUS);

    for(int i = 0; i< l.size(); i++) {
        QDomNode node = l.at(i);
        if(node.nodeName() == versionType) {

            break;
        }
    }

}
void UpdatesChecker::updatesAvailable()
{

}
