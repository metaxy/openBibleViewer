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
#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QDesktopServices>
#include <QUrl>
AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AboutDialog)
{
    m_ui->setupUi(this);
    connect(m_ui->buttonBox, SIGNAL(accepted()), this, SLOT(close()));
    connect(m_ui->label, SIGNAL(linkActivated(QString)), this, SLOT(openLink(QString)));
}

AboutDialog::~AboutDialog()
{
    delete m_ui;
}
void AboutDialog::setText(const QString &text)
{
    m_ui->label->setText(text);
}
void AboutDialog::openLink(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link, QUrl::StrictMode));
}

void AboutDialog::changeEvent(QEvent *e)
{
    switch(e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

