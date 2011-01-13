/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
    connect(m_ui->pushButton_search, SIGNAL(clicked()), this, SLOT(search()));
    connect(m_ui->toolButton_nt, SIGNAL(clicked()), this, SLOT(uncheck()));
    connect(m_ui->toolButton_ot, SIGNAL(clicked()), this, SLOT(uncheck()));
    connect(m_ui->toolButton_whole, SIGNAL(clicked()), this, SLOT(uncheck()));
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
void SearchDialog::uncheck()
{
    if(static_cast<QToolButton *>(sender())->isChecked()) {
        const QString senderName = sender()->objectName();
        if(senderName == "toolButton_whole") {
            m_ui->toolButton_nt->setChecked(false);
            m_ui->toolButton_ot->setChecked(false);
        } else if(senderName == "toolButton_ot") {
            m_ui->toolButton_nt->setChecked(false);
            m_ui->toolButton_whole->setChecked(false);
        } else if(senderName == "toolButton_nt") {
            m_ui->toolButton_whole->setChecked(false);
            m_ui->toolButton_ot->setChecked(false);
        }
    }
}
void SearchDialog::setText(const QString &text)
{
    m_ui->lineEdit_query->setText(text);
}
void SearchDialog::search(void)
{
    if(!m_ui->lineEdit_query->text().isEmpty()) {
        SearchQuery query;
        query.searchText = m_ui->lineEdit_query->text();
        query.searchInNotes = m_ui->checkBox_searchInNotes->isChecked();
        if(m_ui->toolButton_whole->isChecked()) {
            query.range = SearchQuery::Whole;
        } else if(m_ui->toolButton_ot->isChecked()) {
            query.range = SearchQuery::OT;
        } else if(m_ui->toolButton_nt->isChecked()) {
            query.range = SearchQuery::NT;
        }

        emit searched(query);
        close();
    }
}
