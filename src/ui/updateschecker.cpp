#include "updateschecker.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtGui/QMessageBox>
#include <QtXml/QDomDocument>
#include "config.h"
#include "src/core/dbghelper.h"
#include "src/core/version.h"
#include "src/ui/dialog/updatecheckerdialog.h"

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
    const QString os = "linux";
    while (!n.isNull()) {
        if (n.nodeName() == versionType) {
            QString version;
            QString link;
            QString desc;
            QDomNode n2 = n.firstChild();
            while (!n2.isNull()) {
                if (n2.nodeName() == "version") {
                    version = n2.firstChild().toText().data();
                } else if(n2.nodeName() == "link" && n2.toElement().attribute("os") == os) {
                    link = n2.firstChild().toText().data();
                } else if(n2.nodeName() == "desc") {
                    desc = n2.firstChild().toText().data();
                }
                n2 = n2.nextSibling();
            }
            if(version.isEmpty())
                return;

            Version oldVersion(QString(OBV_VERSION_NUMBER));
            Version newVersion(version);
            myDebug() << "new Version = " << version;
            if(m_settings->session.file()->value(m_settings->session.id()+ "/" + "skipVersion") == version)
                return;

            if(oldVersion.maintenanceVersion() < newVersion.maintenanceVersion() ||
               oldVersion.minorVersion() < newVersion.minorVersion() ||
               oldVersion.majorVersion() < newVersion.majorVersion()) {

                QString text = tr("A new version of openBibleViewer is available: ") + version + " ";
                if(!link.isEmpty()) {
                    text += tr("You can download it at") +"<a href='"+link+"'> "+ link + "</a>.";
                }
                if(!desc.isEmpty()) {
                    text += desc;
                }
                myDebug() << text;
                UpdateCheckerDialog dialog(0);
                setAll(&dialog);
                dialog.setText(text);
                dialog.setUrl(link);
                dialog.setVersion(version);
                dialog.exec();
            }
            break;
        }
        n = n.nextSibling();
    }

}
void UpdatesChecker::updatesAvailable()
{

}
