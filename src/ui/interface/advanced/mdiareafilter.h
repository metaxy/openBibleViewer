#ifndef MDIAREAFILTER_H
#define MDIAREAFILTER_H

#include <QObject>

class MdiAreaFilter : public QObject
{
Q_OBJECT
public:
    explicit MdiAreaFilter(QObject *parent = 0);

signals:
    void resized();
public slots:

protected:
    bool eventFilter(QObject *obj, QEvent *event);

};

#endif // MDIAREAFILTER_H
