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
#include <QtCore/QMap>
#include <QtGui/QTreeWidgetItem>
#include <QtNetwork/QHttp>
#include <QtCore/QFile>
#include <QtGui/QProgressDialog>
#include "src/core/settings.h"

namespace Ui
{
class ModuleDownloadDialog;
}
/*!
 ModuleDownloadDialog represents a dialog to download new modules

*/
class ModuleDownloadDialog : public QDialog
{
    Q_OBJECT

signals:
    void downloaded(QStringList downloadedFiles, QStringList names);

private slots:
    void item(QTreeWidgetItem* item);
    void download();
public:
    ModuleDownloadDialog(QWidget *parent = 0);
    ~ModuleDownloadDialog();
    void setSettings(Settings settings);
    void readModules();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ModuleDownloadDialog *ui;
    Settings m_set;

    QStringList m_downloadList;
    QMap<QString, QString> m_names;
};

#endif // MODULEDOWNLOADDIALOG_H
