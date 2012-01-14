#ifndef MDISUBWINDOW_H
#define MDISUBWINDOW_H

#include <QMdiSubWindow>

class MdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit MdiSubWindow(QWidget *parent = 0);
protected:
    virtual void closeEvent (QCloseEvent * closeEvent);
signals:
    
public slots:
    
};

#endif // MDISUBWINDOW_H
