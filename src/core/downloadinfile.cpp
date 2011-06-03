#include "downloadinfile.h"
#include <QtCore/QDir>
DownloadInFile::DownloadInFile(QObject *parent, QNetworkAccessManager *manager) :
    QObject(parent)
{
    m_manager = manager;
    m_file = NULL;
}
void DownloadInFile::setUrl(const QString &url)
{
    m_url = url;
}

void DownloadInFile::setFileName(const QString &fileName)
{
    m_inFile = true;
    m_path = fileName;
}

void DownloadInFile::setFolder(const QString &folder)
{
    m_inFile = false;
    m_path = folder;
}
void DownloadInFile::download()
{
    QUrl url(m_url);
    QNetworkRequest request;
    request.setUrl(url);

    if(m_inFile) {
        m_file = new QFile(m_path);
    } else {
        QFileInfo info(m_url);
        m_file = new QFile(m_path+QDir::separator()+info.fileName());
    }
    if (!m_file->open(QIODevice::WriteOnly | QIODevice::Text))
             return;

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(read));
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SIGNAL(progress(qint64,qint64)));
    connect(m_reply, SIGNAL(finished()), this, SLOT(finish()));

}
void DownloadInFile::read()
{
    m_file->write(m_reply->read(m_reply->readBufferSize()));
}
void DownloadInFile::finish()
{
    m_file->close();
    delete m_file;

    m_reply->deleteLater();
}
