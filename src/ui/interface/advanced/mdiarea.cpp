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

/*
MdiSubWindow* MdiArea::currentSubWindow() const
{
    return (MdiSubWindow*) QMdiArea::currentSubWindow();
}
MdiSubWindow* MdiArea::activeSubWindow() const
{
    return (MdiSubWindow*) QMdiArea::activeSubWindow();
}*/
