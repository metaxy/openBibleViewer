/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger <metaxy@walger.name>
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
    explicit DownloadInFile(QObject *parent = 0, QNetworkAccessManager *manager = 0);

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
