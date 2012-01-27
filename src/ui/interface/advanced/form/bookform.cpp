#include "bookform.h"
#include "ui_bookform.h"

BookForm::BookForm(QWidget *parent) :
    Form(parent),
    ui(new Ui::BookForm)
{
    ui->setupUi(this);
}

BookForm::~BookForm()
{
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
void BookForm::copy()
{

}

void BookForm::selectAll()
{

}

void BookForm::print()
{

}

void BookForm::printPreview()
{

}

void BookForm::saveFile()
{

}

QString BookForm::selectedText()
{
    return QString();
}

void BookForm::zoomIn()
{

}

void BookForm::zoomOut()
{

}

void BookForm::activated()
{

}
