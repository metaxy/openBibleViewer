#ifndef UPDATECHECKERDIALOG_H
#define UPDATECHECKERDIALOG_H

#include <QDialog>
#include "src/core/basicclass.h"

namespace Ui {
    class UpdateCheckerDialog;
}

class UpdateCheckerDialog : public QDialog, public BasicClass
{
    Q_OBJECT

public:
    explicit UpdateCheckerDialog(QWidget *parent = 0);
    ~UpdateCheckerDialog();

    void setText(const QString &text);
    void setUrl(const QString &url);
    void setVersion(const QString &version);

private slots:
    void skip();
    void later();
    void download();
private:
    Ui::UpdateCheckerDialog *ui;
    QString m_url;
    QString m_version;
};

#endif // UPDATECHECKERDIALOG_H
