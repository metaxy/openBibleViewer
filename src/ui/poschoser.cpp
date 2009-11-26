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

#include "ui_poschoser.h"

posChoser::posChoser(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::posChoser)
{
    m_ui->setupUi(this);
    connect(m_ui->pushButton_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(m_ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
}

posChoser::~posChoser()
{
    delete m_ui;
}
void posChoser::setData(QStringList bibles, QStringList books)
{
    m_ui->comboBox_books->insertItems(0, books);
    m_bibles = bibles;
}
void posChoser::setCurrent(int bible, QString path, int book, int chapter, int verse)
{
    //todo:load the bible info and from that the booknames
    m_bibleID = bible;
    m_bookID = book;
    m_chapterID = chapter;
    m_verseID = verse;
    m_path = path;
    m_ui->label_bible->setText(m_bibles.at(bible));
    m_ui->comboBox_books->setCurrentIndex(book);
    m_ui->spinBox_chapter->setValue(chapter);
    m_ui->spinBox_verse->setValue(verse);
}
void posChoser::save()
{
    QString out = "";
    out += m_path + ";";
    out += QString::number(m_ui->comboBox_books->currentIndex()) + ";";
    out += QString::number(m_ui->spinBox_chapter->value()) + ";";
    out += QString::number(m_ui->spinBox_verse->value()) + ";";
    close();
    emit updated(out);
}
void posChoser::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
