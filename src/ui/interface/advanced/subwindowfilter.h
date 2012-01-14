#ifndef SUBWINDOWFILTER_H
#define SUBWINDOWFILTER_H

#include <QObject>
#include <QtCore/QEvent>

class SubWindowFilter : public QObject
{
        Q_OBJECT
public:
    explicit SubWindowFilter(QObject *parent = 0);
signals:
    void close();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // SUBWINDOWFILTER_H
