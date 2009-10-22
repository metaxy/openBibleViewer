#include "moduledownloaddialog.h"
#include "ui_moduledownloaddialog.h"
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QFile>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QStyle>
#include <QtGui/QIcon>
#include <QtCore/QList>
#include <QtCore/QDir>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QMessageBox>
#include <QtDebug>
moduleDownloadDialog::moduleDownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::moduleDownloadDialog)
{
    ui->setupUi(this);
     progressDialog = new QProgressDialog(this);

     http = new QHttp(this);
    connect(ui->pushButton_download, SIGNAL(clicked()), this, SLOT(downloadNext()));
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(item(QTreeWidgetItem*,int)));
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
void moduleDownloadDialog::readModules()
{
    qDebug() << "moduleDownloadDialog::readModules()";
     //load list from file
    //show it in treewidget
    QDomDocument doc;
    QFile file(m_set.homePath+"modules.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "moduleDownloadDialog::readModules() cant read the file";
        return;
    }
    if (!doc.setContent(&file)) {
        qDebug() << "moduleDownloadDialog::readModules() the file isnt valid";
        return;
    }
    QMap<QString,QStringList> map;
    QMap<QString,QStringList> map2;
    QDomNode item = doc.documentElement().firstChild().firstChild();
    for (; !item.isNull();) {
        qDebug() << "moduleDownloadDialog::readModules() throw items";
        QDomElement e = item.toElement();
        map[e.attribute("lang","unkown")].append(e.firstChild().toText().data());
        map2[e.attribute("lang","unkown")].append(e.attribute("link",""));
        item = item.nextSibling();
    }

    QList<QTreeWidgetItem *> items;
    QTreeWidgetItem *top = new QTreeWidgetItem(ui->treeWidget);
    QStyle *style = ui->treeWidget->style();
    QIcon folderIcon;
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);
    top->setIcon(0, folderIcon);
    top->setText(0, "Zefania Modules");
    items.append(top);

    QMapIterator<QString, QStringList> i(map);
    while (i.hasNext()) {
        i.next();
        if(i.value().size() > 0)
        {
            QTreeWidgetItem *langItem = new QTreeWidgetItem();
            langItem->setText(0, i.key());
            langItem->setCheckState(0,Qt::Unchecked);
            langItem->setData(1,0,"lang");

            QIcon langIcon;
            langIcon.addPixmap(QPixmap(":/icons/16x16/text-xml.png"), QIcon::Normal, QIcon::Off);
            langItem->setIcon(0, langIcon);

            top->addChild(langItem);
            for(int a = 0; a < i.value().size();a++)
            {
                QTreeWidgetItem *bibleItem = new QTreeWidgetItem();
                bibleItem->setData(1,0,"bible");
                bibleItem->setData(2,0,map2[i.key()].at(a));
                bibleItem->setData(2,1,map[i.key()].at(a));
                bibleItem->setCheckState(0,Qt::Unchecked);
                bibleItem->setText(0, i.value().at(a));
                QIcon bibleIcon;
                bibleIcon.addPixmap(QPixmap(":/icons/16x16/text-xml.png"), QIcon::Normal, QIcon::Off);
                bibleItem->setIcon(0, bibleIcon);
                langItem->addChild(bibleItem);
            }
        }
    }
}
void moduleDownloadDialog::item(QTreeWidgetItem* item,int c)
{
    qDebug() << "moduleDownloadDialog::item item = " << item->data(1,0) << " downloadList = " << downloadList;
    if(item->data(1,0) == "lang")
    {
        if(item->checkState(0) == Qt::Checked)
        {
            //check also all childrem of this
        }
        else if(item->checkState(0) == Qt::Unchecked)
        {
            //uncheck all children
        }

        //lang item
    }
    else if(item->data(1,0) == "bible")
    {

        if(item->checkState(0) == Qt::Checked)
        {
            downloadList.append(item->data(2,0).toString());
            nameList.append(item->data(2,1).toString());
            // add to download list
        }
        else if(item->checkState(0) == Qt::Unchecked)
        {
             downloadList.removeOne(item->data(2,0).toString());
             nameList.removeOne(item->data(2,1).toString());
            //remove from download list
        }
    }
    //if(item->checkState(0) == Qt::
}
void moduleDownloadDialog::setSettings(settings_s settings)
{
    m_set = settings;
}
void moduleDownloadDialog::downloadNext()
{
    if(httpRequestAborted)
        return;
    qDebug() << "moduleDownloadDialog::downloadNext() currentDownload = " << currentDownload << " size = " << downloadList.size();
    QDir dir(m_set.homePath);
    dir.mkdir(m_set.homePath+"modules");
    if(currentDownload +1 == downloadList.size() && downloadList.size() != 0)
    {
        qDebug() << "moduleDownloadDialog::downloadNext() emit";
        emit downloaded(downloadedList,downNames);
        return;
    }
    if(currentDownload < downloadList.size() && downloadList.size() != 0)
    {
        /*delete
        progressDialog = new QProgressDialog(this);
        http = new QHttp(this);*/

        currentDownload++;
        QUrl url(downloadList.at(currentDownload));
        QFileInfo fileInfo(url.path());
        QString fileName = m_set.homePath+"modules/"+fileInfo.fileName();
        downloadedList << fileName;
        downNames << nameList.at(currentDownload);
        qDebug() << "moduleDownloadDialog::downloadNext() fileName = " << fileName;

        if (QFile::exists(fileName))
        {
            QFile::remove(fileName);
        }
        qDebug() << "moduleDownloadDialog::downloadNext() 2";
        file = new QFile(fileName);
        if (!file->open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, tr("HTTP"),
                                  tr("Unable to save the file %1: %2.")
                                  .arg(fileName).arg(file->errorString()));
            delete file;
            file = 0;
            return;
        }
        qDebug() << "moduleDownloadDialog::downloadNext() 3";
        QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
        http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());
        /* if (!url.userName().isEmpty())
         http->setUser(url.userName(), url.password());*/
        httpRequestAborted = false;
        QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
        if (path.isEmpty())
            path = "/";
        httpGetId = http->get(path, file);
        qDebug() << "moduleDownloadDialog::downloadNext() 4";
        progressDialog->setWindowTitle(tr("HTTP"));
        progressDialog->setLabelText(tr("Downloading %1.").arg(nameList.at(currentDownload)));
        qDebug() << "moduleDownloadDialog::downloadNext() 5";
    }
    else
    {
        qDebug() << "moduleDownloadDialog::downloadNext() nothing selected";
    }
    //do downloading
}
void moduleDownloadDialog::cancelDownload()
 {
     //statusLabel->setText(tr("Download canceled."));
     httpRequestAborted = true;
     http->abort();
     //downloadButton->setEnabled(true);
 }

 void moduleDownloadDialog::httpRequestFinished(int requestId, bool error)
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
     progressDialog->hide();
     file->close();

     if (error) {
         file->remove();
         QMessageBox::information(this, tr("HTTP"),
                                  tr("Download failed: %1.")
                                  .arg(http->errorString()));
     } else {
         qDebug() << "moduleDownloadDialog::httpRequestFinished() Downloaded";
        file->close();
       //  QString fileName = QFileInfo(QUrl(urlLineEdit->text()).path()).fileName();
         //statusLabel->setText(tr("Downloaded %1 to current directory.")/*.arg(fileName)*/);
     }

     //downloadButton->setEnabled(true);
     delete file;
     file = 0;
     downloadNext();
 }

 void moduleDownloadDialog::readResponseHeader(const QHttpResponseHeader &responseHeader)
 {
     switch (responseHeader.statusCode()) {
     case 200:                   // Ok
     case 301:                   // Moved Permanently
     case 302:                   // Found
     case 303:                   // See Other
     case 307:                   // Temporary Redirect
         // these are not error conditions
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

 void moduleDownloadDialog::updateDataReadProgress(int bytesRead, int totalBytes)
 {
     if (httpRequestAborted)
         return;
    qDebug() << "moduleDownloadDialog::updateDataReadProgress() bytesRead = " << bytesRead;
    if(totalBytes == 0) {
         if(progressDialog->maximum() != totalBytes) {
        progressDialog->setMaximum(totalBytes);
        progressDialog->setMinimum(totalBytes);
    }
    progressDialog->setValue(bytesRead);
    } else {
         if(progressDialog->maximum() != totalBytes) {
        progressDialog->setMaximum(totalBytes);
    }
    progressDialog->setValue(bytesRead);
    }
 }

moduleDownloadDialog::~moduleDownloadDialog()
{
    delete ui;
}

void moduleDownloadDialog::changeEvent(QEvent *e)
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
