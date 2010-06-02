#include "biblelistwidget.h"
#include "ui_biblelistwidget.h"
#include "src/core/dbghelper.h"
BibleListWidget::BibleListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BibleListWidget)
{
    ui->setupUi(this);

}
void BibleListWidget::init()
{
    QMap<QString, int> map;

     foreach (QString str, map.keys())
         qDebug() << str << ":" << map.value(str);
}

BibleListWidget::~BibleListWidget()
{
    delete ui;
}

void BibleListWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
