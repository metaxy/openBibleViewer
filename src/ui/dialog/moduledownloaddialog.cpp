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
#include "moduledownloaddialog.h"
#include "ui_moduledownloaddialog.h"
#include "src/core/dbghelper.h"
#include <QtCore/QList>
#include <QtCore/QDir>
#include <QtCore/QUrl>
#include <QtCore/QFile>
#include <QtGui/QMessageBox>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QStyle>
#include <QtGui/QIcon>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>


ModuleDownloadDialog::ModuleDownloadDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::ModuleDownloadDialog)
{
    ui->setupUi(this);
    progressDialog = new QProgressDialog(this);

    http = new QHttp(this);
    connect(ui->pushButton_download, SIGNAL(clicked()), this, SLOT(downloadNext()));//set httpRequestAborted = false
    connect(ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(item(QTreeWidgetItem*)));
    connect(http, SIGNAL(requestFinished(int, bool)),
            this, SLOT(httpRequestFinished(int, bool)));
    connect(http, SIGNAL(dataReadProgress(int, int)),
            this, SLOT(updateDataReadProgress(int, int)));
    connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
            this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
    connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelDownload()));
    currentDownload = -1;
    httpRequestAborted = false;

}
/**
  Read the xml file withe downloadable modules and generate the tree.
  */
void ModuleDownloadDialog::readModules()
{
    //DEBUG_FUNC_NAME
    QDomDocument doc;
    QFile file(":/data/modules.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        myDebug() << "cant read the file";
        return;
    }
    if (!doc.setContent(&file)) {
        myDebug() << "the file isnt valid";
        return;
    }
    QList<QTreeWidgetItem *> items;

    QDomNode id = doc.documentElement().firstChild();
    for (; !id.isNull();) {
        QMap<QString, QStringList> map2;
        QMap<QString, QStringList> map;
        QDomNode item = id.firstChild();
        for (; !item.isNull();) {
            QDomElement e = item.toElement();
            map[e.attribute("lang", "unkown")].append(e.firstChild().toText().data());
            map2[e.attribute("lang", "unkown")].append(e.attribute("link", ""));
            item = item.nextSibling();
        }
        QTreeWidgetItem *top = new QTreeWidgetItem(ui->treeWidget);

        QStyle *style = ui->treeWidget->style();
        QIcon folderIcon;
        folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
        folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);
        top->setIcon(0, folderIcon);
        QDomElement e = id.toElement();
        QString typ = e.attribute("typ", "bible");
        if (typ == "bible") {
            top->setText(0, tr("Bibles"));
        } else if (typ == "strong") {
            top->setText(0, tr("Strong Modules"));
        }
        items.append(top);

        QMapIterator<QString, QStringList> i(map);
        while (i.hasNext()) {
            i.next();
            if (i.value().size() > 0) {
                QTreeWidgetItem *langItem = new QTreeWidgetItem();
                langItem->setText(0, i.key());
                langItem->setCheckState(0, Qt::Unchecked);
                langItem->setData(1, 0, "lang");
                top->addChild(langItem);
                for (int a = 0; a < i.value().size(); a++) {
                    QTreeWidgetItem *bibleItem = new QTreeWidgetItem();
                    bibleItem->setData(1, 0, "module");
                    bibleItem->setData(2, 0, map2[i.key()].at(a));
                    bibleItem->setData(2, 1, map[i.key()].at(a));
                    bibleItem->setCheckState(0, Qt::Unchecked);
                    bibleItem->setText(0, i.value().at(a));
                    langItem->addChild(bibleItem);
                }
            }
        }
        top->setExpanded(true);
        id = id.nextSibling();
    }
}
void ModuleDownloadDialog::item(QTreeWidgetItem* i)
{
    //DEBUG_FUNC_NAME
    myDebug() << "i = " << i->data(1, 0) << " downloadList = " << downloadList;
    httpRequestAborted = false;
    if (i->data(1, 0) == "lang") {
        if (i->checkState(0) == Qt::Checked) {
            for (int c = 0; c < i->childCount(); c++) {
                i->child(c)->setCheckState(0, Qt::Checked);
                item(i->child(c));
            }
            //check also all childrem of this
        } else if (i->checkState(0) == Qt::Unchecked) {
            for (int c = 0; c < i->childCount(); c++) {
                i->child(c)->setCheckState(0, Qt::Unchecked);
                item(i->child(c));
            }
            //uncheck all children
        }
    } else if (i->data(1, 0) == "module") {

        if (i->checkState(0) == Qt::Checked) {
            downloadList.append(i->data(2, 0).toString());
            nameList.append(i->data(2, 1).toString());
            // add to download list
        } else if (i->checkState(0) == Qt::Unchecked) {
            downloadList.removeOne(i->data(2, 0).toString());
            nameList.removeOne(i->data(2, 1).toString());
        }
    }
}
void ModuleDownloadDialog::setSettings(Settings settings)
{
    m_set = settings;
}
void ModuleDownloadDialog::downloadNext()
{
    if (httpRequestAborted)
        return;
    myDebug() << "currentDownload = " << currentDownload << " size = " << downloadList.size();
    QDir dir(m_set.homePath);
    dir.mkdir(m_set.homePath + "modules");
    if (currentDownload + 1 == downloadList.size() && downloadList.size() != 0) {
        emit downloaded(downloadedList, downNames);
        return;
    }
    if (currentDownload < downloadList.size() && downloadList.size() != 0) {
        currentDownload++;
        download(downloadList.at(currentDownload));

    } else {
        myDebug() << "nothing selected";
        QMessageBox::information(this, "Nothing selected", "You have to selected something to download it.");
    }
}
void ModuleDownloadDialog::download(QString url_, bool addToList)
{
    DEBUG_FUNC_NAME
    myDebug() << url_;
    QUrl url(url_);
    QFileInfo fileInfo(url.path());
    QDir d(m_set.homePath + "modules/");
    QDir dir(m_set.homePath);
    dir.mkpath(m_set.homePath + "modules/" + fileInfo.fileName() + "/");
    QString fileName = m_set.homePath + "modules/" + fileInfo.fileName() + "/" + fileInfo.fileName();
    if (addToList) {
        downloadedList << fileName;
        downNames << nameList.at(currentDownload);
    }


    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }
    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("HTTP"), tr("Unable to save the file %1: %2.").arg(fileName).arg(file->errorString()));
        delete file;
        file = 0;
        return;
    }
    httpRequestAborted = false;
    http->setHost(url.host(), QHttp::ConnectionModeHttp, url.port() == -1 ? 0 : url.port());
    httpGetId = http->get(url_, file);
    progressDialog->setWindowTitle(tr("Downloading"));
    progressDialog->setLabelText(tr("Downloading %1 / %2. %3 MB").arg(currentDownload + 1).arg(downloadList.size()).arg(0));
    progressDialog->setModal(true);
}
void ModuleDownloadDialog::cancelDownload()
{
    httpRequestAborted = true;
    http->abort();
}

void ModuleDownloadDialog::httpRequestFinished(int requestId, bool error)
{
    if (requestId != httpGetId)
        return;
    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        progressDialog->hide();
        return;
    }
    if (requestId != httpGetId)
        return;
    if (currentDownload > downloadList.size() - 2 || downloadList.size() == 1) {
        progressDialog->hide();
    }
    file->close();

    if (error) {
        file->remove();
        QMessageBox::information(this, tr("HTTP"), tr("Download failed: %1.").arg(http->errorString()));
    } else {
        qDebug() << "ModuleDownloadDialog::httpRequestFinished() Downloaded";
        file->close();
    }
    delete file;
    file = 0;
    downloadNext();
}

void ModuleDownloadDialog::readResponseHeader(const QHttpResponseHeader &responseHeader)
{
    myDebug() << responseHeader.statusCode() << responseHeader.toString();
    switch (responseHeader.statusCode()) {
    case 200:                   // Ok

        break;
    case 302:                   // Found
    case 303:                   // See Other
    case 307:                   // Temporary Redirect
    case 301:                   // Moved Permanently
        myDebug() << "moved";
        if (responseHeader.hasKey("Location") && !responseHeader.value("Location").contains("failed")) {
            QString location = responseHeader.value("Location");
            download(location, false);
        }
        break;
    default:
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Download failed: %1.")
                                 .arg(responseHeader.reasonPhrase()));
        httpRequestAborted = true;
        progressDialog->hide();
        http->abort();
    }
}

void ModuleDownloadDialog::updateDataReadProgress(int bytesRead, int totalBytes)
{
    if (httpRequestAborted)
        return;
    progressDialog->setLabelText(tr("Downloading %1 / %2. %3 MB").arg(currentDownload + 1).arg(downloadList.size()).arg(QString::number((float)bytesRead / (1024*1024), 'f', 2)));
    if (totalBytes == 0) {
        if (progressDialog->maximum() != 0) {
            progressDialog->setMaximum(0);
            progressDialog->setMinimum(0);
            progressDialog->setValue(1);
        }
    } else {

        if (progressDialog->maximum() != totalBytes) {
            progressDialog->setMaximum(totalBytes);
        }
        progressDialog->setValue(bytesRead);
    }
}

ModuleDownloadDialog::~ModuleDownloadDialog()
{
    delete ui;
}

void ModuleDownloadDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
