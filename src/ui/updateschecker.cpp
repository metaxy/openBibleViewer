#include "updateschecker.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtGui/QMessageBox>
#include <QtXml/QDomDocument>
#include "config.h"
#include "src/core/dbghelper.h"

UpdatesChecker::UpdatesChecker()
{
}
void UpdatesChecker::checkForUpdates()
{
    DEBUG_FUNC_NAME;

    myDebug() << "check for updates";
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("https://raw.github.com/metaxy/openBibleViewer/next/UPDATES")));
}
void UpdatesChecker::replyFinished(QNetworkReply* reply)
{
    DEBUG_FUNC_NAME
    myDebug() << "reply get";
    QDomDocument doc;
    doc.setContent(reply->readAll());
    QDomNode n = doc.firstChild();

    const QString versionType = QString(OBV_VERSION_STATUS);

    while (!n.isNull()) {
        myDebug() << n.nodeName();
        if (n.nodeName() == versionType) {
            QString version;
            QDomNode n2 = doc.firstChild();
            while (!n2.isNull()) {
                if (n2.nodeName() == "version") {
                    version = n2.toText().data();
                }
                n2 = n2.nextSibling();
            }
            QMessageBox msgBox;
            msgBox.setText(version);
            msgBox.exec();
            break;
        }
        n = n.nextSibling();
    }




}
void UpdatesChecker::updatesAvailable()
{

}
