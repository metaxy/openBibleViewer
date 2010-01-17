#ifndef ADVANCEDINTERFACE_H
#define ADVANCEDINTERFACE_H

#include <QWidget>
#include "src/ui/interface/interface.h"
#include "src/ui/dock/bookdockwidget.h"
#include "src/ui/dock/moduledockwidget.h"
#include "src/ui/dock/searchresultdockwidget.h"
#include "src/core/windowcache.h"
#include <QtGui/QMdiSubWindow>
#include "mdiform.h"
namespace Ui
{
class AdvancedInterface;
}

class AdvancedInterface : public Interface
{
    Q_OBJECT
private slots:
    void myCascade();
    void myTileVertical();
    void myTileHorizontal();
    void newMdiChild();
    int closeWindow();
    int reloadWindow(QMdiSubWindow * window);
    void pharseUrl(QUrl url);
    void pharseUrl(QString url);
    void showText(const QString &text);
    void zoomIn();
    void zoomOut();
  //  void showSearchDialog();
    //void search(SearchQuery query);
public:
    AdvancedInterface(QWidget *parent = 0);
    ~AdvancedInterface();
    void init();
    void setBookDockWidget(BookDockWidget *bookDockWidget);
    void setModuleDockWidget(ModuleDockWidget *moduleDockWidget);
    void setSearchResultDockWidget(SearchResultDockWidget *searchResultDockWidget);

    SearchResultDockWidget *m_searchResultDockWidget;
    BookDockWidget *m_bookDockWidget;
    ModuleDockWidget *m_moduleDockWidget;


protected:
    void changeEvent(QEvent *e);
signals:
    void get(QString);
    void historySetUrl(QString url);
private:
    Ui::AdvancedInterface *ui;
    bool m_enableReload;
    QMdiSubWindow *activeMdiChild();
    QList<QMdiSubWindow*> usableWindowList();
    QList<QMdiSubWindow *> m_internalWindows;
    int currentWindowID();
    int tabIDof(QMdiSubWindow* window);
    WindowCache m_windowCache;
    int m_lastActiveWindow;
    QTextBrowser* getCurrentTextBrowser();
    void setEnableReload(bool enable);
    void loadModuleDataByID(int id);

    void setTitle(const QString &title);
    void setChapters(const QStringList &chapters);
    void setBooks(const QStringList &books);
    void setCurrentBook(const int &bookID);
    void setCurrentChapter(const int &chapterID);
    void readBook(const int &id);
    void readBookByID(int id);

    void readChapter(const int &id);
    void showChapter(const int &chapterID, const int &verseID);
    void nextChapter();
    void previousChapter();
};

#endif // ADVANCEDINTERFACE_H
