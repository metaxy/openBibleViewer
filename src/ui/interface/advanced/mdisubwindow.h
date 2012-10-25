#ifndef MDISUBWINDOW_H
#define MDISUBWINDOW_H

#include <QtGui/QMdiSubWindow>

class MdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit MdiSubWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent * closeEvent);
signals:
    void close();
public slots:
    
};

#endif // MDISUBWINDOW_H
