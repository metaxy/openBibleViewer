#ifndef MODULEDOWNLOADDIALOG_H
#define MODULEDOWNLOADDIALOG_H

#include <QDialog>
#include <QtCore/QStringList>
#include <QtGui/QTreeWidgetItem>
#include <QtNetwork/QHttp>
#include <QtCore/QFile>
#include <QtGui/QProgressDialog>
#include "../core/config.h"
namespace Ui {
    class moduleDownloadDialog;
}

class moduleDownloadDialog : public QDialog {
    Q_OBJECT

signals:
    void downloaded(QStringList downloadedFiles,QStringList names);

private slots:
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
    void updateDataReadProgress(int bytesRead, int totalBytes);
    void downloadNext();
    void item(QTreeWidgetItem* item,int c);
    void cancelDownload();
public:
    moduleDownloadDialog(QWidget *parent = 0);
    ~moduleDownloadDialog();
    void setSettings(settings_s settings);
    void readModules();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::moduleDownloadDialog *ui;
    struct settings_s m_set;
    QStringList downloadList,nameList,downloadedList,downNames;
    int currentDownload;
    QProgressDialog *progressDialog;
    QHttp *http;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;
};

#endif // MODULEDOWNLOADDIALOG_H
