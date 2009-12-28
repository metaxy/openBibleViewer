#include "searchresultdockwidget.h"
#include "ui_searchresultdockwidget.h"

SearchResultDockWidget::SearchResultDockWidget(QWidget *parent) :
        DockWidget(parent),
        ui(new Ui::SearchResultDockWidget)
{
    ui->setupUi(this);
}

SearchResultDockWidget::~SearchResultDockWidget()
{
    delete ui;
}

void SearchResultDockWidget::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
