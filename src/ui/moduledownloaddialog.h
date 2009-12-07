/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
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
#ifndef MODULEDOWNLOADDIALOG_H
#define MODULEDOWNLOADDIALOG_H

#include <QDialog>
#include <QtCore/QStringList>
#include <QtGui/QTreeWidgetItem>
#include <QtNetwork/QHttp>
#include <QtCore/QFile>
#include <QtGui/QProgressDialog>
#include "../core/settings.h"
namespace Ui
{
class moduleDownloadDialog;
}

class moduleDownloadDialog : public QDialog
{
    Q_OBJECT

signals:
    void downloaded(QStringList downloadedFiles, QStringList names);

private slots:
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
    void updateDataReadProgress(int bytesRead, int totalBytes);
    void downloadNext();
    void item(QTreeWidgetItem* item);
    void cancelDownload();
public:
    moduleDownloadDialog(QWidget *parent = 0);
    ~moduleDownloadDialog();
    void setSettings(Settings settings);
    void readModules();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::moduleDownloadDialog *ui;
    Settings m_set;
    QStringList downloadList, nameList, downloadedList, downNames;
    int currentDownload;
    QProgressDialog *progressDialog;
    QHttp *http;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;
};

#endif // MODULEDOWNLOADDIALOG_H
