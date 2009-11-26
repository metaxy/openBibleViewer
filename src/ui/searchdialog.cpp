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
#include  "searchdialog.h"
#include  "ui_searchdialog.h"

searchDialog::searchDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::searchDialog)
{
    m_ui->setupUi(this);
    connect(m_ui->pushButton, SIGNAL(clicked()), this, SLOT(search()));
    connect(m_ui->pushButton_more, SIGNAL(clicked()), this, SLOT(showMore()));
    m_ui->frame->hide();
}

searchDialog::~searchDialog()
{
    delete m_ui;
}
void searchDialog::showMore(void)
{
    m_ui->frame->show();
}
void searchDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
int searchDialog::setText(QString text)
{
    m_ui->lineEdit->setText(text);
    return 0;
}
int searchDialog::search(void)
{
    if (m_ui->lineEdit->text() != "") {
        struct searchQuery query;
        query.text = m_ui->lineEdit->text();
        query.regexp = m_ui->checkBox_regexp->isChecked();
        query.whole = m_ui->checkBox_whole->isChecked();
        query.caseSensitive = m_ui->checkBox_casesen->isChecked();
        emit searched(query);
        this->hide();
        this->close();
    }
    return 0;
}
