#include "history.h"
#include <QtCore/QtDebug>
history::history()
{
}

QString history::forward()
{
    qDebug() << "history::forward() items = " << forwardItems;
    if (!forwardItems.isEmpty()) {
        goF = true;
        return forwardItems.takeLast();
    } else
        return QString();
}
QString history::backward()
{
    qDebug() << "history::backward() items = " << backwardItems;
    if (backwardItems.size() >= 2) {
        goF = true;
        forwardItems.append(backwardItems.takeLast());
        return backwardItems.takeLast();
    } else
        return QString();
}
bool history::forwardAvailable()
{
    return !forwardItems.isEmpty();
}
bool history::backwardAvailable()
{
    return backwardItems.size() >= 2;
}
bool history::setCurrent(QString url)
{
    if (goF == true) {
        goF = false;
    } else {
        forwardItems.clear();
    }
    backwardItems.append(url);
    return true;
}
