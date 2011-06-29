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
}

void DownloadProgressDialog::start()
{
    m_downloader->start();
    connect(m_downloader, SIGNAL(updateProgress(qint64,qint64)), this, SLOT(updateProgress(qint64,qint64)));
}
void DownloadProgressDialog::updateProgress(qint64 current, qint64 total)
{
    if(ui->progressBar->maximum() != total)
        ui->progressBar->setMaximum(total);

    ui->progressBar->setValue(current);
    myDebug() << current << total;
}
