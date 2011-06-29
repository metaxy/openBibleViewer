#ifndef DOWNLOADPROGRESSDIALOG_H
#define DOWNLOADPROGRESSDIALOG_H

#include <QDialog>
#include "src/core/moduledownloader.h"
namespace Ui {
    class DownloadProgressDialog;
}

class DownloadProgressDialog : public QDialog
{
    Q_OBJECT
private slots:
    void updateProgress(qint64 current,qint64 total);
public:
    explicit DownloadProgressDialog(QWidget *parent = 0);
    ~DownloadProgressDialog();
    void setModuleDownloader(ModuleDownloader *downloader);
    void start();

private:
    Ui::DownloadProgressDialog *ui;
    ModuleDownloader *m_downloader;
};

#endif // DOWNLOADPROGRESSDIALOG_H
