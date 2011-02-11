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
#include "biblepassagedialog.h"
#include "src/core/dbghelper.h"
#include "src/core/urlconverter.h"
#include "ui_biblepassagedialog.h"

BiblePassageDialog::BiblePassageDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::BiblePassageDialog)
{
    m_ui->setupUi(this);
    connect(m_ui->buttonBox, SIGNAL(accepted()), this, SLOT(save()));
    connect(m_ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
    m_frame = new BiblePassageFrame(this);
    m_ui->verticalLayout_2->addWidget(m_frame);
}
BiblePassageDialog::~BiblePassageDialog()
{
    delete m_ui;
}
void BiblePassageDialog::init()
{
    setAll(m_frame);
    m_frame->init();
}

BiblePassageFrame *BiblePassageDialog::frame()
{
    return m_frame;
}

void BiblePassageDialog::save()
{
    emit updated(m_frame->toVerseUrl());
    close();
}
void BiblePassageDialog::changeEvent(QEvent *e)
{

    switch(e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
