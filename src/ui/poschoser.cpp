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
#include "poschoser.h"
#include <QtCore/QtDebug>
#include "../core/dbghelper.h"
#include "ui_poschoser.h"

PosChoser::PosChoser(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::PosChoser)
{
    m_ui->setupUi(this);
    connect(m_ui->pushButton_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(m_ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_ui->comboBox_bibles, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));
}

PosChoser::~PosChoser()
{
    delete m_ui;
}
void PosChoser::setSettings(Settings *set)
{
    m_settings = set;
}
void PosChoser::setCurrent(const int &bible,const QString &path, const int &book, const int &chapter, const int &verse)
{
    DEBUG_FUNC_NAME
    m_ui->comboBox_bibles->insertItems(0, m_settings->getBibleName());
    m_bookID = book;
    m_chapterID = chapter;
    m_verseID = verse;
    m_path = path;

    int newIndex = m_settings->getBiblePath().lastIndexOf(path);
    m_ui->comboBox_bibles->setCurrentIndex(newIndex);//todo: if lastindexof == -1 make warning
    m_ui->comboBox_books->setCurrentIndex(book);
    m_ui->spinBox_chapter->setValue(chapter);
    m_ui->spinBox_verse->setValue(verse);
}
void PosChoser::indexChanged(int index)
{
    DEBUG_FUNC_NAME
    if (index >= 0) {
        m_ui->comboBox_books->clear();
        m_ui->comboBox_books->insertItems(0, m_settings->getBookNames().at(index));
        m_path = m_settings->getBiblePath().at(index);

        m_ui->comboBox_books->setCurrentIndex(0);
        m_ui->spinBox_chapter->setValue(1);
        m_ui->spinBox_verse->setValue(1);
    } else {
        m_ui->comboBox_books->clear();
        m_ui->comboBox_books->setCurrentIndex(0);
        m_ui->spinBox_chapter->setValue(1);
        m_ui->spinBox_verse->setValue(1);
    }
}

void PosChoser::save()
{
    QString out = "";
    out += m_path + ";";
    out += QString::number(m_ui->comboBox_books->currentIndex()) + ";";
    out += QString::number(m_ui->spinBox_chapter->value()) + ";";
    out += QString::number(m_ui->spinBox_verse->value()) + ";";
    emit updated(out);
    close();
}
void PosChoser::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
