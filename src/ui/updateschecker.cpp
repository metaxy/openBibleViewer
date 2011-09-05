#include "updateschecker.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtGui/QMessageBox>
#include <QtXml/QDomDocument>
#include "config.h"
#include "src/core/dbghelper.h"
#include "src/core/version.h"

UpdatesChecker::UpdatesChecker(QObject *parent) :
    QObject(parent)
{
}
void UpdatesChecker::checkForUpdates()
{
    if(!m_settings->checkForUpdates) {
        return;
    }
    myDebug() << "checking for Updates";
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("http://openbv.uucyc.name/UPDATES")));
}
void UpdatesChecker::replyFinished(QNetworkReply* reply)
{
    QDomDocument doc;
    doc.setContent(reply->readAll());
    QDomNode n = doc.documentElement().firstChild();

    const QString versionType = QString(OBV_VERSION_STATUS);

    while (!n.isNull()) {
        if (n.nodeName() == versionType) {
            QString version;
            QDomNode n2 = n.firstChild();
            while (!n2.isNull()) {
                if (n2.nodeName() == "version") {
                    version = n2.firstChild().toText().data();
                }
                n2 = n2.nextSibling();
            }
            if(version.isEmpty())
                return;

            Version oldVersion(QString(OBV_VERSION_NUMBER));
            Version newVersion(version);
            myDebug() << "new Version = " << version;

            if(oldVersion.maintenanceVersion() < newVersion.maintenanceVersion() ||
               oldVersion.minorVersion() < newVersion.minorVersion() ||
               oldVersion.majorVersion() < newVersion.majorVersion()) {

                QMessageBox msgBox;
                msgBox.setText("New Version avaiable " + version);
                msgBox.exec();
            }
            break;
        }
        n = n.nextSibling();
    }

}
void UpdatesChecker::updatesAvailable()
{

}
