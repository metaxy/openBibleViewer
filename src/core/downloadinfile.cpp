#include "downloadinfile.h"
#include <QtCore/QDir>
#include "src/core/dbghelper.h"
DownloadInFile::DownloadInFile(QObject *parent, QNetworkAccessManager *manager) :
    QObject(parent)
{
    m_manager = manager;
    m_file = NULL;
}
void DownloadInFile::setUrl(const QString &url)
{
    QUrl u(url);
    m_url = u;
}

void DownloadInFile::setFileName(const QString &fileName)
{
    myDebug() << "fileName = " << fileName;
    m_fileName = fileName;
}

void DownloadInFile::setFolder(const QString &folder)
{
    m_path = folder;
}
void DownloadInFile::download()
{
    DEBUG_FUNC_NAME

    QDir dir(QDir::homePath());
    dir.mkpath(m_path);
    const QString full = m_path + QDir::separator() + m_fileName;
    m_file = new QFile(full);

    if (!m_file->open(QIODevice::WriteOnly)) {
        myWarning() << "could not open file" << m_file->errorString();
        myDebug() << "full = " << full;
        return;
    }
    QNetworkRequest request(m_url);
    m_reply = m_manager->get(request);

    connect(m_reply, SIGNAL(readyRead()), this, SLOT(read()));
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SIGNAL(progress(qint64,qint64)));
    connect(m_reply, SIGNAL(finished()), this, SLOT(finish()));

}
void DownloadInFile::read()
{
    m_file->write(m_reply->readAll());
}
void DownloadInFile::finish()
{
    DEBUG_FUNC_NAME;
    m_file->close();
    delete m_file;

    int status = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    myDebug() << "status = " << status;
    if (status == 302 || status == 301) { // redirected
        m_url = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        download();
        return;
    } else {
        emit finished();
    }

    m_reply->deleteLater();
}
