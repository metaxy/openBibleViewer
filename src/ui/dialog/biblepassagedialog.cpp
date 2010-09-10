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
#include "biblepassagedialog.h"
#include "src/core/dbghelper.h"
#include "src/core/urlconverter.h"
#include "ui_biblepassagedialog.h"

BiblePassageDialog::BiblePassageDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::BiblePassageDialog)
{
    m_ui->setupUi(this);
    connect(m_ui->pushButton_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(m_ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_ui->comboBox_bibles, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));
}

BiblePassageDialog::~BiblePassageDialog()
{
    delete m_ui;
}
void BiblePassageDialog::setCurrent(const int &bible, const QString &path, const int &book, const int &chapter, const int &verse)
{
    Q_UNUSED(bible);
    //DEBUG_FUNC_NAME
    m_ui->comboBox_bibles->insertItems(0, m_moduleManager->getBibleTitles());
    m_bookID = book;
    m_chapterID = chapter + 1;
    m_verseID = verse + 1;
    m_path = path;

    const int newIndex = m_moduleManager->getBiblePaths().lastIndexOf(path);
    m_ui->comboBox_bibles->setCurrentIndex(newIndex);//todo: if lastindexof == -1 show a warning
    m_ui->comboBox_books->setCurrentIndex(m_settings->getModuleCache(m_path).bookNames.keys().indexOf(book));
    m_ui->spinBox_chapter->setValue(chapter);
    m_ui->spinBox_verse->setValue(verse);
}
void BiblePassageDialog::indexChanged(int index)
{
    if(index >= 0) {
        m_path = m_moduleManager->getBiblePaths().at(index);
        m_ui->comboBox_books->clear();
        m_ui->comboBox_books->insertItems(0, m_settings->getModuleCache(m_path).bookNames.values());
        //todo: set max using bookCount
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

void BiblePassageDialog::save()
{
    const QString link = m_path
                   + ";" + QString::number(m_settings->getModuleCache(m_path).bookNames.keys().at(m_ui->comboBox_books->currentIndex()))
                   + ";" + QString::number(m_ui->spinBox_chapter->value() - 1)
                   + ";" + QString::number(m_ui->spinBox_verse->value() - 1)
                   + ";" + m_settings->getModuleCache(m_path).bookNames.values().at(m_ui->comboBox_books->currentIndex());


    emit updated(link);
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
