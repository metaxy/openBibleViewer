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
#ifndef MODULEDOWNLOADER_H
#define MODULEDOWNLOADER_H
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtNetwork/QHttp>
#include <QtCore/QFile>
#include <QtGui/QProgressDialog>
#include "src/core/settings/settings.h"

class ModuleDownloader : public QObject
{
    Q_OBJECT
signals:
    void downloaded(QStringList downloadedFiles, QStringList names);
public:
    ModuleDownloader(QWidget *parent, QStringList urls, QMap<QString, QString> names);
    void setSettings(Settings *settings);
    void start();

private slots:
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
    void updateDataReadProgress(int bytesRead, int totalBytes);
    void downloadNext();
    void cancelDownload();
private:
    QWidget *m_parent;
    QStringList m_urls;
    QMap<QString, QString> m_names;

    Settings *m_settings;

    int m_currentDownload;
    QProgressDialog *m_progressDialog;
    QHttp *m_http;
    QFile *m_file;
    int m_httpGetId;
    bool m_httpRequestAborted;
    QStringList m_downloadedList, m_downNames;

    void download(QString url, bool addToList = true);
};

#endif // MODULEDOWNLOADER_H
