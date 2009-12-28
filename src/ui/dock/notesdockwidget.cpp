#include "notesdockwidget.h"
#include "ui_notesdockwidget.h"

NotesDockWidget::NotesDockWidget(QWidget *parent) :
        DockWidget(parent),
        ui(new Ui::NotesDockWidget)
{
    ui->setupUi(this);
}

NotesDockWidget::~NotesDockWidget()
{
    delete ui;
}

void NotesDockWidget::changeEvent(QEvent *e)
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
