#include "mdisubwindow.h"

MdiSubWindow::MdiSubWindow(QWidget *parent) :
    QMdiSubWindow(parent)
{
}
void MdiSubWindow::closeEvent(QCloseEvent * closeEvent)
{
    emit close();
    QMdiSubWindow::closeEvent(closeEvent);
}
