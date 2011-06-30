#include "downloadprogressdialog.h"
#include "ui_downloadprogressdialog.h"
#include "src/core/dbghelper.h"
DownloadProgressDialog::DownloadProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadProgressDialog)
{
    ui->setupUi(this);
}

DownloadProgressDialog::~DownloadProgressDialog()
{
    delete ui;
}

void DownloadProgressDialog::setModuleDownloader(ModuleDownloader *downloader)
{
   m_downloader = downloader;
   connect(m_downloader, SIGNAL(downloaded(QMap<QString,QString>)), this, SLOT(close()));
   connect(m_downloader, SIGNAL(nextFile(int,int)), this, SLOT(updateProgress2(int,int)));
}

void DownloadProgressDialog::start()
{
    connect(m_downloader, SIGNAL(updateProgress(qint64,qint64)), this, SLOT(updateProgress(qint64,qint64)));

    m_fileCount = m_downloader->start();
    ui->progressBar_2->setMaximum(m_fileCount*100);

}

void DownloadProgressDialog::updateProgress(qint64 current, qint64 total)
{
    if(ui->progressBar->maximum() != total)
        ui->progressBar->setMaximum(total);

    ui->progressBar->setValue(current);
    //myDebug() << current << total;
}

void DownloadProgressDialog::updateProgress2(int current,int total)
{
    //m_current = current;
    ui->progressBar_2->setValue(current*100);
}
