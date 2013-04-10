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
#ifndef VERSETABLEWIDGET_H
#define VERSETABLEWIDGET_H

#include <QDialog>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QStandardItemModel>

#include "src/core/basicclass.h"
#include "src/core/dbghelper.h"
#include "src/core/link/verseurl.h"

#include "versetableitemdelegate.h"
namespace Ui
{
class VerseTableWidget;
}

class VerseTableWidget : public QDialog, public BasicClass
{
    Q_OBJECT
public:
    explicit VerseTableWidget(QWidget *parent = 0);
    ~VerseTableWidget();
    void init();
    void setVerseTable(VerseTable *table);

protected:
    void changeEvent(QEvent *e);
private slots:
    void removeCol();
    void addCol();

    void removeRow();
    void addRow();

    void save();
signals:
    void loadVerseTable(bool hasBible);
    void clear();
private:
    Ui::VerseTableWidget *ui;
    QStandardItemModel *m_model;
    QItemSelectionModel *m_selectionModel;

    VerseTable *m_verseTable;
};

#endif // VERSETABLEWIDGET_H
