/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
#include <QDir>
#include <QScopedPointer>

#include <QMenu>
#include <QCursor>
#include <QMessageBox>
#include <QPrinter>
#include <QClipboard>
#include <QColorDialog>
#include <QMdiArea>
#include <QTextDocumentWriter>
#include <QCloseEvent>
#include <QToolBar>
#include <QContextMenuEvent>

#include <QtWebKitWidgets>

#include "src/core/dbghelper.h"
#include "src/core/moduletools.h"
#include "src/core/link/verseurl.h"
#include "src/core/history.h"
#include "src/core/module/versetable.h"


#include "src/ui/webview.h"
#include "src/ui/interface/advanced/form/webviewform.h"
#include "src/ui/interface/advanced/versetablewidget.h"


#include "form.h"
namespace Ui
{
class BibleForm;
}
/**
  * BibelForm contains a VerseTable.
  */
class BibleForm : public WebViewForm
{
    Q_OBJECT
    Q_DISABLE_COPY(BibleForm)
public:
    explicit BibleForm(QWidget *parent = 0);
    virtual ~BibleForm();

    void init();

    VerseSelection verseSelection();
    void restore(const QString &key);
    void save();
    Form::FormType type() const;
    ModuleID moduleID() const;

    void parseUrl(const VerseUrl &url, bool reload = false);
    void parseUrl(const QString &url);

    void nextChapter();
    void previousChapter();

    SearchableModule * searchableModule() const;
    VerseTable *verseTable() const;
    virtual TextRangesVerseModule *verseModule() const;

    ModuleID newModule(const ModuleID moduleID);

    /**
     * @brief newModule() load just a (random) TextRanges Module which we can use. If there is a default module, it loads this one;
     * @return moduleID
     */
    ModuleID newModule();

    void addParallelH(const int moduleID);
    void addParallelV(const int moduleID);

    bool verseTableLoaded();
private slots:
    void showBibleListMenu();
    void readBook(int id);
    void readChapter(int id);
    void attachApi();
signals:
    void onClose();

public slots:

    void backward();
    void forward();

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
    void forwardSearchInText(SearchResult *result);
    void forwardClear();

    void forwardDeleteModule(const int itemID);

    void activated();

    void showText(const QString &text);

    void evaluateJavaScript(const QString &js);

    void reload(bool full);
private slots:
    void moduleChanged(const int moduleID);
    void openCommentary();

    void historySetUrl(QString url);
    void searchInText(SearchResult *res);
    void showTextRanges(const QString &html, const TextRanges &range, const VerseUrl &url);

    void showContextMenu(QContextMenuEvent* ev);
    void copyWholeVerse();
    void newColorMark();
    void newCustomColorMark();
    void newBoldMark();
    void newItalicMark();
    void newUnderlineMark();
    void removeMark();
    void newNoteWithLink();
    void newBookmark();
    void deleteModule(const int itemID);
    void reloadIf(const VerseUrl &url);

protected:
    virtual void changeEvent(QEvent *e);
    virtual QString getStyleSheetUrl();
private:
    History m_browserHistory;

    QMenu *m_menuMark;
    QAction *m_actionCopy;
    QAction *m_actionSelect;
    QAction *m_actionRemoveMark;
    QAction *m_actionBookmark;
    QAction *m_actionNote;

    void setButtons();
    void createDefaultMenu();
    void deleteDefaultMenu();
    void showRanges(const Ranges &ranges, const VerseUrl &url, bool showStart = false, bool ignoreModuleID = true);

    TextRanges m_lastTextRanges;
    VerseUrl m_lastUrl;

    VerseTable *m_verseTable;
    Ui::BibleForm *m_ui;

    VerseSelection m_lastSelection;
    QList<int> m_bookIDs;

    VerseUrl m_url;
};

#endif // BIBLEFORM_H
