#include "bookform.h"
#include "ui_bookform.h"
#include "src/module/response/stringresponse.h"
BookForm::BookForm(QWidget *parent) :
    WebViewForm(parent),
    ui(new Ui::BookForm)
{
    ui->setupUi(this);
    ui->verticalLayout->addWidget(m_view);
    m_book = NULL;
}

BookForm::~BookForm()
{
    if(m_book != NULL) {
        delete m_book;
        m_book = NULL;
    }
    delete ui;
}
void BookForm::init()
{

}

void BookForm::restore(const QString &key)
{

}

void BookForm::save()
{

}

Form::FormType BookForm::type() const
{
    return Form::BookForm;
}

void BookForm::activated()
{

}
void BookForm::loadModule(const int moduleID)
{
    if(m_book == NULL) {
        m_book = new Book();
        m_moduleManager->initSimpleModule(m_book);
    }
    ModuleTools::ModuleType type = m_moduleManager->getModule(moduleID)->moduleType();
    m_book->setModuleType(type);
    m_book->setModuleID(moduleID);
}

void BookForm::show()
{
    if(!m_book) {
        m_view->setHtml(tr("Not loaded"));
        return;
    }

    StringResponse *r = (StringResponse *)m_book->getAll();
    if(r) {
        m_view->setHtml(r->data());
    }
    delete r;
}
