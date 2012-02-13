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
#include "downloadinfile.h"
#include <QtCore/QDir>
#include "src/core/dbghelper.h"
DownloadInFile::DownloadInFile(QObject *parent, QNetworkAccessManager *manager) :
    QObject(parent)
{
    m_manager = manager;
    m_file = NULL;
    m_reply = NULL;
}

void DownloadInFile::setUrl(const QUrl &url)
{
    m_url = url;
}

void DownloadInFile::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

void DownloadInFile::setFolder(const QString &folder)
{
    m_path = folder;
}

void DownloadInFile::setName(const QString &name)
{
    m_name = name;
}

void DownloadInFile::download()
{
    QDir dir(QDir::homePath());
    dir.mkpath(m_path);
    //m_path has seperator
    m_localUrl = m_path + m_fileName;
    m_file = new QFile(m_localUrl);

    if(!m_file->open(QIODevice::WriteOnly)) {
        myWarning() << "could not open file" << m_file->errorString();
        return;
    }

    QNetworkRequest request(m_url);

    //otherwise it will send a redirection link for browsers
    request.setRawHeader("User-Agent", "curl/7.21.2");
    request.setRawHeader("Accept", "*/*");
    m_reply = m_manager->get(request);

    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(progress(qint64, qint64)));
    connect(m_reply, SIGNAL(finished()), this, SLOT(finish()));

}
void DownloadInFile::finish()
{
    int status = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(status == 302 || status == 301) {  // redirected
        m_url = m_reply->header(QNetworkRequest::LocationHeader).toUrl();
        m_reply->deleteLater();
        m_file->close();
        delete m_file;
        download();
        return;
    } else {
        m_file->write(m_reply->readAll());
        m_file->close();
        delete m_file;
        emit finished(m_localUrl, m_name, status);
    }
    m_reply->deleteLater();
}
