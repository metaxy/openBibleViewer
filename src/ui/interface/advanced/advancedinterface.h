#ifndef ADVANCEDINTERFACE_H
#define ADVANCEDINTERFACE_H

#include <QWidget>
#include "src/ui/interface/interface.h"
#include "src/ui/dock/bookdockwidget.h"
#include "src/ui/dock/moduledockwidget.h"
#include "src/ui/dock/searchresultdockwidget.h"
#include "src/ui/dock/notesdockwidget.h"
#include "src/ui/dock/bookmarksdockwidget.h"
#include "src/ui/dock/strongdockwidget.h"
#include "src/core/windowcache.h"
#include <QtGui/QMdiSubWindow>
#include <QtGui/QCloseEvent>
#include "mdiform.h"
namespace Ui
{
class AdvancedInterface;
}

class AdvancedInterface : public Interface
{
    Q_OBJECT
public slots:
    void closing();
    void settingsChanged(Settings settings);
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
    void readChapter(const int &id);
    void readBook(const int &id);
    void nextChapter();
    void previousChapter();
    int textBrowserContextMenu(QPoint);
    int copyWholeVerse();
    void newYellowMark();
    void newGreenMark();
    void newBlueMark();
    void newOrangeMark();
    void newVioletMark();
    void reloadChapter();
    void removeMark();
    void showSearchDialog();
    void search(SearchQuery query);
    void showBookmarksDock();
    void showNotesDock();
    void newBookmark();
    void newNoteWithLink();
    int showAboutDialog();
    int saveFile();
    int printFile();
    void onlineHelp();
    void copy();
    void selectAll();
    void nextVerse();
    void previousVerse();

public:
    AdvancedInterface(QWidget *parent = 0);
    ~AdvancedInterface();
    void init();
    bool hasMenuBar();
    QMenuBar* menuBar();
    bool hasToolBar();
    QToolBar* toolBar();
    void setBookDockWidget(BookDockWidget *bookDockWidget);
    void setModuleDockWidget(ModuleDockWidget *moduleDockWidget);
    void setSearchResultDockWidget(SearchResultDockWidget *searchResultDockWidget);
    void setNotesDockWidget(NotesDockWidget *notesDockWidget);
    void setBookmarksDockWidget(BookmarksDockWidget *boockmarksDockWidget);
    void setStrongDockWidget(StrongDockWidget *strongDockWidget);

    SearchResultDockWidget *m_searchResultDockWidget;
    BookDockWidget *m_bookDockWidget;
    ModuleDockWidget *m_moduleDockWidget;
    NotesDockWidget *m_notesDockWidget;
    BookmarksDockWidget * m_bookmarksDockWidget;
    StrongDockWidget * m_strongDockWidget;

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

    void readBookByID(int id);
    VerseSelection verseSelectionFromCursor(QTextCursor cursor);

    void showChapter(const int &chapterID, const int &verseID);
    QTextCursor m_textCursor;


};

#endif // ADVANCEDINTERFACE_H
