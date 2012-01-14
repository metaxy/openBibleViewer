#include "subwindowfilter.h"
#include "src/core/dbghelper.h"
SubWindowFilter::SubWindowFilter(QObject *parent) :
    QObject(parent)
{
}
bool SubWindowFilter::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Close) {
        myDebug() << "emmiting close";
        QObject::eventFilter(obj, event);
        emit close();
        return true;
    }
    return QObject::eventFilter(obj, event);
}
