#include "mdiareafilter.h"
#include <QtCore/QEvent>
#include <QtGui/QResizeEvent>
MdiAreaFilter::MdiAreaFilter(QObject *parent) :
    QObject(parent)
{
}
bool MdiAreaFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Resize) {
        QResizeEvent *resizeEvent = static_cast<QResizeEvent *>(event);
        emit resized();
        return true;
    }
    return QObject::eventFilter(obj, event);
}
