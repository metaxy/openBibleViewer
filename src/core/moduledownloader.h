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
#ifndef MODULEDOWNLOADER_H
#define MODULEDOWNLOADER_H
#include <QtNetwork/QNetworkAccessManager>
#include "src/core/basicclass.h"

class ModuleDownloader : public QObject, public BasicClass
{
    Q_OBJECT
signals:
    void downloaded(QMap<QString, QString> data);
    void updateProgress(qint64 current, qint64 total);
    void updateLog(QString text);
    void updateTitle(QString title);
    void nextFile(int fileCount, int maxFiles);
public:
    //QMap<Url,Title>
    ModuleDownloader(QObject *parent , const QMap<QString, QString> &data);
    int start();
private slots:

    void save(QString url, QString name, int status);
private:
    void downloadNext();
    QMap<QString, QString> m_data;
    int m_counter;
    QNetworkAccessManager *m_manager;
    QStringList m_urls;
    QStringList m_downloadedList;
    QStringList m_downNames;
    void download(const QString &url);
    QMap<QString, QString> m_retData;

    int m_fileCount;

    QString m_prefix;
};

#endif // MODULEDOWNLOADER_H
