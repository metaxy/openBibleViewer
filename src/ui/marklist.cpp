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
#include "marklist.h"
#include "ui_marklist.h"
#include "src/core/dbghelper.h"
#include "src/core/urlconverter.h"
MarkList::MarkList(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::MarkList)
{
    ui->setupUi(this);

}
void MarkList::init()
{
    if (!m_notes->isLoaded()) {
        m_notes->init(m_settings->homePath + "notes.xml");
        m_notes->loadNotes();
        m_notes->readNotes();
    }

    m_notes->loadingNewInstance();
    connect(m_notes, SIGNAL(saveAll()), this, SLOT(saveNote()));
    connect(ui->tableView, SIGNAL(activated(QModelIndex)), this, SLOT(load(QModelIndex)));
    connect(this, SIGNAL(get(QString)), m_bibleDisplay, SIGNAL(get(QString)));
    QStringList marks;
    QStringList id = m_notes->getIDList();
    for (int i = 0; i < id.size(); ++i) {
        if (m_notes->getType(id.at(i)) == "mark") {
            marks << id.at(i);
        }
    }
    m_itemModel = new QStandardItemModel(marks.size(), 4);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_selectionModel = new QItemSelectionModel(m_proxyModel);

    m_itemModel->clear();

    myDebug() << " id = " << id;

    for (int row = 0; row < marks.size(); ++row) {

        QString string = "";
        UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, m_notes->getRef(marks.at(row), "link"));
        urlConverter.m_biblesRootPath = m_moduleManager->bible()->biblesRootPath();
        urlConverter.pharse();

        string =  urlConverter.m_bookName + " " + QString::number(urlConverter.m_chapterID + 1) + "," +
                  QString::number(urlConverter.m_verseID + 1);

        QStandardItem *stelle = new QStandardItem(string);
        stelle->setData(marks.at(row));
        m_itemModel->setItem(row, 0, stelle);

        QStandardItem *modul = new QStandardItem(m_moduleManager->m_moduleList.at(urlConverter.m_bibleID.toInt()).m_title);
        m_itemModel->setItem(row, 1, modul);

    }
    m_proxyModel->setSourceModel(m_itemModel);
    m_proxyModel->setHeaderData(0, Qt::Horizontal, tr("Mark Position"));
    m_proxyModel->setHeaderData(1, Qt::Horizontal, tr("Module"));


    ui->tableView->setSortingEnabled(true);
    ui->tableView->setModel(m_proxyModel);
    ui->tableView->setSelectionModel(m_selectionModel);
}
void MarkList::load(QModelIndex index)
{
    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, m_notes->getRef(index.data(Qt::UserRole + 1).toString(), "link"));
    urlConverter.m_biblesRootPath = m_moduleManager->bible()->biblesRootPath();
    urlConverter.pharse();
    QString link = urlConverter.convert();
    emit get(link);
}

MarkList::~MarkList()
{
    delete ui;
}

void MarkList::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
