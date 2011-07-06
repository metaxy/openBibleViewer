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
