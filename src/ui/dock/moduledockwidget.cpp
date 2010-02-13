#include "moduledockwidget.h"
#include "ui_moduledockwidget.h"
#include "src/core/dbghelper.h"
#include <QtGui/QToolTip>
#include <QtGui/QHelpEvent>
ModuleDockWidget::ModuleDockWidget(QWidget *parent) :
        DockWidget(parent),
        ui(new Ui::ModuleDockWidget)
{
    ui->setupUi(this);
    connect(ui->treeWidget_bibles, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(loadModuleData(QTreeWidgetItem*)));
    ui->treeWidget_bibles->installEventFilter(this);
    m_dontLoad = false;
}
void ModuleDockWidget::init()
{
    //DEBUG_FUNC_NAME
    ui->treeWidget_bibles->clear();
    ui->treeWidget_bibles->insertTopLevelItems(0, m_moduleManager->m_bibleItems);
    ui->treeWidget_bibles->sortByColumn(0, Qt::AscendingOrder);//sort
}
void ModuleDockWidget::loadModuleData(QTreeWidgetItem *fitem)
{
    //DEBUG_FUNC_NAME

    if (fitem->text(1).toInt() >= 0 && m_dontLoad == false) {
        emit get("bible://" + fitem->text(1) + "/0,0,0");
    }
}
void ModuleDockWidget::loadedModule(int id)
{
    DEBUG_FUNC_NAME
    myDebug() << "id = " << id;
    m_moduleID = id;
    iterateTreeWidget();
    //todo: implement this
}
void ModuleDockWidget::iterateTreeWidget(QTreeWidgetItem *parent)
{
    int count = parent ? parent->childCount() : ui->treeWidget_bibles->topLevelItemCount();
    for (int i = 0; i < count; i++)
    {
        m_dontLoad = true;
        QTreeWidgetItem *item = parent ? parent->child(i) : ui->treeWidget_bibles->topLevelItem(i);
        if(item->text(1).toInt() == m_moduleID) {
            item->setSelected(true);
        } else {
            item->setSelected(false);
        }
        iterateTreeWidget(item);
        m_dontLoad = false;
    }
}

bool ModuleDockWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->treeWidget_bibles) {
        if (event->type() == QEvent::ToolTip) {
            QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
            if (ui->treeWidget_bibles->itemAt(helpEvent->pos())) {
                int index = ui->treeWidget_bibles->itemAt(helpEvent->pos())->text(1).toInt();
                if (index != -1 && index < m_moduleManager->m_moduleList.size()) {
                    QString typ;
                    Module m = m_moduleManager->m_moduleList.at(index);
                    if (m.m_moduleType == Module::BibleQuoteModule) {
                        typ = tr("BibleQuote Module");
                    } else if (m.m_moduleType == Module::ZefaniaBibleModule) {
                        typ = tr("Zefania XML Module");
                    }
                    QToolTip::showText(helpEvent->globalPos(), typ + " - " + m.m_path);
                }
            }
        } else {
            return QWidget::eventFilter(obj, event);;
        }
    } else {
        return QWidget::eventFilter(obj, event);
    }
}
ModuleDockWidget::~ModuleDockWidget()
{
    delete ui;
}

void ModuleDockWidget::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
