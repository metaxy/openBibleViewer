#include "mdisubwindow.h"
#include "src/core/dbghelper.h"
MdiSubWindow::MdiSubWindow(QWidget *parent) :
    QMdiSubWindow(parent)
{
}
void MdiSubWindow::closeEvent(QCloseEvent * closeEvent)
{
    DEBUG_FUNC_NAME;
    emit close();
    QMdiSubWindow::closeEvent(closeEvent);
}
