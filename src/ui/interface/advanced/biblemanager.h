#ifndef BIBLEMANAGER_H
#define BIBLEMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include "src/ui/dock/dockwidget.h"
#include "src/core/basicclass.h"
#include "src/ui/dock/bookdockwidget.h"
#include "src/ui/dock/moduledockwidget.h"
#include "src/ui/dock/advancedsearchresultdockwidget.h"
#include "src/ui/dock/quickjumpdockwidget.h"
#include "src/core/bible/bibleurlrange.h"
class BibleManager : public QObject, public BasicClass
{
    Q_OBJECT
public:
    explicit BibleManager(QObject *parent = 0);
    void init();
    void setWidget(QWidget *p);
    void createDocks();
    QHash<DockWidget *, Qt::DockWidgetArea> docks();

    //todo:

    bool loadModuleDataByID(const int &id);
    void readBookByID(const int &id);
    void showChapter(const int &chapterID, const int &verseID);

signals:
    void setTitle(const QString &title);
    void updateChapters(const QStringList &chapters);
    void updateBooks(const QHash<int, QString> &books, QList<int> ids);
    void updateChapters();
    void updateBooks();

    void setCurrentBook(const int &bookID);
    void setCurrentChapter(const int &chapterID);

public slots:
    //todo:
    void pharseUrl(const QString &url);
    void pharseUrl(const BibleUrl &url);
    /*
        void readChapter(const int &id);
        void readBook(const int &id);
    */
    //void nextVerse();
    //void previousVerse();

    void nextChapter();
    void previousChapter();
    //int copyWholeVerse();
    void reloadChapter(bool full = false);
private:
    void showRanges(Ranges ranges);
    Ranges bibleUrlRangeToRanges(BibleUrlRange r);
    QWidget *m_p;
    AdvancedSearchResultDockWidget *m_advancedSearchResultDockWidget;
    BookDockWidget *m_bookDockWidget;
    ModuleDockWidget *m_moduleDockWidget;
    QuickJumpDockWidget * m_quickJumpDockWidget;
};

#endif // BIBLEMANAGER_H
