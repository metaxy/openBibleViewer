#include "dictionaryform.h"
#include "ui_dictionaryform.h"

DictionaryForm::DictionaryForm(QWidget *parent) :
    Form(parent),
    ui(new Ui::DictionaryForm)
{
    ui->setupUi(this);
    connect(ui->toolButton_enter, SIGNAL(clicked()), this, SLOT(showEntry()));
}

DictionaryForm::~DictionaryForm()
{
    delete ui;
}
void DictionaryForm::init()
{

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

}
