#include "bookmarksdockwidget.h"
#include "ui_bookmarksdockwidget.h"

BookmarksDockWidget::BookmarksDockWidget(QWidget *parent) :
        DockWidget(parent),
        ui(new Ui::BookmarksDockWidget)
{
    ui->setupUi(this);
}

BookmarksDockWidget::~BookmarksDockWidget()
{
    delete ui;
}

void BookmarksDockWidget::changeEvent(QEvent *e)
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
