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
#ifndef TREEBOOKFORM_H
#define TREEBOOKFORM_H
#include "webviewform.h"
#include "src/module/book/treebook.h"
#include "src/core/history.h"
#include <QWidget>

namespace Ui {
class TreeBookForm;
}

class TreeBookForm : public WebViewForm
{
    Q_OBJECT
    
public:
    explicit TreeBookForm(QWidget *parent = 0);
    ~TreeBookForm();

    void init();
    void restore(const QString &key);
    void save();

    Form::FormType type() const;


    void loadModule(const int moduleID);

    void showChapter(const int moduleID, const int chapterID);
public slots:
    void activated();
private slots:
    void loadChapter(QModelIndex index);
    void backward();
    void forward();
    void showContextMenu(QContextMenuEvent* ev);
    void get(QUrl url);
    void newGet(QUrl url);
private:
    Ui::TreeBookForm *ui;

    TreeBook *m_book;

    int m_chapterID;

    void setButtons();
    void historySetUrl(QString url);
    History m_browserHistory;

    void createTree(QStandardItem *parentItem, BookTree *parentTree);
    void selectChapter(const int chapterID);
    QString transformUrl(const QString &url);

    RecursivProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;
    QStandardItemModel *m_treeModel;
};

#endif // TREEBOOKFORM_H
