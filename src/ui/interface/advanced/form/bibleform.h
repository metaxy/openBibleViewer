/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#ifndef BIBLEFORM_H
#define BIBLEFORM_H
#include <QtCore/QDir>
#include <QtCore/QScopedPointer>

#include <QtGui/QMenu>
#include <QtGui/QCursor>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtGui/QClipboard>
#include <QtGui/QColorDialog>
#include <QtGui/QMdiArea>
#include <QtGui/QPrintPreviewDialog>
#include <QtGui/QTextDocumentWriter>
#include <QtGui/QCloseEvent>
#include <QtGui/QToolBar>
#include <QtGui/QContextMenuEvent>

#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElementCollection>
#include <QtWebKit/QWebInspector>

#include "src/core/dbghelper.h"
#include "src/core/core.h"
#include "src/core/verse/verseurl.h"
#include "src/core/basicclass.h"
#include "src/core/history.h"
#include "src/module/versetable.h"
#include "src/api/api.h"

#include "src/ui/interface/advanced/webview.h"
#include "src/ui/interface/advanced/manager/biblemanager.h"
#include "src/ui/interface/advanced/manager/notesmanager.h"
#include "src/ui/interface/advanced/biblelistwidget.h"
#include "src/ui/interface/advanced/manager/bookmarksmanager.h"
namespace Ui
{
class BibleForm;
}

class BibleForm : public QWidget, public BasicClass
{
    Q_OBJECT
    Q_DISABLE_COPY(BibleForm)
public:
    explicit BibleForm(QWidget *parent = 0);
    virtual ~BibleForm();
    void setID(const int &id);
    int id();
    void init();
    void setApi(Api *api);
    void setBibleManager(BibleManager *bibleManager);
    void setNotesManager(NotesManager *notesManager);
    void setBookmarksManager(BookmarksManager *bookmarksManager);

    Ui::BibleForm *m_ui;
    WebView *m_view;
    QList<int> m_bookIDs;
    VerseTable *m_verseTable;

    VerseSelection verseSelection();

    int *currentWindowID;
private slots:
    void showBibleListMenu();
    void readBook(int id);
    void readChapter(int id);
    void attachApi();
signals:
    void onClose();
    void historyGo(QString);
    void previousChapter();
    void nextChapter();
public slots:
    void historySetUrl(QString url);
    void backward();
    void forward();
    void zoomIn();
    void zoomOut();

    void setChapters(const QStringList &chapters);
    void setBooks(Versification *v11n);

    void clearBooks();
    void clearChapters();

    void setCurrentBook(const QSet<int> &bookID);
    void setCurrentChapter(const QSet<int> &chapterID);

    void forwardSetChapters(const QStringList &chapters);
    void forwardSetBooks(Versification *v11n);
    void forwardClearBooks();
    void forwardClearChapters();
    void forwardSetCurrentBook(const QSet<int> &bookID);
    void forwardSetCurrentChapter(const QSet<int> &chapterID);
    void forwardShowText(const QString &text);
    void forwardHistorySetUrl(const QString &url);


    void activated();

    void scrollToAnchor(const QString &anchor);

    void showText(const QString &text);

    void evaluateJavaScript(const QString &js);
    void print();
    void printPreview();
    void saveFile();
    QString selectedText();

    void copy();
    void selectAll();

    void showContextMenu(QContextMenuEvent* ev);
    void copyWholeVerse();
    void debugger();
    void newColorMark();
    void newCustomColorMark();
    void newBoldMark();
    void newItalicMark();
    void newUnderlineMark();
    void removeMark();
    void newNoteWithLink();
    void newBookmark();
    void forwardShowTextRanges(const QString &html, const TextRanges &range, const VerseUrl &url);
    void showTextRanges(const QString &html, const TextRanges &range, const VerseUrl &url);

protected:
    virtual void changeEvent(QEvent *e);
private:
    int m_id;
    History browserHistory;
    Api *m_api;//not in out control
    void setButtons();

    QAction *m_actionCopy;
    QAction *m_actionSelect;
    QMenu *m_menuMark;
    QAction *m_actionRemoveMark;
    QAction *m_actionBookmark;
    QAction *m_actionNote;
    void createDefaultMenu();
    void deleteDefaultMenu();

    BibleManager *m_bibleManager;
    NotesManager *m_notesManager;
    BookmarksManager *m_bookmarksManager;
    bool active();

    TextRanges m_lastTextRanges;
    VerseUrl m_lastUrl;

    QWebInspector *m_inspector;
};

#endif // BIBLEFORM_H
