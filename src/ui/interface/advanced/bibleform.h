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
    void init();
    void setApi(Api *api);

    Ui::BibleForm *m_ui;
    WebView *m_view;
    QList<int> m_bookIDs;
    BibleList *m_bibleList;
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
    void historyGetUrl(QString url);
    void backward();
    void forward();
    void zoomIn();
    void zoomOut();

    void setChapters(const QStringList &chapters);
    void setBooks(const QHash<int, QString> &books, QList<int> ids);

    void clearBooks();
    void clearChapters();

    void setCurrentBook(const int &bookID);
    void setCurrentChapter(const int &chapterID);

    void activated();

    void scrollToAnchor(const QString &anchor);

    void showText(const QString &text);

    void evaluateJavaScript(const QString &js);
    void print();
    void saveFile();
    QString selectedText();

    void copy();
    void selectAll();

protected:
    virtual void changeEvent(QEvent *e);
private:
    History browserHistory;
    Api *m_api;//not in out control
    void setButtons();
};

#endif // BIBLEFORM_H
