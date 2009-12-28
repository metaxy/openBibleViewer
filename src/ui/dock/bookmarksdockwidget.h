#ifndef BOOKMARKSDOCKWIDGET_H
#define BOOKMARKSDOCKWIDGET_H

#include <QDockWidget>
#include "dockwidget.h"
namespace Ui
{
class BookmarksDockWidget;
}

class BookmarksDockWidget : public DockWidget
{
    Q_OBJECT
public:
    BookmarksDockWidget(QWidget *parent = 0);
    ~BookmarksDockWidget();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::BookmarksDockWidget *ui;
};

#endif // BOOKMARKSDOCKWIDGET_H
