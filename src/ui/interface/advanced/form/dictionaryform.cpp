#include "dictionaryform.h"
#include "ui_dictionaryform.h"

DictionaryForm::DictionaryForm(QWidget *parent) :
    Form(parent),
    ui(new Ui::DictionaryForm)
{
    ui->setupUi(this);
    connect(ui->toolButton_enter, SIGNAL(clicked()), this, SLOT(showEntry()));
    m_dictionary = NULL;
}

DictionaryForm::~DictionaryForm()
{
    delete ui;
}
void DictionaryForm::init()
{
    connect(m_actions, SIGNAL(_showHtml(QString)), this, SLOT(showHtml(QString)));
}

void DictionaryForm::restore(const QString &key)
{

}

void DictionaryForm::save()
{

}

void DictionaryForm::copy()
{

}

void DictionaryForm::selectAll()
{

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
    return "";
}

void DictionaryForm::zoomIn()
{

}

void DictionaryForm::zoomOut()
{

}

void DictionaryForm::activated()
{

}
void DictionaryForm::showEntry()
{
    showEntry(ui->lineEdit_input->text());
}
void DictionaryForm::showEntry(const QString &key)
{
    testDictionary();

    m_actions->needDictionaryWindow();
    if(m_dictionary->moduleType() == OBVCore::ZefaniaLexModule || m_dictionary->moduleType() == OBVCore::BibleQuoteDictModule) {
        const QString html = m_dictionary->getEntry(key);
        m_actions->showHtml(html);
        //get text
        //show html
    }
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
    }
}
void DictionaryForm::testDictionary()
{
    if(m_dictionary == NULL) {
        m_dictionary = new Dictionary();

        m_dictionary->setModuleMap(m_moduleManager->m_moduleMap);
        m_dictionary->setNotes(m_notes);
        m_dictionary->setSettings(m_settings);
    }
    if(!m_moduleManager->dictionaryLoaded(m_dictionary)) {
        const QString last = m_settings->session.getData("lastDictModuleInWindow").toString();
        int moduleID = -1;

        const QString lastUrl = m_settings->recoverUrl(last);
        QMapIterator<int, Module *> i(m_moduleManager->m_moduleMap->m_map);
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
    }
}
void DictionaryForm::showHtml(const QString &html)
{
    if(!active())
        return;
    ui->webView->setHtml(html);
}
