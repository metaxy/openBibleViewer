#ifndef MDIAREA_H
#define MDIAREA_H

#include <QMdiArea>
#include "mdisubwindow.h"
class MdiArea : public QMdiArea
{
    Q_OBJECT
public:
    explicit MdiArea(QWidget *parent = 0);
    void resizeEvent(QResizeEvent * resizeEvent);
   // MdiSubWindow *addSubWindow(QWidget *widget, Qt::WindowFlags flags = 0);
    /*MdiSubWindow *currentSubWindow() const;
    MdiSubWindow *activeSubWindow() const;*/
signals:
    void resized();
public slots:
    
};

#endif // MDIAREA_H
