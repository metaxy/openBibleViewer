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

    void setUrl(const QUrl &url);
    void setFileName(const QString &fileName);
    void setFolder(const QString &folder);
    void setName(const QString &name);
signals:
    void progress(qint64 bytesReceived, qint64 bytesTotal);
    void finished(QString localUrl, QString name, int status);
public slots:
    void download();
private slots:
    void finish();
private:
    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
    QFile *m_file;
    bool m_inFile;
    QString m_path;
    QString m_fileName;
    QUrl m_url;

    QString m_localUrl;
    QString m_name;
};

#endif // DOWNLOADINFILE_H
