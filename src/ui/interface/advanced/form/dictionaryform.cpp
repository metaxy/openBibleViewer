/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger <metaxy@walger.name>
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
#include "dictionaryform.h"
#include "ui_dictionaryform.h"

#include <QtGui/QCompleter>
#include <QtCore/QScopedPointer>
#include "src/module/dictionary/webdictionary.h"
DictionaryForm::DictionaryForm(QWidget *parent) :
    Form(parent),
    ui(new Ui::DictionaryForm)
{
    ui->setupUi(this);
    connect(ui->toolButton_enter, SIGNAL(clicked()), this, SLOT(showEntry()));
    connect(ui->lineEdit_input, SIGNAL(returnPressed()), this, SLOT(showEntry()));

    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    m_dictionary = NULL;
}

DictionaryForm::~DictionaryForm()
{
    delete ui;
}
void DictionaryForm::init()
{
    connect(m_actions, SIGNAL(_showHtml(QString)), this, SLOT(forwardShowHtml(QString)));
    connect(m_actions, SIGNAL(_showDictEntry(QString, int)), this, SLOT(forwardShowEntry(QString, int)));
    connect(ui->webView->page(), SIGNAL(linkClicked(QUrl)), this, SLOT(get(QUrl)));
}
void DictionaryForm::get(QUrl url)
{
    if(m_dictionary->moduleType() == OBVCore::WebDictionaryModule) {
        WebDictionary *d = (WebDictionary*) (m_dictionary->module());
        m_actions->get(d->pharseUrl(url));
    } else {
        m_actions->get(url);
    }
}

Form::FormType DictionaryForm::type() const
{
    return Form::DictionaryForm;
}
Dictionary* DictionaryForm::dictionary() const
{
    return m_dictionary;
}
void DictionaryForm::restore(const QString &key)
{
    const QString a = m_settings->session.id() + "/windows/" + key + "/";
    const qreal zoom = m_settings->session.file()->value(a + "zoom").toReal();
    const QPoint scroll = m_settings->session.file()->value(a + "scrool").toPoint();

    const QString k = m_settings->session.file()->value(a + "key").toString();
    const QString uid = m_settings->session.file()->value(a + "uid").toString();
    int moduleID = -1;
    foreach(Module*m,m_moduleManager->m_moduleMap->data) {
        if(m->moduleUID() == uid) {
            moduleID = m->moduleID();
            break;
        }
    }
    if(moduleID != -1) {
        showEntry(k, moduleID);
    }

    ui->webView->page()->mainFrame()->setScrollPosition(scroll);
    ui->webView->setZoomFactor(zoom);
}

void DictionaryForm::save()
{
    const QString a = m_settings->session.id() + "/windows/" + QString::number(m_id) + "/";
    m_settings->session.file()->setValue(a + "type", "dictionary");
    m_settings->session.file()->setValue(a + "key", m_key);
    m_settings->session.file()->setValue(a + "scrollPosition", ui->webView->page()->mainFrame()->scrollPosition());
    m_settings->session.file()->setValue(a + "zoom",  ui->webView->zoomFactor());
    if(m_dictionary != NULL) {
        m_settings->session.file()->setValue(a + "uid", m_moduleManager->getModule(m_dictionary->moduleID())->moduleUID());
    }
}

void DictionaryForm::copy()
{
    ui->webView->page()->triggerAction(QWebPage::Copy);
}

void DictionaryForm::selectAll()
{
    ui->webView->page()->triggerAction(QWebPage::SelectAll);
}

void DictionaryForm::print()
{

}

void DictionaryForm::printPreview()
{

}

void DictionaryForm::saveFile()
{

}

QString DictionaryForm::selectedText()
{
    return ui->webView->selectedText();
}

void DictionaryForm::zoomIn()
{
    ui->webView->setZoomFactor(ui->webView->zoomFactor() + 0.1);
}

void DictionaryForm::zoomOut()
{
    ui->webView->setZoomFactor(ui->webView->zoomFactor() - 0.1);
}

void DictionaryForm::activated()
{
    if(m_dictionary == NULL)
        return;
    m_actions->setTitle(m_dictionary->moduleTitle());
    m_actions->setCurrentModule(m_dictionary->moduleID());
}
void DictionaryForm::showEntry()
{
    showEntry(ui->lineEdit_input->text());
}
void DictionaryForm::forwardShowEntry(const QString &key, int moduleID)
{
    if(!active())
        return;
    showEntry(key, moduleID);
}
/**
  * moduleID == -1 means current Dictionary
  */
void DictionaryForm::showEntry(const QString &key, int moduleID)
{
    myDebug() << key;
    ui->lineEdit_input->setText(key);
    if(moduleID != -1)
        testDictionary(moduleID);

    m_key = "";

    if(key.isEmpty()) {
        showHtml(m_dictionary->moduleTitle());
    }
    else if(m_dictionary->moduleType() == OBVCore::ZefaniaLexModule || m_dictionary->moduleType() == OBVCore::BibleQuoteDictModule) {
        m_key = key;
        const QString html = m_dictionary->getEntry(key);
        showHtml(html);
    } else if(m_dictionary->moduleType() == OBVCore::WebDictionaryModule) {
        m_key = key;
        const QString url = m_dictionary->getEntry(key);
        ui->webView->load(QUrl(url));
    }
    m_actions->setTitle(m_dictionary->moduleTitle());
    //else
    //show url
}
void DictionaryForm::loadDictionary(int moduleID)
{
    DEBUG_FUNC_NAME
    if(moduleID >= 0) {
        myDebug() << "moduleID = " << moduleID;
        Module *m = m_moduleManager->getModule(moduleID);
        OBVCore::ModuleType type = m->moduleType();
        m_dictionary->setModuleType(type);
        m_dictionary->loadModuleData(moduleID);

        //todo: do this when closing
        m_settings->session.setData("lastDictModuleInWindow", m_settings->savableUrl(m->path()));


        QCompleter *completer = new QCompleter(m_dictionary->getAllKeys(), this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        if(ui->lineEdit_input->completer()) {
            delete ui->lineEdit_input->completer();
        }
        ui->lineEdit_input->setCompleter(completer);

        m_actions->setTitle(m_dictionary->moduleTitle());
    }
}
void DictionaryForm::testDictionary(int module)
{
    if(m_dictionary == NULL) {
        m_dictionary = new Dictionary();
        m_moduleManager->initSimpleModule(m_dictionary);
    }
    if(!m_moduleManager->metaModuleLoaded(m_dictionary)) {
        if(module == -1) {
            const QString last = m_settings->session.getData("lastDictModuleInWindow").toString();
            int moduleID = -1;

            const QString lastUrl = m_settings->recoverUrl(last);
            QMapIterator<int, Module *> i(m_moduleManager->m_moduleMap->data);
            while(i.hasNext()) {
                i.next();
                if(i.value()->moduleClass() == OBVCore::DictionaryModuleClass) {
                    moduleID = i.key();
                    if(i.value()->path() == lastUrl) {
                        break;
                    }
                }
            }

            if(moduleID == -1) {
                myDebug() << "no Dictionaries found";
                return;
            }
            loadDictionary(moduleID);
        } else {
            loadDictionary(module);
        }
    } else if(m_dictionary->moduleID() != module && module != -1) {
        loadDictionary(module);
    }
}
void DictionaryForm::forwardShowHtml(const QString &html)
{
    if(!active())
        return;
    showHtml(html);
}
void DictionaryForm::showHtml(const QString &html)
{
    ui->webView->setHtml(html);
}
