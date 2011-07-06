#include "webform.h"
#include "ui_webform.h"
#include <QtGui/QToolBar>
WebForm::WebForm(QWidget *parent) :
    Form(parent),
    m_ui(new Ui::WebForm)
{
    m_ui->setupUi(this);

    connect(m_ui->lineEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));
    m_ui->toolButton_back->setIcon(m_ui->webView->pageAction(QWebPage::Back)->icon());
    m_ui->toolButton_back->setToolTip(m_ui->webView->pageAction(QWebPage::Back)->toolTip());
    connect(m_ui->toolButton_back, SIGNAL(clicked()), m_ui->webView, SLOT(back()));

    m_ui->toolButton_forward->setIcon(m_ui->webView->pageAction(QWebPage::Forward)->icon());
    m_ui->toolButton_forward->setToolTip(m_ui->webView->pageAction(QWebPage::Forward)->toolTip());
    connect(m_ui->toolButton_forward, SIGNAL(clicked()), m_ui->webView, SLOT(forward()));

    /*QToolBar *toolBar = new QToolBar(tr("Navigation"), this);
    toolBar->addAction();
    toolBar->addAction(m_ui->webView->pageAction(QWebPage::Forward));
    toolBar->addAction(m_ui->webView->pageAction(QWebPage::Reload));
    toolBar->addAction(m_ui->webView->pageAction(QWebPage::Stop));*/


}

WebForm::~WebForm()
{
    delete m_ui;
}

void WebForm::init()
{
}
void WebForm::changeLocation()
{
    m_ui->webView->load(QUrl(m_ui->lineEdit->text()));
}
void WebForm::adjustLocation()
{

}

void WebForm::copy()
{

}

void WebForm::selectAll()
{

}

void WebForm::print()
{

}

void WebForm::printPreview()
{

}

void WebForm::saveFile()
{

}

QString WebForm::selectedText()
{
    return "";
}

void WebForm::zoomIn()
{

}

void WebForm::zoomOut()
{

}

void WebForm::activated()
{

}
