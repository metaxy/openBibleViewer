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
#ifndef BIBLEFORM_H
#define BIBLEFORM_H

#include <QtGui/QWidget>
#include <QtGui/QCloseEvent>
#include <QtGui/QToolBar>
#include "src/core/basicclass.h"
#include "ui_bibleform.h"
#include "src/core/history.h"
#include "webview.h"
#include <QContextMenuEvent>
#include "src/module/biblelist.h"
#include "src/api/api.h"
#include "biblemanager.h"
#include "notesmanager.h"

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

    Ui::BibleForm *m_ui;
    WebView *m_view;
    QList<int> m_bookIDs;
    BibleList *m_bibleList;

    VerseSelection verseSelection();//todo:

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
    void setBooks(const QHash<int, QString> &books, QList<int> ids);

    void clearBooks();
    void clearChapters();

    void setCurrentBook(const QSet<int> &bookID);
    void setCurrentChapter(const QSet<int> &chapterID);

    void forwardSetChapters(const QStringList &chapters);
    void forwardSetBooks(const QHash<int, QString> &books, QList<int> ids);

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
    void forwardShowTextRanges(const QString &html, const TextRanges &range, const BibleUrl &url);
    void showTextRanges(const QString &html, const TextRanges &range, const BibleUrl &url);



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


    BibleManager *m_bibleManager;
    NotesManager *m_notesManager;
    bool active();

    TextRanges m_lastTextRanges;
    BibleUrl m_lastUrl;
};

#endif // BIBLEFORM_H
