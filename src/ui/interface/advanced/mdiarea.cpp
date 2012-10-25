#include "mdiarea.h"

MdiArea::MdiArea(QWidget *parent) :
    QMdiArea(parent)
{
}
void MdiArea::resizeEvent(QResizeEvent * resizeEvent)
{
    emit resized();
    QMdiArea::resizeEvent(resizeEvent);
}
MdiSubWindow *MdiArea::addSubWindow(QWidget *widget, Qt::WindowFlags flags)
{
    return (MdiSubWindow*) QMdiArea::addSubWindow(widget, flags);
}
