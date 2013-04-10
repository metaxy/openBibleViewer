/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
#include "updatecheckerdialog.h"
#include "ui_updatecheckerdialog.h"
#include <QDesktopServices>
#include <QUrl>

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
    m_settings->session.file()->setValue(m_settings->session.id() + "/" + "skipVersion", m_version);
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
