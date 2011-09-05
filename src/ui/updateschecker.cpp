#include "updateschecker.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtGui/QMessageBox>
#include <QtXml/QDomDocument>
#include "config.h"
#include "src/core/dbghelper.h"

UpdatesChecker::UpdatesChecker(QObject *parent) :
    QObject(parent)
{
}
void UpdatesChecker::checkForUpdates()
{
    DEBUG_FUNC_NAME;

    myDebug() << "check for updates2";
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("http://openbv.uucyc.name/UPDATES")));
}
void UpdatesChecker::replyFinished(QNetworkReply* reply)
{
    DEBUG_FUNC_NAME
    myDebug() << "reply get";
    QDomDocument doc;
    doc.setContent(reply->readAll());
    QDomNode n = doc.documentElement().firstChild();

    const QString versionType = QString(OBV_VERSION_STATUS);
    myDebug() << "version type = " << versionType;

    while (!n.isNull()) {
        if (n.nodeName() == versionType) {
            QString version;
            QDomNode n2 = n.firstChild();
            while (!n2.isNull()) {
                if (n2.nodeName() == "version") {
                    version = n2.firstChild().toText().data();
                    break;
                }
                n2 = n2.nextSibling();
            }
            myDebug() << version;
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
