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
#include <QtGui/QFileDialog>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtCore/QPointer>
#include <QtGui/QPrintPreviewDialog>
#include <QtGui/QTextDocumentWriter>
#include "src/module/dictionary/webdictionary.h"
DictionaryForm::DictionaryForm(QWidget *parent) :
    Form(parent),
    ui(new Ui::DictionaryForm)
{
    ui->setupUi(this);
    connect(ui->toolButton_enter, SIGNAL(clicked()), this, SLOT(showEntry()));
    connect(ui->lineEdit_input, SIGNAL(returnPressed()), this, SLOT(showEntry()));
    connect(ui->toolButton_forward, SIGNAL(clicked()), this, SLOT(forward()));
    connect(ui->toolButton_back, SIGNAL(clicked()), this, SLOT(backward()));

    m_dictionary = NULL;

    m_view = new WebView(this);
    m_view->setObjectName("webView");
    m_view->setUrl(QUrl("about:blank"));
    ui->verticalLayout->addWidget(m_view);
    m_view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    setButtons();

}

DictionaryForm::~DictionaryForm()
{
    delete ui;
    if(m_dictionary != NULL) {
        delete m_dictionary;
        m_dictionary = NULL;
    }
}
void DictionaryForm::init()
{
    connect(m_actions, SIGNAL(_showHtml(QString)), this, SLOT(forwardShowHtml(QString)));
    connect(m_actions, SIGNAL(_showDictEntry(QString, int)), this, SLOT(forwardShowEntry(QString, int)));
    connect(m_view->page(), SIGNAL(linkClicked(QUrl)), this, SLOT(get(QUrl)));
    connect(m_view, SIGNAL(linkMiddleOrCtrlClicked(QUrl)), SLOT(newGet(QUrl)));
}
void DictionaryForm::get(const QUrl &url)
{
    DEBUG_FUNC_NAME
    if(m_dictionary->moduleType() == OBVCore::WebDictionaryModule) {
        WebDictionary *d = (WebDictionary*)(m_dictionary->module());
        m_actions->get(d->pharseUrl(url));
    } else {
        m_actions->get(url);
    }
}
void DictionaryForm::newGet(const QUrl &url)
{
    DEBUG_FUNC_NAME
    if(m_dictionary->moduleType() == OBVCore::WebDictionaryModule) {
        WebDictionary *d = (WebDictionary*)(m_dictionary->module());
        m_actions->get(d->pharseUrl(url), Actions::OpenInNewWindow);
    } else {
        m_actions->get(url, Actions::OpenInNewWindow);
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
    foreach(Module * m, m_moduleManager->m_moduleMap->data) {
        if(m->moduleUID() == uid) {
            moduleID = m->moduleID();
            break;
        }
    }
    if(moduleID != -1) {
        showEntry(k, moduleID);
    }

    m_view->page()->mainFrame()->setScrollPosition(scroll);
    m_view->setZoomFactor(zoom);
}

void DictionaryForm::save()
{
    const QString a = m_settings->session.id() + "/windows/" + QString::number(m_id) + "/";
    m_settings->session.file()->setValue(a + "type", "dictionary");
    m_settings->session.file()->setValue(a + "key", m_key);
    m_settings->session.file()->setValue(a + "scrollPosition", m_view->page()->mainFrame()->scrollPosition());
    m_settings->session.file()->setValue(a + "zoom",  m_view->zoomFactor());
    if(m_dictionary != NULL) {
        m_settings->session.file()->setValue(a + "uid", m_moduleManager->getModule(m_dictionary->moduleID())->moduleUID());
    }
}

void DictionaryForm::copy()
{
    m_view->page()->triggerAction(QWebPage::Copy);
}

void DictionaryForm::selectAll()
{
    m_view->page()->triggerAction(QWebPage::SelectAll);
}

void DictionaryForm::print()
{
    QPrinter printer;
    QPointer<QPrintDialog> dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print"));
    if(dialog->exec() == QDialog::Accepted) {
        m_view->page()->mainFrame()->print(&printer);
    }
    delete dialog;
}

void DictionaryForm::printPreview()
{
    QPrinter printer;
    QPointer<QPrintPreviewDialog> preview = new QPrintPreviewDialog(&printer, this);
    connect(preview, SIGNAL(paintRequested(QPrinter *)), m_view, SLOT(print(QPrinter *)));
    preview->exec();
    delete preview;
}

void DictionaryForm::saveFile()
{
    const QString lastPlace = m_settings->session.getData("lastSaveFilePlace").toString();
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Save output"), lastPlace, tr("Open Document (*.odt);;PDF (*.pdf);;Html (*.html *.htm);;Plain (*.txt)"));
    QFileInfo fi(fileName);
    m_settings->session.setData("lastSaveFilePlace", fi.path());
    if(fi.suffix().compare("html", Qt::CaseInsensitive) == 0 ||
            fi.suffix().compare("htm", Qt::CaseInsensitive) == 0) {
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&file);
        out << m_view->page()->mainFrame()->toHtml();
        file.close();
    } else if(fi.suffix().compare("pdf", Qt::CaseInsensitive) == 0) {
        QPrinter printer;
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        m_view->print(&printer);
    } else if(fi.suffix().compare("odt", Qt::CaseInsensitive) == 0) {
        QTextDocumentWriter writer;
        writer.setFormat("odf");
        writer.setFileName(fileName);
        QTextDocument doc;
        doc.setHtml(m_view->page()->mainFrame()->toHtml());
        writer.write(&doc);
    } else {
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&file);
        out << m_view->page()->mainFrame()->toPlainText();
        file.close();
    }
}

QString DictionaryForm::selectedText()
{
    return m_view->selectedText();
}

void DictionaryForm::zoomIn()
{
    m_view->setZoomFactor(m_view->zoomFactor() + 0.1);
}

void DictionaryForm::zoomOut()
{
    m_view->setZoomFactor(m_view->zoomFactor() - 0.1);
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
    const QString key = ui->lineEdit_input->text();
    historySetUrl("dict:/"+QString::number(m_dictionary->moduleID())+"/"+key);

    showEntry(key);
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
    } else if(m_dictionary->moduleType() == OBVCore::ZefaniaLexModule || m_dictionary->moduleType() == OBVCore::BibleQuoteDictModule) {
        m_key = key;
        const QString html = m_dictionary->getEntry(key);
        showHtml(html);
    } else if(m_dictionary->moduleType() == OBVCore::WebDictionaryModule) {
        m_key = key;
        const QString url = m_dictionary->getEntry(key);
        m_view->load(QUrl(url));
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
        int ret = m_dictionary->loadModuleData(moduleID);
        if(ret == 1) {
            showHtml("Could not load module " + QString::number(moduleID));
            return;
        }

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
void DictionaryForm::testDictionary(const int module)
{
    if(m_dictionary == NULL) {
        m_dictionary = new Dictionary();
        m_moduleManager->initSimpleModule(m_dictionary);
    }
    int newModuleID = newDictionary(module);
    if(newModuleID != -1)
        loadDictionary(newModuleID);
}
int DictionaryForm::newDictionary(const int module)
{
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
                return -1;
            }
            return moduleID;
        } else {
            return module;
        }
    } else if(m_dictionary->moduleID() != module && module != -1) {
        return module;
    }
    return -1;
}

void DictionaryForm::historySetUrl(const QString &url)
{
    myDebug() << url;
    m_browserHistory.setCurrent(url);
    setButtons();
}
void DictionaryForm::backward()
{
    m_actions->get(m_browserHistory.backward());
    setButtons();
}
void DictionaryForm::forward()
{
    m_actions->get(m_browserHistory.forward());
    setButtons();
}
void DictionaryForm::setButtons()
{
    if(m_browserHistory.backwardAvailable()) {
        ui->toolButton_back->setDisabled(false);
    } else {
        ui->toolButton_back->setDisabled(true);
    }
    if(m_browserHistory.forwardAvailable()) {
        ui->toolButton_forward->setDisabled(false);
    } else {
        ui->toolButton_forward->setDisabled(true);
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
    m_view->setHtml(html);
}
