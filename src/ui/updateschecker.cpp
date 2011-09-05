#include "updateschecker.h"
#include <QtNetwork/QNetworkAccessManager>
#include "src/core/dbghelper.h"

UpdatesChecker::UpdatesChecker()
{
}
void UpdatesChecker::checkForUpdates()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("https://raw.github.com/metaxy/openBibleViewer/master/UPDATES")));
}
void UpdatesChecker::replyFinished(QNetworkReply* reply)
{
    myDebug() << reply->readAll();
}
