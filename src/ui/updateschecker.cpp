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
#include "updateschecker.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QMessageBox>
#include <QtXml/QDomDocument>
#include <QPointer>
#include "config.h"
#include "src/core/dbghelper.h"
#include "src/core/version.h"
#include "src/ui/dialog/updatecheckerdialog.h"
#include <QSslConfiguration>
UpdatesChecker::UpdatesChecker(QObject *parent) :
    QObject(parent)
{
}
UpdatesChecker::~UpdatesChecker()
{
}

void UpdatesChecker::checkForUpdates()
{
    if(!m_settings->checkForUpdates) {
        this->deleteLater();
        return;
    }

    myDebug() << "checking for Updates";
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("http://metaxy.github.com/UPDATES_OBV.xml")));
}
void UpdatesChecker::replyFinished(QNetworkReply* reply)
{
    QDomDocument doc;
    doc.setContent(reply->readAll());
    QDomNode n = doc.documentElement().firstChild();

    const QString versionType = QString(OBV_VERSION_STATUS);
#if defined(Q_OS_LINUX)
    const QString os = "linux";
#elif defined(Q_OS_WIN)
    const QString os = "windows";
#elif defined(Q_OS_DARWIN)
    const QString os = "darwin";
#else
	const QString os = "windows";
#endif
    QString link;
    QString text;
    QString version;
    while(!n.isNull()) {
        if(n.nodeName() == versionType) {

            QString desc;
            QDomNode n2 = n.firstChild();
            while(!n2.isNull()) {
                if(n2.nodeName() == "version") {
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
	    myDebug() << "new Version = " << version << "my version" << oldVersion.toString();
            if(m_settings->session.file()->value(m_settings->session.id() + "/" + "skipVersion") == version)
                return;

            if(oldVersion.maintenanceVersion() < newVersion.maintenanceVersion() ||
                    oldVersion.minorVersion() < newVersion.minorVersion() ||
                    oldVersion.majorVersion() < newVersion.majorVersion()) {

                text = tr("A new version of openBibleViewer is available: ") + version + " ";
                if(!link.isEmpty()) {
                    text += tr("You can download it at") + "<a href='" + link + "'> " + link + "</a>.";
                }
                if(!desc.isEmpty()) {
                    text += desc;
                }
                myDebug() << text;
                QPointer<UpdateCheckerDialog> dlg = new UpdateCheckerDialog(nullptr);
				setAll(dlg);
				dlg->setText(text);
				dlg->setUrl(link);
				dlg->setVersion(version);
				dlg->exec();
				delete dlg;

            }
            break;
        }
        n = n.nextSibling();
    }
    this->deleteLater();

}
void UpdatesChecker::updatesAvailable()
{

}
