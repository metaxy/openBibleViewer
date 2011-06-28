#ifndef DOWNLOADINFILE_H
#define DOWNLOADINFILE_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtCore/QFile>
#include <QtNetwork/QNetworkReply>

class DownloadInFile : public QObject
{
    Q_OBJECT
public:
    explicit DownloadInFile(QObject *parent = 0, QNetworkAccessManager *manager=0);
    void setUrl(const QString &url);
    void setFileName(const QString &fileName);
    void setFolder(const QString &folder);
signals:
    void progress(qint64 bytesReceived, qint64 bytesTotal);
    void finished();
public slots:
    void download();
private slots:
    void read();
    void finish();
private:
    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
    QFile *m_file;
    bool m_inFile;
    QString m_path;
    QString m_fileName;
    QUrl m_url;
};

#endif // DOWNLOADINFILE_H
