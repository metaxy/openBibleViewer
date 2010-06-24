#include "biblelistwidget.h"
#include "ui_biblelistwidget.h"
#include "src/core/dbghelper.h"
#include "biblelistitemdelegate.h"
BibleListWidget::BibleListWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BibleListWidget)
{
    ui->setupUi(this);
    connect(ui->toolButton_addCol,SIGNAL(clicked()),this,SLOT(addCol()));
    connect(ui->toolButton_addRow,SIGNAL(clicked()),this,SLOT(addRow()));
    connect(ui->toolButton_removeCol,SIGNAL(clicked()),this,SLOT(removeCol()));
    connect(ui->toolButton_removeRow,SIGNAL(clicked()),this,SLOT(removeRow()));

}
void BibleListWidget::init()
{
    int maxRow = 0;
    int maxCol = 0;
    foreach(QPoint p, m_moduleManager->bibleList()->m_biblePoints) {
        maxRow = qMax(maxRow,p.y());
        maxCol = qMax(maxCol,p.x());
    }
    m_model = new QStandardItemModel(maxRow,maxCol);
    ui->tableView->setModel(m_model);
    for(int i = 0; i <= maxRow; i++) {
        for(int j = 0; j <= maxCol; j++) {
            int id = m_moduleManager->bibleList()->m_biblePoints.key(QPoint(i,j));
            Bible *b = m_moduleManager->bibleList()->m_bibles.value(id);
            QStandardItem *item;
            if(b) {
                item = new QStandardItem(b->bibleTitle());
                item->setData(QVariant(b->bibleID()));
            } else {
                item = new QStandardItem("");
            }

            m_model->setItem(i, j, item);
        }
    }
    BibleListItemDelegate *delegate = new BibleListItemDelegate();
    ui->tableView->setItemDelegate(delegate);

}
void BibleListWidget::removeCol()
{
    m_model->removeColumn(m_model->columnCount()-1);
}
void BibleListWidget::addCol()
{
    QList<QStandardItem *> list;
    for(int i = 0; i < m_model->rowCount(); i++) {
        QStandardItem *item = new QStandardItem("");
        list.append(item);
    }
    m_model->appendColumn(list);
}

void BibleListWidget::removeRow()
{
    m_model->removeRow(m_model->rowCount()-1);
}

void BibleListWidget::addRow()
{
    QList<QStandardItem *> list;
    for(int i = 0; i < m_model->columnCount(); i++) {
        QStandardItem *item = new QStandardItem("");
        list.append(item);
    }
    m_model->appendRow(list);
}

BibleListWidget::~BibleListWidget()
{
    delete ui;
    ui = 0;
    delete m_model;
    m_model = 0;
}

void BibleListWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
