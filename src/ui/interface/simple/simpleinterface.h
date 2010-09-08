/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
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
    void settingsChanged(Settings oldSettings, Settings newSettings);
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
    QList<QToolBar*> toolBars();

protected:
    void changeEvent(QEvent *e);
    bool eventFilter(QObject *obj, QEvent *ev);
signals:
    void get(QString);
    void reloadInterface();
private:
    Ui::SimpleInterface *ui;
    WindowCache m_windowCache;
    void setTitle(const QString &title);
    void setChapters(const QStringList &chapters);
    void setBooks(const QHash<int, QString> &books);
    void setCurrentBook(const int &bookID);
    void setCurrentChapter(const int &chapterID);
    void readBook(const int &id);
    void readBookByID(int id);

    void readChapter(const int &id);
    void showChapter(int chapterID, int verseID);
    void nextChapter();
    void previousChapter();
};

#endif // SIMPLEINTERFACE_H
