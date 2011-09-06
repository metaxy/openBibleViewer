#include "updatecheckerdialog.h"
#include "ui_updatecheckerdialog.h"
#include <QtGui/QDesktopServices>
#include <QtCore/QUrl>

UpdateCheckerDialog::UpdateCheckerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateCheckerDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton_later, SIGNAL(clicked()), this, SLOT(later()));
    connect(ui->pushButton_skip, SIGNAL(clicked()), this, SLOT(skip()));
    connect(ui->pushButton_download, SIGNAL(clicked()), this, SLOT(download()));
}

UpdateCheckerDialog::~UpdateCheckerDialog()
{
    delete ui;
}
void UpdateCheckerDialog::setText(const QString &text)
{
    ui->label->setText(text);
}
void UpdateCheckerDialog::setUrl(const QString &url)
{
    m_url = url;
}
void UpdateCheckerDialog::setVersion(const QString &version)
{
    m_version = version;
}

void UpdateCheckerDialog::skip()
{
    m_settings->session.file()->setValue(m_settings->session.id()+ "/" + "skipVersion", m_version);
    close();
}

void UpdateCheckerDialog::later()
{
    close();
}

void UpdateCheckerDialog::download()
{
    QDesktopServices::openUrl(QUrl(m_url));
    close();
}
