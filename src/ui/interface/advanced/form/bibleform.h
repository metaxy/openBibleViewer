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
#include "src/core/obvcore.h"
#include "src/core/link/verseurl.h"
#include "src/core/history.h"
#include "src/module/versetable.h"


#include "src/ui/webview.h"

#include "src/ui/interface/advanced/versetablewidget.h"


#include "form.h"
namespace Ui
{
class BibleForm;
}

class BibleForm : public Form
{
    Q_OBJECT
    Q_DISABLE_COPY(BibleForm)
public:
    explicit BibleForm(QWidget *parent = 0);
    virtual ~BibleForm();

    void init();

    WebView *m_view;

    VerseSelection verseSelection();
    void restore(const QString &key);
    void save();
    Form::FormType type() const;

    void pharseUrl(const VerseUrl &url);
    void pharseUrl(const QString &url);

    void nextChapter();
    void previousChapter();

    SearchableModule * searchableModule() const;
    VerseTable *verseTable() const;
    VerseModule *verseModule() const;

private slots:
    void showBibleListMenu();
    void readBook(int id);
    void readChapter(int id);
    void attachApi();
signals:
    void onClose();

public slots:
    void historySetUrl(QString url);
    void backward();
    void forward();
    void zoomIn();
    void zoomOut();

    void setChapters(int bookID, QSharedPointer<Versification> v11n);
    void setBooks(QSharedPointer<Versification> v11n);

    void clearBooks();
    void clearChapters();
    void clear();

    void setCurrentBook(const QSet<int> &bookID);
    void setCurrentChapter(const QSet<int> &chapterID);

    void forwardSetChapters(int bookID, QSharedPointer<Versification> v11n);
    void forwardSetBooks(QSharedPointer<Versification> v11n);
    void forwardClearBooks();
    void forwardClearChapters();
    void forwardSetCurrentBook(const QSet<int> &bookID);
    void forwardSetCurrentChapter(const QSet<int> &chapterID);
    void forwardShowText(const QString &text);
    void forwardSearchInText(SearchResult *result);
    void forwardClear();
    void reloadIf(const VerseUrl &url);

    void activated();

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
    void searchInText(SearchResult *res);

    void reload(bool full);
protected:
    virtual void changeEvent(QEvent *e);
private:
    History m_browserHistory;

    void setButtons();

    QAction *m_actionCopy;
    QAction *m_actionSelect;
    QMenu *m_menuMark;
    QAction *m_actionRemoveMark;
    QAction *m_actionBookmark;
    QAction *m_actionNote;

    void createDefaultMenu();
    void deleteDefaultMenu();

    TextRanges m_lastTextRanges;
    VerseUrl m_lastUrl;

    QWebInspector *m_inspector;
    VerseTable *m_verseTable;
    Ui::BibleForm *m_ui;

    VerseSelection lastSelection;
    QList<int> m_bookIDs;

    void showRanges(const Ranges &ranges, const VerseUrl &url);

    bool verseTableLoaded();
    VerseUrl m_url;
};

#endif // BIBLEFORM_H
