/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include "moduledownloader.h"
#include "src/core/dbghelper.h"
#include <QtCore/QDir>
#include <QtGui/QMessageBox>
#include <QtCore/QUrl>
#include <QtCore/QFileInfo>
#include <QtNetwork/QNetworkReply>
#include "src/core/downloadinfile.h"

/**
  Conctruct a ModuleDownloader. Need a parent widget. And a list with links e.g http://example.com/a.xml
and the names for it e.g names['http://example.com/a.xml'] = "A Book".
  */
ModuleDownloader::ModuleDownloader(QObject *parent, QMap<QString, QString> data) :
    QObject(parent)
{
    m_data = data;
    m_manager = new QNetworkAccessManager(this);
    m_counter = 0;
}
/**
  Starts the download. If finished the signal downloaded ist emited.
  */
void ModuleDownloader::start()
{
    DEBUG_FUNC_NAME
    //create folder where the modules should be downloaded
    QDir dir(m_settings->homePath);
    dir.mkdir(m_settings->homePath + "modules");

    //hack: remove duplicates
    const QSet<QString> set = m_data.keys().toSet();
    m_urls = set.toList();
    m_counter = 0;
    myDebug() << m_urls;

    downloadNext();
}
void ModuleDownloader::downloadNext()
{
    DEBUG_FUNC_NAME
    myDebug() << "m_counter = " << m_counter;

    if(m_counter >= m_urls.size() && m_urls.size() != 0) {
        //finished all
        myDebug() << "finished!!!";
        emit downloaded(m_retData);
        return;
    }
    if(m_counter < m_urls.size() && m_urls.size() != 0) {
        myDebug() << "download next";
        //download next
        download(m_urls.at(m_counter));
        m_counter++;
    }
}
void ModuleDownloader::save(QString url, QString name, int status)
{
    m_retData[url] = name;
    downloadNext();
}

void ModuleDownloader::download(const QString &url_)
{
    DEBUG_FUNC_NAME;
    myDebug() << url_;

    const QUrl url(url_);

    QFileInfo fileInfo(url.path());
    DownloadInFile *d = new DownloadInFile(this, m_manager);
    d->setUrl(url);
    const QString folder = m_settings->homePath + "modules" + QDir::separator() + fileInfo.fileName()  + QDir::separator();
    d->setFileName(fileInfo.fileName());
    d->setFolder(folder);
    d->setName(m_data[url_]);

    d->download();
    connect(d, SIGNAL(finished(QString, QString, int)), this, SLOT(save(QString, QString, int)));
    connect(d, SIGNAL(progress(qint64,qint64)), this, SIGNAL(updateProgress(qint64,qint64)));


   /* DEBUG_FUNC_NAME
    const QUrl url(url_);
    QFileInfo fileInfo(url.path());
    QDir d(m_settings->homePath + "modules/");
    QDir dir(m_settings->homePath);
    dir.mkpath(m_settings->homePath + "modules/" + fileInfo.fileName() + "/");
    const QString fileName = m_settings->homePath + "modules/" + fileInfo.fileName() + "/" + fileInfo.fileName();

    if(QFile::exists(fileName)) {
        QFile::remove(fileName);
    }
    m_file = new QFile(fileName);
    if(!m_file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(m_parent, tr("HTTP"), tr("Unable to save the file %1: %2.").arg(fileName).arg(m_file->errorString()));
        delete m_file;
        m_file = 0;
        return;
    }
    m_httpRequestAborted = false;
    m_http->setHost(url.host(), QHttp::ConnectionModeHttp, url.port() == -1 ? 0 : url.port());
    m_httpGetId = m_http->get(url_, m_file);
    m_progressDialog->setWindowTitle(tr("Downloading"));
    m_progressDialog->setLabelText(tr("Downloading %1 / %2. %3 MB").arg(m_currentDownload + 1).arg(m_urls.size()).arg(0));
    m_progressDialog->setModal(true);*/

}

