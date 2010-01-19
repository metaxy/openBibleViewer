#ifndef BOOKDOCKWIDGET_H
#define BOOKDOCKWIDGET_H

#include <QtGui/QDockWidget>
#include <QtCore/QStringList>
#include "dockwidget.h"
namespace Ui
{
class BookDockWidget;
}
/*!
 BockDockWidget represents a dock widget, to navigate through the bible

*/
class BookDockWidget : public DockWidget
{
    Q_OBJECT
private slots:
    void readBook(QListWidgetItem * item);
    void readChapter(QListWidgetItem * item);
public:
    BookDockWidget(QWidget *parent = 0);
    ~BookDockWidget();
    void setChapters(const QStringList &chapters);
    void setBooks(const QStringList &books);
    void setCurrentBook(const int &bookID);
    void setCurrentChapter(const int &chapterID);

protected:
    void changeEvent(QEvent *e);
signals:
    void get(QString);
private:
    Ui::BookDockWidget *ui;

};

#endif // BOOKDOCKWIDGET_H
