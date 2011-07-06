#include "webform.h"
#include "ui_webform.h"

WebForm::WebForm(QWidget *parent) :
    Form(parent),
    ui(new Ui::WebForm)
{
    ui->setupUi(this);
}

WebForm::~WebForm()
{
    delete ui;
}

void WebForm::init()
{
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(loadPage()));
}
void WebForm::loadPage()
{
    ui->webView->load(QUrl(ui->lineEdit->text()));
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
