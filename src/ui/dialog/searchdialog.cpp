/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SearchDialog)
{
    m_ui->setupUi(this);
    connect(m_ui->pushButton, SIGNAL(clicked()), this, SLOT(search()));
}

SearchDialog::~SearchDialog()
{
    delete m_ui;
}
void SearchDialog::changeEvent(QEvent *e)
{
    switch(e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void SearchDialog::setText(const QString &text)
{
    m_ui->lineEdit->setText(text);
}
void SearchDialog::search(void)
{
    if(m_ui->lineEdit->text() != "") {
        SearchQuery query;
        query.searchText = m_ui->lineEdit->text();
        query.regExp = m_ui->checkBox_regexp->isChecked();
        query.wholeWord = m_ui->checkBox_whole->isChecked();
        query.caseSensitive = m_ui->checkBox_casesen->isChecked();
        emit searched(query);
        close();
    }
}
