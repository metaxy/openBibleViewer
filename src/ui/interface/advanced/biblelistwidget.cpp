#include "biblelistwidget.h"
#include "ui_biblelistwidget.h"
#include "src/core/dbghelper.h"
#include "biblelistitemdelegate.h"
#include <QtGui/QComboBox>
#include <QtGui/QDialogButtonBox>
BibleListWidget::BibleListWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BibleListWidget)
{
    ui->setupUi(this);
    connect(ui->toolButton_addCol,SIGNAL(clicked()),this,SLOT(addCol()));
    connect(ui->toolButton_addRow,SIGNAL(clicked()),this,SLOT(addRow()));
    connect(ui->toolButton_removeCol,SIGNAL(clicked()),this,SLOT(removeCol()));
    connect(ui->toolButton_removeRow,SIGNAL(clicked()),this,SLOT(removeRow()));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                          | QDialogButtonBox::Cancel);
    ui->gridLayout->addWidget(buttonBox,3,1);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(save()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(close()));

}
void BibleListWidget::init()
{
    int maxRow = 0;
    int maxCol = 0;
    foreach(QPoint p, m_moduleManager->bibleList()->m_biblePoints) {
        maxRow = qMax(maxRow,p.x());
        maxCol = qMax(maxCol,p.y());
    }
    m_model = new QStandardItemModel(maxRow,maxCol);
    m_selectionModel = new QItemSelectionModel(m_model);
    ui->tableView->setModel(m_model);
    ui->tableView->setSelectionModel(m_selectionModel);
    for(int i = 0; i <= maxRow; i++) {
        for(int j = 0; j <= maxCol; j++) {

            int id = m_moduleManager->bibleList()->m_biblePoints.key(QPoint(i,j),-1);
            Bible *b = m_moduleManager->bibleList()->bible(id);
            myDebug() << "id = " << id;
            QStandardItem *item;
            if(id >= 0 && b) {
                myDebug() << " title = " << b->bibleTitle() << " id = " << m_moduleManager->getBibleIDs().indexOf(b->moduleID()) << " moduleID " << b->moduleID();
                item = new QStandardItem(b->bibleTitle());
                item->setData(QVariant(b->moduleID()),Qt::UserRole+2);
                item->setData(QVariant(m_moduleManager->getBibleIDs().indexOf(b->moduleID())+1),Qt::UserRole+3);//todo: check if indexOF return -1

            } else {
                item = new QStandardItem("");
                item->setData(QVariant(0),Qt::UserRole+2);
                item->setData(QVariant(0),Qt::UserRole+3);

            }

            m_model->setItem(i, j, item);
        }
    }
    BibleListItemDelegate *delegate = new BibleListItemDelegate(this);
    setAll(delegate);
    delegate->init();
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
void BibleListWidget::save()
{
    //Bible *last = m_moduleManager->bible();
    int book = m_moduleManager->bible()->bookID();
    int chapter = m_moduleManager->bible()->chapterID();
   /* //list of setted points
    QList<QPoint> list;
    for(int x = 0; x < m_model->rowCount();++x) {
        for(int y = 0; y < m_model->columnCount(); ++y) {
            if(m_model->index(x,y).data(Qt::UserRole+2).toInt() > 0) {
                list << QPoint(x,y);
                myDebug() << "list  x = " << x << " y = " << y;
            }
        }
    }
    //remove all not used bibles( not setted points)
    QMapIterator<int, QPoint> i(m_moduleManager->bibleList()->m_biblePoints);
    while(i.hasNext()) {
        i.next();
        if(!list.contains(i.value())) {
            m_moduleManager->bibleList()->m_biblePoints.remove(i.key());
            m_moduleManager->bibleList()->m_bibles.remove(i.key());
        }
    }*/
    m_moduleManager->bibleList()->clear();
    //load them
    int selectedModule = -1;//the selected bible
    int lastModule;
    for(int x = 0; x < m_model->rowCount();++x) {
        for(int y = 0; y < m_model->columnCount(); ++y) {
            int id = m_model->item(x,y)->data(Qt::UserRole+2).toInt();
            if(id > 0) {
                m_moduleManager->newBible(id,QPoint(x,y));
                if(m_selectionModel->selection().contains(m_model->index(x,y)))
                    selectedModule = m_moduleManager->bibleList()->m_currentBible;
                lastModule = m_moduleManager->bibleList()->m_currentBible;
            }
        }
    }
    if(selectedModule != -1)
        m_moduleManager->bibleList()->m_currentBible = selectedModule;
    else
        m_moduleManager->bibleList()->m_currentBible = lastModule;
    myDebug() << book << chapter;
    m_bibleDisplay->emitGet("bible://"+QString::number(m_moduleManager->bibleList()->bible()->moduleID())+"/"+QString::number(book)+","+QString::number(chapter)+",0,force=true");//todo: make better
    close();
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
