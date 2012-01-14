#include "mdisubwindow.h"
#include "src/core/dbghelper.h"
MdiSubWindow::MdiSubWindow(QWidget *parent) :
    QMdiSubWindow(parent)
{
    DEBUG_FUNC_NAME;
}
void MdiSubWindow::closeEvent (QCloseEvent * closeEvent)
{
    DEBUG_FUNC_NAME;
    QMdiSubWindow::closeEvent(closeEvent);
}
