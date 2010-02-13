#include "quickjumpdockwidget.h"
#include "ui_quickjumpdockwidget.h"
#include "src/core/goto.h"
#include "src/core/dbghelper.h"
#include <QtGui/QKeyEvent>
QuickJumpDockWidget::QuickJumpDockWidget(QWidget *parent) :
        DockWidget(parent),
        ui(new Ui::QuickJumpDockWidget)
{
    ui->setupUi(this);
    connect(ui->pushButton_goTo, SIGNAL(clicked()), this, SLOT(goToPos()));
    ui->lineEdit_goTo->installEventFilter(this);
}

QuickJumpDockWidget::~QuickJumpDockWidget()
{
    delete ui;
}
void QuickJumpDockWidget::goToPos()
{
    QString text = ui->lineEdit_goTo->text();
    GoTo go(m_moduleManager->m_bible.bibleID(), m_moduleManager->m_bible.bookFullName);//todo:make it better for 0.4
    QString url = go.getUrl(text);
    emit get(url);
    return;
}
bool QuickJumpDockWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->lineEdit_goTo) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            myDebug() << keyEvent->key();
            if (keyEvent->key() == 16777220) {
                goToPos();
                return true;
            } else {
                return QDockWidget::eventFilter(obj, event);
            }

        } else {
            return QDockWidget::eventFilter(obj, event);;
        }
    } else {
        return QDockWidget::eventFilter(obj, event);
    }
}
void QuickJumpDockWidget::changeEvent(QEvent *e)
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
