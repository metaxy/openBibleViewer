#include "strongdockwidget.h"
#include "ui_strongdockwidget.h"
#include "src/core/dbghelper.h"
StrongDockWidget::StrongDockWidget(QWidget *parent) :
        DockWidget(parent),
        ui(new Ui::StrongDockWidget)
{
    ui->setupUi(this);
    connect(ui->comboBox_strongModule,SIGNAL(currentIndexChanged(int)),this,SLOT(loadModule(int)));
    connect(ui->toolButton_strongSearch,SIGNAL(clicked()),this,SLOT(search()));
    connect(ui->textBrowser_strong,SIGNAL(anchorClicked(QUrl)),this,SLOT(get_(QUrl)));
}

StrongDockWidget::~StrongDockWidget()
{
    delete ui;
}
void StrongDockWidget::init()
{
   strongModuleTitle.clear();
   strongModuleID.clear();
   for(int i=0; i< m_moduleManager->m_moduleList.size();i++) {
        Module m = m_moduleManager->m_moduleList.at(i);
        if(m.m_moduleClass == Module::StrongModule) {
            strongModuleTitle << m.m_title;
            strongModuleID << m.m_id;
        }
    }
    ui->comboBox_strongModule->insertItems(0, strongModuleTitle);
    m_strong.setSettings(m_settings);
}

void StrongDockWidget::search()
{
    QString s = ui->lineEdit_strong->text();
    if (s.size() > 0)
        showStrong(s);
}
void StrongDockWidget::showStrong(QString strongID)
{
    if (isHidden()) {
        show();
    }
    if(!m_moduleManager->strongLoaded()) {
        loadModule(0);
    }
    ui->lineEdit_strong->setText(strongID);
    ui->textBrowser_strong->setText(m_strong.loadStrong(strongID));
}
void StrongDockWidget::loadModule(int id)
{
    //DEBUG_FUNC_NAME
    if(strongModuleID.size() > id && id >= 0) {
        int moduleID = strongModuleID.at(id);
        m_strong.setSettings(m_settings);
        m_strong.loadStrongModule(moduleID,m_moduleManager->m_moduleList.at(moduleID).m_path);
    }
}
void StrongDockWidget::get_(QUrl url)
{
    emit get(url);
}

void StrongDockWidget::changeEvent(QEvent *e)
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
