#ifndef NOTESDOCKWIDGET_H
#define NOTESDOCKWIDGET_H

#include <QDockWidget>
#include "dockwidget.h"
namespace Ui
{
class NotesDockWidget;
}

class NotesDockWidget : public DockWidget
{
    Q_OBJECT
public:
    NotesDockWidget(QWidget *parent = 0);
    ~NotesDockWidget();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::NotesDockWidget *ui;
};

#endif // NOTESDOCKWIDGET_H
