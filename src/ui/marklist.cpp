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
#include "src/core/urlconverter2.h"
#include "src/core/core.h"
#include <QtGui/QMenu>
#include <QtGui/QCursor>
MarkList::MarkList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MarkList)
{
    ui->setupUi(this);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
}
MarkList::~MarkList()
{
    delete ui;
    delete m_itemModel;
    delete m_proxyModel;
}
void MarkList::init()
{
    DEBUG_FUNC_NAME
    if(!m_notes->isLoaded()) {
        m_notes->init(m_settings->homePath + "notes.xml");
        m_notes->loadNotes();
        m_notes->readNotes();
    }

    m_notes->loadingNewInstance();
    connect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));
    connect(m_notes, SIGNAL(noteRemoved(QString, QMap<QString, QString>)), this, SLOT(removeNote(QString)));

    connect(ui->tableView, SIGNAL(activated(QModelIndex)), this, SLOT(load(QModelIndex)));

    const QStringList marks = m_notes->getIDList("mark");
    m_itemModel = new QStandardItemModel(marks.size(), 2);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_selectionModel = new QItemSelectionModel(m_proxyModel);

    for(int row = 0; row < marks.size(); ++row) {
        addMark(row, marks.at(row));
    }
    m_proxyModel->setSourceModel(m_itemModel);
    m_proxyModel->setHeaderData(0, Qt::Horizontal, tr("Mark Position"));
    m_proxyModel->setHeaderData(1, Qt::Horizontal, tr("Module"));


    ui->tableView->setSortingEnabled(true);
    ui->tableView->setModel(m_proxyModel);
    ui->tableView->setSelectionModel(m_selectionModel);
}
void MarkList::addMark(const int &row, const QString &id)
{
    UrlConverter2 urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, m_notes->getRef(id, "link"));
    urlConverter.setSM(m_settings, m_moduleManager->m_moduleMap);
    urlConverter.convert();

    const QString string = urlConverter.bookName() + " " + QString::number(urlConverter.chapterID() + 1) + "," +
                            QString::number(urlConverter.verseID() + 1);

    QStandardItem *stelle = new QStandardItem(string);
    stelle->setData(id);

    m_itemModel->setItem(row, 0, stelle);

    QString t = "";
    //todo: this can be faster
    if(m_moduleManager->getModule(urlConverter.moduleID()) && !m_moduleManager->getModule(urlConverter.moduleID())->title().isEmpty())
        t = m_moduleManager->getModule(urlConverter.moduleID())->title();

    QStandardItem *module = new QStandardItem(t);
    m_itemModel->setItem(row, 1, module);
}

void MarkList::load(QModelIndex index)
{
    const int row = index.row();
    index = m_itemModel->index(row, 0);
    UrlConverter2 urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, m_notes->getRef(index.data(Qt::UserRole + 1).toString(), "link"));
    urlConverter.setSM(m_settings, m_moduleManager->m_moduleMap);
    urlConverter.convert();
    m_actions->get(urlConverter.url());
}


void MarkList::showContextMenu(QPoint point)
{
    QMenu *contextMenu = new QMenu(this);
    QAction *actionDelete = new QAction(QIcon::fromTheme("edit-delete",
                                        QIcon(":/icons/16x16/edit-delete.png")),
                                        tr("Delete"),
                                        contextMenu);
    connect(actionDelete, SIGNAL(triggered()), this, SLOT(deleteMarks()));
    contextMenu->addAction(actionDelete);
    contextMenu->exec(QCursor::pos());
    m_currentPoint = point;
}
void MarkList::addNote(const QString &id)
{
    if(m_notes->getType(id) == "mark") {
        addMark(m_itemModel->rowCount(), id);
    }
}
void MarkList::removeNote(const QString &id)
{
    QModelIndexList list = m_proxyModel->match(m_proxyModel->index(0, 0),
                           Qt::UserRole + 1,
                           id);
    if(list.size() != 1) {
        return;
    }
    QModelIndex index = list.at(0);
    m_proxyModel->removeRow(index.row());
}
void MarkList::deleteMarks()
{
    disconnect(m_notes, SIGNAL(noteRemoved(QString, QMap<QString, QString>)),
               this, SLOT(removeNote(QString)));
    QModelIndexList list = m_selectionModel->selectedRows();
    if(list.isEmpty()) {
        QModelIndex index = ui->tableView->indexAt(m_currentPoint);
        if(index.isValid()) {
            QString id = index.data(Qt::UserRole + 1).toString();
            m_notes->removeNote(id);
            m_proxyModel->removeRow(index.row());
        }

    } else {
        while(list.size() != 0) {
            QModelIndex index = list.at(0);
            QString id = index.data(Qt::UserRole + 1).toString();
            m_notes->removeNote(id);
            m_proxyModel->removeRow(index.row());
            list = m_selectionModel->selectedRows();
        }
    }
    connect(m_notes, SIGNAL(noteRemoved(QString, QMap<QString, QString>)),
            this, SLOT(removeNote(QString)));
}
void MarkList::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch(e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        m_proxyModel->setHeaderData(0, Qt::Horizontal, tr("Mark Position"));
        m_proxyModel->setHeaderData(1, Qt::Horizontal, tr("Module"));
        break;
    default:
        break;
    }
}
