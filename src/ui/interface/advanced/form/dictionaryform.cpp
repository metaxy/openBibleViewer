#include "dictionaryform.h"
#include "ui_dictionaryform.h"

DictionaryForm::DictionaryForm(QWidget *parent) :
    Form(parent),
    ui(new Ui::DictionaryForm)
{
    ui->setupUi(this);
}

DictionaryForm::~DictionaryForm()
{
    delete ui;
}
