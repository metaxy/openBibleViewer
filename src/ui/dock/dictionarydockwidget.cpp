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
#include "dictionarydockwidget.h"
#include "ui_dictionarydockwidget.h"
#include "src/core/dbghelper.h"
#include "src/core/obvcore.h"
#include <QtGui/QCompleter>
DictionaryDockWidget::DictionaryDockWidget(QWidget *parent) :
    DockWidget(parent),
    ui(new Ui::DictionaryDockWidget)
{
    ui->setupUi(this);
    m_dictionary = NULL;
}

DictionaryDockWidget::~DictionaryDockWidget()
{
    delete ui;
    if(m_dictionary != NULL) {
        delete m_dictionary;
        m_dictionary = NULL;
    }
}
void DictionaryDockWidget::init()
{
    if(m_dictionary == NULL) {
        m_dictionary = new Dictionary();
        m_moduleManager->initSimpleModule(m_dictionary);
    }

    dictModuleTitle.clear();
    dictModuleID.clear();
    ui->comboBox_strongModule->clear();

    QMapIterator<int, Module *> i(m_moduleManager->m_moduleMap->data);
    while(i.hasNext()) {
        i.next();
        Module *m = i.value();
        if(m->moduleClass() == OBVCore::DictionaryModuleClass) {
            dictModuleTitle << m->title();
            dictModuleID << m->moduleID();
        }
    }
    ui->comboBox_strongModule->insertItems(0, dictModuleTitle);
    connect(ui->comboBox_strongModule, SIGNAL(currentIndexChanged(int)), this, SLOT(loadModule(int)));
    connect(ui->comboBox_strongModule, SIGNAL(currentIndexChanged(int)), this, SLOT(search()));

    connect(ui->toolButton_strongSearch, SIGNAL(clicked()), this, SLOT(search()));
    connect(ui->lineEdit_strong, SIGNAL(returnPressed()), this, SLOT(search()));

    connect(ui->textBrowser_strong, SIGNAL(anchorClicked(QUrl)), m_actions, SLOT(get(QUrl)));

}

void DictionaryDockWidget::search()
{
    const QString s = ui->lineEdit_strong->text();
    if(!s.isEmpty())
        showEntry(s);
}
void DictionaryDockWidget::showEntry(QString strongID)
{
    if(isHidden()) {
        show();
    }
    if(!m_moduleManager->dictionaryLoaded(m_dictionary))) {
        const QString last = m_settings->session.getData("lastDictModule").toString();
        int moduleID = 0;
        if(!last.isEmpty()) {
            QMapIterator<int, Module *> i(m_moduleManager->m_moduleMap->data);
            while(i.hasNext()) {
                i.next();
                if(i.value()->moduleUID() == last) {
                    moduleID = dictModuleID.lastIndexOf(i.key());
                    break;
                }

            }
        }
        moduleID = moduleID != -1 ? moduleID : 0;
        loadModule(moduleID);
    }
    ui->lineEdit_strong->setText(strongID);
    ui->textBrowser_strong->setText(m_dictionary->getEntry(strongID));
}
void DictionaryDockWidget::loadModule(int id)
{
    DEBUG_FUNC_NAME
    if(dictModuleID.size() > id && id >= 0) {
        ui->comboBox_strongModule->setCurrentIndex(id);//select item in combobox
        const int moduleID = dictModuleID.at(id);//convert id to module id
        Module *m = m_moduleManager->getModule(moduleID);
        OBVCore::ModuleType type = m->moduleType();
        m_dictionary->setModuleType(type);
        m_dictionary->loadModuleData(moduleID);

        QCompleter *completer = new QCompleter(m_dictionary->getAllKeys(), this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        ui->lineEdit_strong->setCompleter(completer);

        //todo: do this when closing
        m_settings->session.setData("lastDictModule", m->moduleUID());
    }
}

void DictionaryDockWidget::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch(e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
