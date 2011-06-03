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

/**
  Conctruct a ModuleDownloader. Need a parent widget. And a list with links e.g http://example.com/a.xml
and the names for it e.g names['http://example.com/a.xml'] = "A Book".
  */
ModuleDownloader::ModuleDownloader(QMap<QString, QString> data)
{
    m_data = data;
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
}
/**
  Starts the download. If finished the signal downloaded ist emited.
  */
void ModuleDownloader::start()
{
    //create folder where the modules should be downloaded
    QDir dir(m_settings->homePath);
    dir.mkdir(m_settings->homePath + "modules");

    //hack: remove duplicates
   /* const QSet<QString> set = m_data.keys().toSet();
    m_urls = set.toList();
    m_counter = 0;*/

    //downloadNext();
}
/*void ModuleDownloader::downloadNext()
{

    if(m_counter + 1 == m_urls.size() && m_urls.size() != 0) {
        emit downloaded(m_downloadedList, m_downNames);
        return;
    }
    if(m_counter < m_urls.size() && m_urls.size() != 0) {
        m_counter++;
        download(m_urls.at(m_currentDownload));
    }
}*/
void ModuleDownloader::download(QString url_, bool addToList)
{
   /* DEBUG_FUNC_NAME
    const QUrl url(url_);
    QFileInfo fileInfo(url.path());
    QDir d(m_settings->homePath + "modules/");
    QDir dir(m_settings->homePath);
    dir.mkpath(m_settings->homePath + "modules/" + fileInfo.fileName() + "/");
    const QString fileName = m_settings->homePath + "modules/" + fileInfo.fileName() + "/" + fileInfo.fileName();
    if(addToList) {
        m_downloadedList << fileName;
        m_downNames << m_names[m_urls.at(m_currentDownload)];
    }
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

