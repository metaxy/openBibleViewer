#include "marksdockwidget.h"
#include "ui_marksdockwidget.h"

MarksDockWidget::MarksDockWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MarksDockWidget)
{
    ui->setupUi(this);
}

MarksDockWidget::~MarksDockWidget()
{
    delete ui;
}
