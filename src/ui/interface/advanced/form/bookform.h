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
#ifndef BOOKFORM_H
#define BOOKFORM_H
#include "webviewform.h"
#include <QWidget>
#include "src/core/module/book/book.h"
namespace Ui {
class BookForm;
}

class BookForm : public WebViewForm
{
    Q_OBJECT
    
public:
    explicit BookForm(QWidget *parent = 0);
    ~BookForm();
    void init();
    void restore(const QString &key);
    void save();

    Form::FormType type() const;


    void loadModule(const int moduleID);

    void show();
public slots:
    void activated();
private slots:
    void showContextMenu(QContextMenuEvent* ev);
    void debugger();
private:
    Ui::BookForm *ui;
    void actTitle();

    Book *m_book;
};

#endif // BOOKFORM_H
