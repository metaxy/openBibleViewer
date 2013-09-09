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
#ifndef DICTIONARYFORM_H
#define DICTIONARYFORM_H

#include <QWidget>
#include "src/ui/webview.h"
#include "src/core/history.h"

namespace Ui
{
class DictionaryForm;
}

#include "src/ui/interface/advanced/form/webviewform.h"
class DictionaryForm :  public WebViewForm
{
    Q_OBJECT

public:
    explicit DictionaryForm(QWidget *parent = 0);
    ~DictionaryForm();
    void init();

    void restore(const QString &key);
    void save();
    Form::FormType type() const;
    Dictionary* dictionary() const;
    QString key() const;
public slots:
    void activated();
    void showEntry();
    void showEntry(const QString &key, int moduleID);

private slots:
    void get(const QUrl &url);
    void newGet(const QUrl &url);
    void backward();
    void forward();
    void showContextMenu(QContextMenuEvent* ev);

    void loadKey(const QModelIndex &index);
    void selectKey(const QString &key);

    void openInBrowser();
    void openInMenu();
protected:
    void changeEvent(QEvent *e);
    void showStringResponse(StringResponse *res);
private:
    Ui::DictionaryForm *ui;
    void historySetUrl(const QString &url);
    void loadDictionary(int moduleID);
    void testDictionary(const int module = -1);
    /**
     * @brief which module should i load
     * @param module
     * @return
     */
    int newDictionary(const int module = -1);
    Dictionary *m_dictionary;
    void showHtml(QString html);

    QString m_key;
    History m_browserHistory;
    void setButtons();

    void createDefaultMenu();
    void deleteDefaultMenu();
    QAction *m_actionSelect;

    void fillList();

    int m_lastFilledModuleID;

    QStandardItemModel *m_model;
    QItemSelectionModel *m_selectionModel;
    QSortFilterProxyModel *m_proxyModel;

    void actTitle();

};

#endif // DICTIONARYFORM_H
