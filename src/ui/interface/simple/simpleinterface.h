#ifndef SIMPLEINTERFACE_H
#define SIMPLEINTERFACE_H

#include <QWidget>
#include <QtCore/QUrl>

#include "src/ui/interface/interface.h"
#include "src/ui/dock/moduledockwidget.h"
#include "src/ui/dock/bookdockwidget.h"
#include "src/ui/dock/searchresultdockwidget.h"
#include "src/core/windowcache.h"
namespace Ui
{
class SimpleInterface;
}

class SimpleInterface : public Interface
{
    Q_OBJECT
public slots:
    void settingsChanged(Settings settings);
private slots:
    void pharseUrl(QUrl url);
    void pharseUrl(QString url);
    void showText(const QString &text);
    void zoomIn();
    void zoomOut();
    void showSearchDialog();
    void search(SearchQuery query);
public:
    SimpleInterface(QWidget *parent = 0);
    ~SimpleInterface();
    void setModuleDockWidget(ModuleDockWidget *moduleDockWidget);
    void setBookDockWidget(BookDockWidget *bookDockWidget);
    void setSearchResultDockWidget(SearchResultDockWidget *searchResultDockWidget);
    void init();
    void loadModuleDataByID(int id);



    bool hasMenuBar();
    QMenuBar* menuBar();

    bool hasToolBar();
    QToolBar* toolBar();
    void showSettingsDialog(int tabID);

protected:
    void changeEvent(QEvent *e);
    bool eventFilter(QObject *obj, QEvent *ev);
signals:
    void get(QString);
private:
    Ui::SimpleInterface *ui;
    ModuleDockWidget *m_moduleDockWidget;
    BookDockWidget *m_bookDockWidget;
    SearchResultDockWidget *m_searchResultDockWidget;
    WindowCache m_windowCache;
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

#endif // SIMPLEINTERFACE_H
